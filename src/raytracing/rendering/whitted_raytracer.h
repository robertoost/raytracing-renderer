#pragma once
namespace RaytracingRenderer {

    class WhittedRaytracer: public Renderer
    {
    public:
        struct Blockjob
        {
            int rowStart = 0;
            int rowEnd = 0;
            int colSize = 0;
            int samples_per_pixel = 0;

            vector<float2> indices;
            vector<float3> colors;
        };

        const int nThreads = thread::hardware_concurrency();
        int rowsPerThread = SCRHEIGHT / nThreads;
        int leftOver = SCRHEIGHT % nThreads;

        mutex mute;
        condition_variable cvResults;
        atomic<int> completedThreads = { 0 };

        vector<Blockjob> imageBlocks = vector<Blockjob>();
        vector<thread> threads;

        uint max_bounces = MAX_RECURSION_DEPTH;
        uint max_reflection_shadows = MAX_SHADOW_DEPTH;
        bool antialiasing = ANTIALIASING;
        uint samples_per_pixel = AA_SAMPLES_PER_PIXEL;
        bool multithreading = MULTITHREADING;

        inline WhittedRaytracer(Scene& scene, Camera& camera): Renderer(scene, camera) {}
        inline WhittedRaytracer(): Renderer() {}

        void CalculateColor(Blockjob job);
        float3 Trace(Ray &ray);
        float3 TraceSolid(Ray& ray, hit_record& rec, uint bounce_count);
        float3 TraceGlass(Ray& ray, hit_record& rec, uint bounce_count);
        float3 DirectIllumination(float3 &position, float3 &normal);
        float3 TraceReflection(Ray& ray, uint bounce_count);
        float3 TraceRefraction(Ray& ray, uint bounce_count);
        void RenderScene(float3 frame[SCRHEIGHT][SCRWIDTH]) override;
    };
}

