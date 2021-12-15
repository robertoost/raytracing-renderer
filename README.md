# Raynetracing Renderobert

A reasonably simple whitted-style raytracer, created for assignment 1 of the Advanced Graphics course of Utrecht University.

Code is fully public domain. Use as you please.
Original template by dr. ing. J. Bikker (bikker.j@gmail.com).

## Features

- Sphere and Plane primitives.
- Ambient, Point, and Directional lights.
- Various materials, including diffuse, specular, and dielectric.
- Raytracing with shadow, reflection, and refraction rays.
- Transmission with Absorption using Beer's law.
- Free camera, with translation and rotation.
- Antialiasing.
- CPU Multi-threading.

## TO-DO

- Texturing of primitives using bitmaps.
- Post-processing effects.
- Triangle mesh using obj files.
- Fish-eye lens, barrel distortion.

## Instructions

### Installation

In order to run this app, open the solution in VS (tested in VS 2019) and build + run the project.

### Usage

In the scene view, press WASD to move around, and change your viewing direction by left clicking and dragging the mouse across the screen.

In order to switch scenes, just go to the Init function in myApp.cpp and change the scene loaded by the SceneManager.

In order to change the settings, go to myapp.h. There you will find several settings:

- `max_bounces` - The amount of times a ray can be reflected or refracted before recursion is terminated.\
    default: `10`.

- `max_reflection_shadows` - The amount of recursions where reflected/refracted rays can still cast shadow rays. Values above `max_bounces` have no effect.\
    default: `4`.

- `antialiasing` - Determines whether antialiasing is used to render the scene.\
    default: `false`.

- `samples_per_pixel` - The amount of rays that are traced for each pixel if antialiasing is on.\
    default: `10`.
  
- `multithreading` - Determines whether to use CPU multithreading to speed up the renderer.\
    default: `true`.

## References

- Shirley, P. (2020). Raytracing in One Weekend.
https://raytracing.github.io/books/RayTracingInOneWeekend.html

- Prunier, J.C. et al. (2016). Scratchapixel 2.0 Volume 1: Foundations of 3D Rendering.
https://www.scratchapixel.com

- Adaixo, M. (2019). Paralellizing Ray Tracing.
https://mikeadev.net/2019/11/parallelizing-ray-tracing/

- Li, V. (2018). Raytracing - Ray Sphere Intersection.
https://viclw17.github.io/2018/07/16/raytracing-ray-sphere-intersection/

- Bikker, J. (2021). Tmpl8.
https://github.com/jbikker/advgrtmpl8