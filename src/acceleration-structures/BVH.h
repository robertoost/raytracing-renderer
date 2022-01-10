#pragma once
#include <queue>

namespace RaytracingRenderer {

	class BVH : public AccelerationStructure
	{
		static const int kNumPlaneSetNormals = 7;

		struct Extents {
			Extents() {
				for (int i = 0; i < kNumPlaneSetNormals; i++) {
					d[i][0] = INFINITY, d[i][1] = -INFINITY;
				}
			}

			void extendBy(const Extents& extents)
			{
				for (int i = 0; i < kNumPlaneSetNormals; ++i) {
					if (extents.d[i][0] < d[i][0]) d[i][0] = extents.d[i][0];
					if (extents.d[i][1] > d[i][1]) d[i][1] = extents.d[i][1];
				}
			}

			bool intersect(const Ray& ray, float* precomputedNumerator, float* precomputeDenominator, float& tNear, float& tFar, int& planeIndex) {
				for (int i = 0; i < kNumPlaneSetNormals; ++i) {
					float tn = (d[i][0] - precomputedNumerator[i]) / precomputeDenominator[i];
					float tf = (d[i][1] - precomputedNumerator[i]) / precomputeDenominator[i];
					if (precomputeDenominator[i] < 0) swap(tn, tf);
					if (tn > tNear) tNear = tn, planeIndex = i;
					if (tf < tFar) tFar = tf;
					if (tNear > tFar) 
						return false;
				}

				return true;
			}

			float d[kNumPlaneSetNormals][2];
			shared_ptr<Hittable> object;
		};

		struct OctreeNode {
			shared_ptr<OctreeNode> child[8] = { nullptr };
			vector<shared_ptr<Extents>> data;
			Extents extents;
			bool isLeaf;
			int depth; //for debugging
			OctreeNode() : isLeaf(true) { memset(child, 0x0, sizeof(OctreeNode*) * 8); }
			//~OctreeNode() { for (int i = 0; i < 8; ++i) if (child[i] != NULL) delete child[i]; }
		};

		struct Octree {
			Octree(const Extents& extents) : root(NULL) {
				float xdiff = extents.d[0][1] - extents.d[0][0];
				float ydiff = extents.d[1][1] - extents.d[1][0];
				float zdiff = extents.d[2][1] - extents.d[2][0];
				float dim = std::max(xdiff, std::max(ydiff, zdiff));
				float3 centroid(
					(extents.d[0][0] + extents.d[0][1]),
					(extents.d[1][0] + extents.d[1][1]),
					(extents.d[2][0] + extents.d[2][1]));
				bounds[0] = (float3(centroid) - float3(dim, dim, dim)) * 0.5f;
				bounds[1] = (float3(centroid) + float3(dim, dim, dim)) * 0.5f;
				root = make_shared<OctreeNode>(OctreeNode());
			}
			void insert(shared_ptr<Extents> extents)
			{
				insert(root, extents, bounds[0], bounds[1], 0);
			}
			void build()
			{
				build(root, bounds[0], bounds[1]);
			}

			struct QueueElement
			{
				shared_ptr<OctreeNode> node; // octree node held by this node in the tree 
				float t; // used as key 
				QueueElement(const shared_ptr<OctreeNode> &n, float thit) : node(n), t(thit) {}
				QueueElement(const QueueElement& qe) { node = qe.node; t = qe.t; }
				// comparator is > instead of < so priority_queue behaves like a min-heap
				friend bool operator < (const QueueElement& a, const QueueElement& b) { return a.t > b.t; }
				//QueueElement& operator=(const QueueElement& qe) {  return QueueElement(qe.node, qe.t); }
			};
			float3 bounds[2];
			shared_ptr<OctreeNode> root = nullptr;

			~Octree() { deleteOctreeNode(root); }
		private:

			void deleteOctreeNode(shared_ptr<OctreeNode> node)
			{
				for (uint8_t i = 0; i < 8; i++) {
					if (node->child[i] != nullptr) {
						deleteOctreeNode(node->child[i]);
					}
				}
				node.reset();
			}

			void insert(
				const shared_ptr<OctreeNode> node, const shared_ptr<Extents> extents,
				const float3 boundMin, float3 boundMax, int depth)
			{
				if (node->isLeaf) {
					if (node->data.size() == 0 || depth == 16) {
						node->data.push_back(extents);
					}
					else {
						node->isLeaf = false;
						while (node->data.size()) {
							insert(node, node->data.back(), boundMin, boundMax, depth);
							node->data.pop_back();
						}
						insert(node, extents, boundMin, boundMax, depth);
					}
				}
				else {
					// insert bounding volume in the right octree cell
					float3 extentsCentroid = (
						float3(extents->d[0][0], extents->d[1][0], extents->d[2][0]) +
						float3(extents->d[0][1], extents->d[1][1], extents->d[2][1])) * 0.5;
					float3 nodeCentroid = (boundMax + boundMin) * 0.5f;
					int childIndex = 0;
					if (extentsCentroid.x > nodeCentroid.x) childIndex += 4;
					if (extentsCentroid.y > nodeCentroid.y) childIndex += 2;
					if (extentsCentroid.z > nodeCentroid.z) childIndex += 1;
					float3 childBoundMin, childBoundMax;
					float3 boundCentroid = (boundMin + boundMax) * 0.5;
					computeChildBound(childIndex, boundCentroid, boundMin, boundMax, childBoundMin, childBoundMax);
					if (node->child[childIndex] == NULL)
						node->child[childIndex] = make_shared<OctreeNode>(OctreeNode()), node->child[childIndex]->depth = depth;
					insert(node->child[childIndex], extents, childBoundMin, childBoundMax, depth + 1);
				}
			}
			void computeChildBound(
				const int& i, const float3& boundCentroid,
				const float3& boundMin, const float3& boundMax,
				float3& pMin, float3& pMax) const
			{
				pMin.x = (i & 4) ? boundCentroid.x : boundMin.x;
				pMax.x = (i & 4) ? boundMax.x : boundCentroid.x;
				pMin.y = (i & 2) ? boundCentroid.y : boundMin.y;
				pMax.y = (i & 2) ? boundMax.y : boundCentroid.y;
				pMin.z = (i & 1) ? boundCentroid.z : boundMin.z;
				pMax.z = (i & 1) ? boundMax.z : boundCentroid.z;
			}
			// bottom-up construction
			void build(shared_ptr<OctreeNode> node, const float3& boundMin, const float3& boundMax)
			{
				if (node->isLeaf) {
					// compute leaf node bounding volume
					for (shared_ptr<Extents> e : (node->data)) {
						node->extents.extendBy(*e);
					}
				}
				else {
					for (int i = 0; i < 8; ++i)
						if (node->child[i]) {
							float3 childBoundMin, childBoundMax;
							float3 boundCentroid = (boundMin + boundMax) * 0.5;
							computeChildBound(i, boundCentroid, boundMin, boundMax, childBoundMin, childBoundMax);
							build(node->child[i], childBoundMin, childBoundMax);
							node->extents.extendBy(node->child[i]->extents);
						}
				}
			}
		};


	public:

		vector<Extents> extents;
		shared_ptr<Octree> octree;
		shared_ptr<int> passes = make_shared<int>(int(0));

		BVH(){}
		BVH(list<shared_ptr<Hittable>> objects) : extents(NULL) {
			Extents sceneExtents;
			extents.resize(objects.size());
			//cout << objects.size();
			int i = 0;
			for (shared_ptr<Hittable> obj : objects) {
				int j = 0;
				while (j < kNumPlaneSetNormals) {
					//cout << j << " ";
					obj->computeBounds(planeSetNormals[j], extents[i].d[j][0], extents[i].d[j][1]);
					j++;
				}
				sceneExtents.extendBy(extents[i]);
				extents[i].object = obj;
				i++;
			}
			//create hierarchy
			octree = make_shared<Octree>(Octree(sceneExtents));
			for (int i = 0; i < objects.size(); i++) {
				octree->insert(make_shared<Extents>(extents[i]));
			}
			octree->build();

		}
		~BVH() { octree.reset(); }

		BVH& operator=(const BVH& ac) {
			this->extents = ac.extents;
			this->octree = ac.octree;
			return *this;
		}

		const float3 planeSetNormals[kNumPlaneSetNormals] = {
			float3(1, 0, 0),
			float3(0, 1, 0),
			float3(0, 0, 1),
			float3(sqrtf(3) / 3.f,  sqrtf(3) / 3.f, sqrtf(3) / 3.f),
			float3(-sqrtf(3) / 3.f,  sqrtf(3) / 3.f, sqrtf(3) / 3.f),
			float3(-sqrtf(3) / 3.f, -sqrtf(3) / 3.f, sqrtf(3) / 3.f),
			float3(sqrtf(3) / 3.f, -sqrtf(3) / 3.f, sqrtf(3) / 3.f) 
		};

		//const shared_ptr<Hittable> intersect(const Ray& ray, float t_min, float t_max, hit_record& rec) const
		const bool intersect(const Ray& ray, float t_min, float t_max, hit_record& rec, const list<shared_ptr<Hittable>>& objects) const
		{
			//float tHit = t_max;
			//shared_ptr<Hittable> hitObject = NULL;
			//bool collision = false;
			//float precomputedNumerator[kNumPlaneSetNormals], precomputeDenominator[kNumPlaneSetNormals];
			//for (int i = 0; i < kNumPlaneSetNormals; ++i) {
			//	precomputedNumerator[i] = dot(planeSetNormals[i], ray.orig);
			//	precomputeDenominator[i] = dot(planeSetNormals[i], ray.dir);;
			//}
			float tHit = INFINITY;
			float precomputedNumerator[kNumPlaneSetNormals];
			float precomputedDenominator[kNumPlaneSetNormals];
			for (int i = 0; i < kNumPlaneSetNormals; ++i) {
				precomputedNumerator[i] = dot(planeSetNormals[i], ray.orig);
				precomputedDenominator[i] = dot(planeSetNormals[i], ray.dir);
			}

			bool collision = false;
			int planeIndex;
			float tNear = 0, tFar = INFINITY; // tNear, tFar for the intersected extents 
			if (!octree->root->extents.intersect(ray, precomputedNumerator, precomputedDenominator, tNear, tFar, planeIndex) || tFar < 0)
				return false;
			tHit = tFar;
			priority_queue<Octree::QueueElement> queue;
			queue.push(Octree::QueueElement(octree->root, 0));
			while (!queue.empty() && queue.top().t < tHit) {
				const shared_ptr<OctreeNode> node = queue.top().node;
				queue.pop();
				if (node->isLeaf) {
					for (shared_ptr<Extents> e : (node->data)) { 
						float t = INFINITY;
						bool buffer = e->object->intersect(ray, t_min, t_max, rec);
						if (buffer && rec.t < tHit) {
							t = rec.t;
							tHit = t;
							collision = true;
						}
					}
				}
				else {
					for (int i = 0; i < 8; ++i) {
						if (node->child[i] != nullptr) {
							float tNearChild = 0, tFarChild = tFar;
							if (node->child[i]->extents.intersect(ray, precomputedNumerator, precomputedDenominator, tNearChild, tFarChild, planeIndex)) {
								float t = (tNearChild < 0 && tFarChild >= 0) ? tFarChild : tNearChild;
								queue.push(Octree::QueueElement(node->child[i], t));
							}
						}
					}
				}
			}
			return collision;

		}

		bool intersect(const Ray& ray, float t_min, float t_max, hit_record& rec, list<shared_ptr<Hittable>>& objects) {
			bool collision = false;
			for (shared_ptr<Hittable> obj : objects) {
				if (obj->bounding_box.intersectBounds(ray, t_min, t_max)) {
					if (obj->intersect(ray, t_min, t_max, rec))
						collision = true;
				}
			}
			return collision;
		}
	};
}