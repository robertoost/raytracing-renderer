
namespace RaytracingRenderer {

	class Renderer {
	public:
        Scene* scene;
        Camera* camera;

        inline Renderer(Scene& scene, Camera& camera) {
            // TODO: Move camera to scene.
            this->scene = &scene;
            this->camera = &camera;
        }
        inline Renderer() {
            this->scene = &Scene();
            this->camera = &Camera();
        }
        Renderer& operator=(const Renderer& rhs) {
            this->scene = rhs.scene;
            this->camera = rhs.camera;
            return *this;
        }

        virtual void RenderScene(float3 frame[SCRHEIGHT][SCRWIDTH]) = 0;
        virtual void OnCameraUpdate() {}
	};
}