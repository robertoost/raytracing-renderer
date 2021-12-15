#include "precomp.h"

namespace RaytracingRenderer {
    // Som
    float3 Raytracer::Trace(Ray &ray) {

        // Trace a ray and record the hit.
        hit_record rec = hit_record();
        const bool collision = scene->intersect(ray, 0.0001f, FLT_MAX, rec);
        
        float3 pixel_color = float3(0, 0, 0);

        // If there's no collision, return black.
        if (collision == false) {
            return float3(0.65f, 0.85f, 1.f);
        }

        // If a collision was found, get the color of the object.
        else {

            rec.mat_ptr->color(ray, rec, pixel_color);

            MAT_TYPE mat_type = rec.mat_ptr->type();

            if (mat_type == SOLID) {
                pixel_color *= TraceSolid(ray, rec, 0);
            }
            else if (mat_type == GLASS) {
                pixel_color *= TraceGlass(ray, rec, 0);
            }
        }
        return pixel_color;
    }

    float3 Raytracer::TraceReflection(Ray& ray, uint bounce_count) {

        // Trace the given reflection ray and find the nearest intersection.
        hit_record rec = hit_record();
        const bool collision = scene->intersect(ray, 0.0001f, FLT_MAX, rec);
        float3 pixel_color = float3(0,0,0);

        // If there's no collision, return black.
        if (collision == false) {
            return float3(0.65f, 0.85f, 1.f);
        }

        // Initially set the pixel color using the material's color.
        rec.mat_ptr->color(ray, rec, pixel_color);

        if (bounce_count > max_bounces) {
            return pixel_color;
        }

        if (rec.mat_ptr->type() == SOLID) {
            pixel_color *= TraceSolid(ray, rec, bounce_count);
        }
        // Reflect glass.
        if (rec.mat_ptr->type() == GLASS) {
            //cout << "INTERNAL REFLECTION";
            pixel_color *= TraceGlass(ray, rec, bounce_count);
        }

        return pixel_color;
    }

    float3 Raytracer::TraceRefraction(Ray& ray, uint bounce_count) {

        // negative bias to allow for self intersection?
        hit_record rec = hit_record();
        const bool collision = scene->intersect(ray, 0.0001f, FLT_MAX, rec);
        float3 pixel_color = float3(0,0,0);

        if (collision == false) {
            return float3(0.65f, 0.85f, 1.f);
        }

        rec.mat_ptr->color(ray, rec, pixel_color);

        if (bounce_count > max_bounces) {
            return pixel_color;
        }

        if (rec.mat_ptr->type() == GLASS) {
            //cout << "GLASS HIT GLASS";
            pixel_color *= TraceGlass(ray, rec, bounce_count);
        }
        else if (rec.mat_ptr->type() == SOLID) {
            pixel_color *= TraceSolid(ray, rec, bounce_count);
        }

        return pixel_color;
    }

    float3 Raytracer::TraceSolid(Ray& ray, hit_record& rec, uint bounce_count) {

        // Get the material's specularity and infer the diffuse value.
        const float specular = rec.mat_ptr->specularity();
        const float diffuse = 1 - specular;

        // Sum both the specular and the diffuse color.
        float3 spec_diff_color = float3(0, 0, 0);

        // Diffuse color value. On the first bounce, add direct illumination.
        if (diffuse > 0 && bounce_count <= max_reflection_shadows) {
            spec_diff_color += diffuse * DirectIllumination(rec.p, rec.normal);
        }
        // If no direct illumination is added, just add the material color.
        else if (diffuse > 0) {
            spec_diff_color += diffuse * float3(1, 1, 1);
        }

        // Specular color value. Trace new ray and increase bounce count to prevent crashing when mirrors face eachother. 
        if (specular > 0) {
            float3 reflect_dir = reflect(ray.dir, rec.normal);
            Ray reflect_ray = Ray(rec.p, reflect_dir);
            spec_diff_color += specular * TraceReflection(reflect_ray, bounce_count + 1);
        }

        return spec_diff_color;
    }

    float3 Raytracer::TraceGlass(Ray& ray, hit_record& rec, uint bounce_count) {
        float prev_ior, next_ior;
        get_ior(ray, rec, prev_ior, next_ior);
        float reflection, transmission;
        fresnel(ray.dir, rec.normal, prev_ior, next_ior, reflection, transmission);

        float3 glass_pixel_color = float3(0, 0, 0);

        if (transmission > 0) {

            float3 refract_dir = refract(ray.dir, rec.normal, prev_ior, next_ior);
            Ray refract_ray = Ray(rec.p, refract_dir);

            float3 transmission_color = transmission * TraceRefraction(refract_ray, bounce_count + 1);

            if (rec.front_face == false) {
                float3 absorption = rec.mat_ptr->absorption();
                float distance = length(rec.p - ray.orig);
                transmission_color.x *= exp(-absorption.x * distance);
                transmission_color.y *= exp(-absorption.y * distance);
                transmission_color.z *= exp(-absorption.z * distance);
            }

            glass_pixel_color += transmission_color;
        }

        if (reflection > 0) {
            float3 reflect_dir = reflect(ray.dir, rec.normal);
            Ray reflect_ray = Ray(rec.p, reflect_dir);
            glass_pixel_color += reflection * TraceReflection(reflect_ray, bounce_count + 1);
        }

        return glass_pixel_color;
    }

    float3 Raytracer::DirectIllumination(float3 &position, float3 &normal) {
        float3 pixel_lighting = float3(0, 0, 0);

        for (shared_ptr<Light> light : scene->lights) {
            pixel_lighting += light->illuminate(position, normal, *scene);
        }
        return pixel_lighting;
    }

    void Raytracer::RenderScene(float3 frame[SCRHEIGHT][SCRWIDTH]) {
        if (multithreading) //MULTI THREADING
        {
            imageBlocks.clear();
            completedThreads = { 0 };
            threads.clear();
            struct Images {
                float2 coords;
                float3 color;
            };
            vector<Images> image(SCRHEIGHT * SCRWIDTH);

            for (int i = 0; i < nThreads; ++i) {
                Blockjob job;
                job.rowStart = i * rowsPerThread;
                job.rowEnd = job.rowStart + rowsPerThread;

                if (i == nThreads - 1)
                {
                    job.rowEnd = job.rowStart + rowsPerThread + leftOver;
                }

                job.colSize = SCRWIDTH;
                job.samples_per_pixel = antialiasing == true ? samples_per_pixel : 1;

                thread t([this, job]() {
                    CalculateColor(job);
                    });

                threads.push_back(move(t));
            }

            {
                unique_lock<std::mutex> lock(mute);
                cvResults.wait(lock, [&] {
                    return completedThreads == nThreads;
                    });
            }
            for (thread& t : threads) {
                t.join();
            }
            for (Blockjob job : imageBlocks) {
                for (int i = 0; i < job.colors.size(); i++)
                {
                    Images images;
                    images.color = job.colors[i];
                    images.coords = job.indices[i];
                    image.push_back(images);
                }
            }
            for (int i = 0; i < image.size(); i++) {
                uint y = image[i].coords.y;
                uint x = image[i].coords.x;

                frame[y][x] = image[i].color;                
            }
        }
        else //Without Multithreading
        {
            float3 x_dir = camera->screen_p1 - camera->screen_p0;
	        float3 y_dir = camera->screen_p2 - camera->screen_p0;

            for (int y = SCRHEIGHT - 1; y >= 0; --y) {
                for (int x = 0; x < SCRWIDTH; ++x) {
                    float3 pixel_color = float3(0, 0, 0);
                    // 	Point on the screen:
                    // 𝑃(𝑢, 𝑣) = 𝑃0 + 𝑢(𝑃1 −𝑃0) + 𝑣(𝑃2 −𝑃0)
                    // 𝑢, 𝑣 ∈[0, 1]

                    // Loop count with or without antialiasing.
                    int n = antialiasing == true ? samples_per_pixel : 1;

                    //ANTI ALIASING
                    for (int s = 0; s < n; ++s) {
                        // Antialiasing
                        float offset_x = antialiasing == true ? random_float() : 0.f;
                        float offset_y = antialiasing == true ? random_float() : 0.f;

                        float u = (x + offset_x) / (((float)SCRWIDTH) - 1.f);
                        float v = (y + offset_y) / (((float)SCRHEIGHT) - 1.f);

                        float3 screen_point = camera->screen_p0 + u * x_dir + v * y_dir;

                        // Ray direction: 𝑃(𝑢,𝑣) − 𝐸 (and then normalized)
                        float3 ray_dir = normalize(screen_point - camera->cameraPos);
                        Ray ray = Ray(camera->cameraPos, ray_dir);

                        pixel_color += Trace(ray);
                    }

                    // Final antialiasing division.
                    if (antialiasing == true) {
                        pixel_color /= samples_per_pixel;
                    }
                    frame[y][x] = pixel_color;
                }
            }
        }
    }


    // -----------------------------------------------------------
    // Initialize the application
    // -----------------------------------------------------------

    void Raytracer::CalculateColor(Blockjob job)
    {
        float3 x_dir = camera->screen_p1 - camera->screen_p0;
	    float3 y_dir = camera->screen_p2 - camera->screen_p0;

        for (int j = job.rowStart; j < job.rowEnd; ++j) //height, row height
        {
            for (int i = 0; i < job.colSize; ++i) //width, row length
            {
                float3 pixel_color = float3(0, 0, 0);
                for (int s = 0; s < job.samples_per_pixel; ++s)
                {
                    float offset_x = antialiasing == true ? random_float() : 0.f;
                    float offset_y = antialiasing == true ? random_float() : 0.f;
                    float u = (i + offset_x) / float(job.colSize);
                    float v = (j + offset_y) / float(SCRHEIGHT);
                    float3 screen_point = camera->screen_p0 + u * x_dir + v * y_dir;
                    // Ray direction: 𝑃(𝑢,𝑣) − 𝐸 (and then normalized)
                    float3 ray_dir = normalize(screen_point - camera->cameraPos);
                    Ray ray = Ray(camera->cameraPos, ray_dir);
                    pixel_color += Trace(ray);
                }

                // Final antialiasing division.
                if (antialiasing == true) {
                    pixel_color /= float(job.samples_per_pixel);
                }
                //pixel_color = float3(sqrt(pixel_color.x), sqrt(pixel_color.y), sqrt(pixel_color.z));
                const unsigned int index = j * job.colSize + i;
                job.indices.push_back(float2(i, j));
                job.colors.push_back(pixel_color);
            }
        }
        {
            lock_guard<std::mutex> lock(mute);
            imageBlocks.push_back(job);
            completedThreads++;
            cvResults.notify_one();
        }
    }
}
