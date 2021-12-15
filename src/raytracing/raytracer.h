#pragma once
namespace RaytracingRenderer {

    class Raytracer
    {
    public:
        struct Blockjob
        {
            int rowStart;
            int rowEnd;
            int colSize;
            int samples_per_pixel;

            vector<float2> indices;
            vector<float3> colors;
        };

        Scene* scene;
        Camera* camera;

        const int nThreads = thread::hardware_concurrency();
        int rowsPerThread = SCRHEIGHT / nThreads;
        int leftOver = SCRHEIGHT % nThreads;

        mutex mute;
        condition_variable cvResults;
        vector<Blockjob> imageBlocks = vector<Blockjob>();
        atomic<int> completedThreads = { 0 };
        vector<thread> threads;

        uint max_bounces = 10;
        uint max_reflection_shadows = 4;
        
        bool antialiasing = false;
        uint samples_per_pixel = 10;
        
        bool multithreading = true;
        //EDIT THIS TO CHANGE ANTI-ALIASING STRENGTH. 100 is beautiful but slow. 0 is none. 

        inline Raytracer(Scene& scene, Camera& camera) {
            // TODO: Move camera to scene.
            this->scene = &scene;
            this->camera = &camera;
        }
        inline Raytracer() {
            this->scene = &Scene();
            this->camera = &Camera();
        }

        //Raytracer(const Raytracer&) = default;
        //Raytracer& operator=(const Raytracer&) = default;

        void CalculateColor(Blockjob job);
        float3 Trace(Ray &ray);
        float3 TraceSolid(Ray& ray, hit_record& rec, uint bounce_count);
        float3 TraceGlass(Ray& ray, hit_record& rec, uint bounce_count);
        float3 DirectIllumination(float3 &position, float3 &normal);
        float3 TraceReflection(Ray& ray, uint bounce_count);
        float3 TraceRefraction(Ray& ray, uint bounce_count);
        void RenderScene(float3 frame[SCRHEIGHT][SCRWIDTH]);
    };
}

