#pragma once

namespace RaytracingRenderer {
    class Vertex : public Object3D 
    {
    public:
        Vertex() {}

        Vertex(float3 position, shared_ptr<Material> material) : Object3D(position, material) {}

        Vertex(float3 position) : Object3D(position, make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0.2, 0.1, 1)))) {}
    };
    struct FaceDefinition {
        int vertexIndexes[3];
        int textureIndexes[3];
        int normalIndexes[3];
        float3 verts[3];
        float3 normals[3];
    };
	// TODO: Introduce a default transform class that handles position, rotation, movement, etc.
	class Triangle : public Hittable
	{
	public:
        //v1, v2, v3 must be initialized in counter-clockwise order (v0 is bottom left, v1 is bottom right, v2 is top; this can be rotated but not flipped)
        Vertex v0, v1, v2;
        float3 normal;

        inline Triangle(FaceDefinition face) {
            this->v0.position = face.verts[0];
            this->v1.position = face.verts[1];
            this->v2.position = face.verts[2];
            //this->position = this->v0;
            this->normal = cross(this->v1.position - this->v0.position, this->v2.position - this->v0.position);
            this->v0.material = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0.2, 0.1, 1)));
            updateAABB(this->bounding_box);
            
        }

        Triangle(){}

        void updateAABB(AABB box) const override {
            box.addPoint(this->v0.position);
            box.addPoint(this->v1.position);
            box.addPoint(this->v2.position);
        }

        inline Triangle(float3 position, float3 v0, float3 v1, float3 v2, shared_ptr<Material> material) {
            this->v0.position = position + v0;
            this->v1.position = position + v1;
            this->v2.position = position + v2;
            this->v0.material = material;
            this->v1.material = material;
            this->v2.material = material;
            this->normal = cross(v1 - v0, v2 - v0);
            updateAABB(this->bounding_box);
        }

        inline Triangle(float3 v0, float3 v1, float3 v2) {
            this->v0.position = v0;
            this->v1.position = v1;
            this->v2.position = v2;
            this->normal = cross(v1 - v0, v2 - v0);
            this->v0.material = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0.2, 0.1, 1)));
            updateAABB(this->bounding_box);
        }

        void computeBounds(const float3& planeNormal, float& dnear, float& dfar) const override {
            float d;
            vector<float3> p = { v0.position, v1.position, v2.position };
            for (uint32_t i = 0; i < p.size(); ++i) {
                d = dot(planeNormal, p[i]);
                if (d < dnear) dnear = d;
                if (d > dfar) dfar = d;
            }
        }

		bool intersect(const Ray& ray, float t_min, float t_max, hit_record& rec) const override {
            
            float3 edge1, edge2, h, s, q;
            float a, f, u, v;
            edge1 = v1.position - v0.position;
            edge2 = v2.position - v0.position;
            h = cross(ray.dir, edge2);
            a = dot(edge1, h);
            if (a > -t_min && a < t_max)
                return false;    // This ray is parallel to this triangle.
            f = 1.0 / a;
            s = ray.orig - v0.position;
            u = f * dot(s, h);
            if (u < 0.0 || u > 1.0)
                return false;
            q = cross(s, edge1);
            v = f * dot(ray.dir, q);
            if (v < 0.0 || u + v > 1.0)
                return false;
            // At this stage we can compute t to find out where the intersection point is on the line.
            float t = f * dot(edge2, q);
            if (t > t_min) // ray intersection
            {
                rec.p = ray.at(t);
                rec.t = t;
                rec.normal = normal;
                rec.mat_ptr = v0.material;
                return true;
            }
            else // This means that there is a line intersection but not a ray intersection.
                return false;
		}

        void set_vertex(Triangle t, int vertex, float3 val) {
            if (vertex == 0)
                t.v0.position = val;
            if (vertex == 1)
                t.v1.position = val;
            if (vertex == 2)
                t.v2.position = val;
        }

        float3 get_vertex(Triangle t, int vertex) {
            if (vertex == 0)
                return t.v0.position;
            if (vertex == 1)
                return t.v1.position;
            if (vertex == 2)
                return t.v2.position;
        }

       
	};
}