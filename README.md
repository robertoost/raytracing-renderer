# Raynetracing Renderobert

A reasonably simple whitted-style raytracer, created for assignment 1 of the Advanced Graphics course of Utrecht University.

Code is fully public domain. Use as you please.
Original template by dr. ing. J. Bikker (bikker.j@gmail.com).

Created by Rayne Blair (raynebblair@gmail.com) & Robert Oost (mail@robertoo.st)

![image](https://user-images.githubusercontent.com/33265853/146192846-d1962713-a60e-4188-bb84-c537b9403910.png)

## Features

- Sphere and Plane primitives.
- Ambient, Point, and Directional lights (Whitted only).
- Various materials, including diffuse, specular, and dielectric.
- Whitted-style Raytracing with shadow, reflection, and refraction rays.
- Transmission with Absorption using Beer's law.
- Free camera, with translation and rotation.
- Antialiasing (Whitted only).
- CPU Multi-threading (Whitted only).
- Post-processing effects. (Vignette, Gamma correction, Chromatic Aberration).
- Supports partially diffuse/specular materials.

### New (!)

- Total energy measurement for the frame.
- Path Tracing using random hemisphere sampling and an accumulator.
- Path Tracing with specular/diffuse reflection and refraction rays.

### TO-DO

- Texturing of primitives using bitmaps.
- Triangle mesh using obj files.
- Fish-eye lens, barrel distortion.

## Instructions

### Installation

In order to run this app, open the solution in VS (tested in VS 2019) and build + run the project.

### Usage

In the scene view, press WASD to move around, and change your viewing direction by left clicking and dragging the mouse across the screen.

In order to switch scenes, just go to the Init function in `src/rr_app.cpp` and change the scene loaded by the SceneManager.

To switch renderers, go to the same Init function in `src/rr_app.cpp`. Then change the `Renderer` class that's initialized there. Options include `WhittedRaytracer`, `HemisphereSampler` (Direct light only), and `PathTracer`. Note that Whitted raytracing requires ambient/point/directional lights to be present in the scene, while the direct light sampler and path tracer only use area lights.

### Rendering Parameters

In order to change the rendering parameters, go to `template/common.h`. There you will find several settings:

- `MAX_RECURSION_DEPTH` - The amount of times a ray can be reflected or refracted before recursion is terminated.\
    default: `10`.

- `MAX_SHADOW_DEPTH` - Whitted only. The amount of recursions where reflected/refracted rays can still cast shadow rays. Values above `MAX_RECURSION_DEPTH` have no effect.\
    default: `4`.

- `ANTIALIASING` - Whitted only. Enables antialiasing for rendering the scene with an amount of samples `AA_SAMPLES_PER_PIXEL`.\
    default: `false`.

- `AA_SAMPLES_PER_PIXEL` - Whitted only. The amount of rays that are traced for each pixel if antialiasing is on.\
    default: `10`.
  
- `MULTITHREADING` - Whitted only. Enables CPU multithreading to speed up the renderer.\
    default: `true`.

- `POSTPROCESSING` - Enables post processing, depending on which settings are enabled.\
    default: `true`

- `GAMMA_CORRECTION` - Enables gamma correction.\
    default: `true`

- `GAMMA` - The gamma value used for gamma correction.\
    default: `0.7`

- `VIGNETTING` - Enables vignetting.\
    default: `true`

- `MAX_VIGNETTE` - Determines the amount of vignetting applied. Scales between 0.0 and 1.0\
    default: `0.7`

- `CHROMATIC_ABERRATION` - Enables chromatic aberration.\
    default: `true`

- `CHROM_ABB_R_OFFSET`, `CHROM_ABB_G_OFFSET`, `CHROM_ABB_B_OFFSET` - Determine the x,y offsets used for each color in chromatic aberration.\
    default: `(1,1), (-1,1), (-1,-1)`

## References

- Shirley, P. (2020). Raytracing in One Weekend.\
https://raytracing.github.io/books/RayTracingInOneWeekend.html

- Prunier, J.C. et al. (2016). Scratchapixel 2.0 Volume 1: Foundations of 3D Rendering.\
https://www.scratchapixel.com

- Adaixo, M. (2019). Paralellizing Ray Tracing.\
https://mikeadev.net/2019/11/parallelizing-ray-tracing/

- Li, V. (2018). Raytracing - Ray Sphere Intersection.\
https://viclw17.github.io/2018/07/16/raytracing-ray-sphere-intersection/

- De Vries, J. (2014). Learn OpenGL.\
  https://learnopengl.com/Getting-started/Camera

- Bikker, J. (2021). Tmpl8.\
https://github.com/jbikker/advgrtmpl8
