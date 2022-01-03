#pragma once

namespace RaytracingRenderer {
    struct Vertex {
        float3 position;
        float2 texture;
        float3 normal;
    };
    struct FaceDefinition {
        int vertexIndexes[3];
        int textureIndexes[3];
        int normalIndexes[3];
        float3 verts[3];
        float3 normals[3];
    };
	// TODO: Introduce a default transform class that handles position, rotation, movement, etc.
	class Triangle : public Object3D, public Hittable
	{
	public:
        //v1, v2, v3 must be initialized in counter-clockwise order (v0 is bottom left, v1 is bottom right, v2 is top; this can be rotated but not flipped)
		float3 v0, v1, v2, normal;

        inline Triangle() : Object3D(), v0(float3(-2, 0, 5)), v1(float3(1, 0, 5)), v2(float3(0, 3, 5)) {
            this->normal = cross(v1 - v0, v2 - v0);
            this->material = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0.2, 0.1, 1)));
        }

        inline Triangle(Vertex v0, Vertex v1, Vertex v2) : Object3D() {
            this->v0 = position + v0.position;
            this->v1 = position + v1.position;
            this->v2 = position + v2.position;
            this->normal = (v1.normal + v0.normal + v2.normal) / length(v1.normal + v0.normal + v2.normal);
            this->material = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0.2, 0.1, 1)));
        }

        inline Triangle(float3 v0, float3 v1, float3 v2) : Object3D(), normal(normal) {
            this->v0 = position + v0;
            this->v1 = position + v1;
            this->v2 = position + v2;
            this->normal = cross(v1 - v0, v2 - v0);
            this->material = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0.f, 0.f, 0.f)));
        }

        inline Triangle(FaceDefinition face) : Object3D(), normal(normal) {
            this->v0 = face.verts[0];
            this->v1 = face.verts[1];
            this->v2 = face.verts[2];
            //this->position = this->v0;
            this->normal = (face.normals[0] + face.normals[1] + face.normals[2]) / length(face.normals[0] + face.normals[1] + face.normals[2]);
            this->material = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0.2, 0.1, 1)));
        }

        inline Triangle(float3 v0, float3 v1, float3 v2, float3 normal) : Object3D() {
            this->v0 = position + v0;
            this->v1 = position + v1;
            this->v2 = position + v2;
            this->normal = normal;
            this->material = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0.2, 0.1, 1)));
        }

        inline Triangle(float3 position, float3 v0, float3 v1, float3 v2, shared_ptr<Material> material) : Object3D(position, material), normal(normal) {
            this->v0 = position + v0;
            this->v1 = position + v1;
            this->v2 = position + v2;
            this->normal = cross(v1 - v0, v2 - v0);
        }

		bool intersect(const Ray& ray, float t_min, float t_max, hit_record& rec) const override {
			// Assuming vectors are all normalized
            float d = dot(normal, v0);

            bool singleSided = false; //back-face culling

            float t = - (dot(normal, ray.orig) + d) / dot(normal, ray.dir);

            float denom = dot(normal, ray.dir);
            float3 diff = v0 - ray.orig;
            float t2 = dot(diff, normal) / denom;

            float3 p = ray.orig + t * ray.dir;

            float area = length(normal);

            //check if ray + triangle are parallel

            if (abs(denom) > 0.0001f == false) {
                return false;
            }

            // Intersection found. Determine if object is occluded
            if ((t > t_min && t < t_max) == false) {
                // Object is occluded or too far away.
                return false;
            }

            // implementing the single/double sided feature
            if (dot(ray.dir, normal) < 0 && singleSided)
                return false; // back-facing surface 

            float3 e0 = v1 - v0;
            float3 vp0 = p - v0;
            float3 C = cross(e0, vp0);
            if (dot(normal, C) < 0) return false; //P is on the right side

            float3 e1 = v2 - v1;
            float3 vp1 = p - v1;
            C = cross(e1, vp1);
            if (dot(normal, C) < 0) return false; //P is on the right side

            float3 e2 = v0 - v2;
            float3 vp2 = p - v2;
            C = cross(e2, vp2);
            if (dot(normal, C) < 0) return false; //P is on the right side

            // Record the hit.
            rec.t = t2;
            rec.p = ray.at(t2);
            rec.normal = normal;
            rec.mat_ptr = material;

            return true;
		}

        void set_vertex(Triangle t, int vertex, float3 val) {
            if (vertex == 0)
                t.v0 = val;
            if (vertex == 1)
                t.v1 = val;
            if (vertex == 2)
                t.v2 = val;
        }

        float3 get_vertex(Triangle t, int vertex) {
            if (vertex == 0)
                return t.v0;
            if (vertex == 1)
                return t.v1;
            if (vertex == 2)
                return t.v2;
        }
	};
}