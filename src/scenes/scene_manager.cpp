#include "precomp.h"

namespace RaytracingRenderer {
	Scene SceneManager::ReflectionRoom() {
		// Instantiate colored material pointers.
		shared_ptr<DiffuseMaterial> purple_mat = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0.2, 0.1, 1)));
		shared_ptr<DiffuseMaterial> pink_mat = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0.9f, 0.3f, 0.4f)));
		shared_ptr<DiffuseMaterial> lavender_mat = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0.4f, 0.4f, 0.8f)));
		shared_ptr<DiffuseMaterial> off_white_mat = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0.85f, 0.85f, 0.9f)));
		shared_ptr<DiffuseMaterial> black_mat = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0, 0, 0)));

		// Special materials
		shared_ptr<NormalMaterial> normal_mat = make_shared<NormalMaterial>(NormalMaterial());
		shared_ptr<CheckerboardMaterial> plane_mat = make_shared<CheckerboardMaterial>(CheckerboardMaterial(float3(0.4f, 0.2f, 1.f), float3(0.4f, 1.f, 0.7f)));
		shared_ptr<SpecularMaterial> mirror_mat = make_shared<SpecularMaterial>(SpecularMaterial());
		shared_ptr<GlassMaterial> glass_mat = make_shared<GlassMaterial>(GlassMaterial());

		// Instantiate object pointers.
		shared_ptr<Sphere> sphere1 = make_shared<Sphere>(Sphere(float3(3, -1, 5), 1.5f, glass_mat));
		shared_ptr<Sphere> sphere2 = make_shared<Sphere>(Sphere(float3(-3, -1, 5), 1.5f, mirror_mat));
		shared_ptr<Plane> floor = make_shared<Plane>(Plane(float3(0, -2.5, 0), float3(0, 1, 0), plane_mat));
		shared_ptr<Plane> left_wall = make_shared<Plane>(Plane(float3(-6, 0, 0), float3(1, 0, 0), pink_mat));
		shared_ptr<Plane> back_wall = make_shared<Plane>(Plane(float3(0, 0, 8), float3(0, 0, -1), off_white_mat));
		shared_ptr<Plane> fourth_wall = make_shared<Plane>(Plane(float3(0, 0, -4), float3(0, 0, 1), black_mat));

		shared_ptr<Plane> right_wall = make_shared<Plane>(Plane(float3(6, 0, 0), float3(-1, 0, 0), lavender_mat));
		shared_ptr<Plane> ceiling = make_shared<Plane>(Plane(float3(0, 6, 0), float3(0, -1, 0), off_white_mat));

		// Put all scene objects in a list
		list<shared_ptr<Hittable>> objects = list<shared_ptr<Hittable>>({ left_wall, right_wall, back_wall, fourth_wall, ceiling, sphere1, sphere2, floor });

		shared_ptr<PointLight> point_light = make_shared<PointLight>(PointLight(float3(0, 5.5f, 3), 20.f));
		shared_ptr<AmbientLight> ambient_light = make_shared<AmbientLight>(AmbientLight(0.5f));

		list<shared_ptr<Light>> lights = list<shared_ptr<Light>>({ point_light, ambient_light });

		Scene scene = Scene(objects, lights);
		return scene;
		//camera = Camera();
	}

	Scene SceneManager::GlassTest() {
		shared_ptr<DiffuseMaterial> purple_mat = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0.2, 0.1, 1)));
		shared_ptr<DiffuseMaterial> off_white_mat = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0.85f, 0.85f, 0.9f)));

		shared_ptr<CheckerboardMaterial> plane_mat = make_shared<CheckerboardMaterial>(CheckerboardMaterial(float3(1.f, 1.f, 0.f), float3(1.f, 0.8f, 0.f)));
		shared_ptr<GlassMaterial> glass_mat = make_shared<GlassMaterial>(GlassMaterial(1.52f, float3(0.1f, 0.7f, 0.7f)));

		shared_ptr<Sphere> sphere = make_shared<Sphere>(Sphere(float3(0, 0, 5), 2.f, glass_mat));
		shared_ptr<Plane> floor = make_shared<Plane>(Plane(float3(0, -2.5, 0), float3(0, 1, 0), plane_mat));

		//shared_ptr<PointLight> point_light = make_shared<PointLight>(PointLight(float3(0, 5.5f, 3), 20.f));
		shared_ptr<AmbientLight> ambient_light = make_shared<AmbientLight>(AmbientLight(1.f));

		list<shared_ptr<Light>> lights = list<shared_ptr<Light>>({ /*point_light,*/ ambient_light });
		list<shared_ptr<Hittable>> objects = list<shared_ptr<Hittable>>({ sphere, floor });

		Scene scene = Scene(objects, lights);
		return scene;
	}

	Scene SceneManager::BeersLaw() {
		shared_ptr<GlassMaterial> glass_mat = make_shared<GlassMaterial>(GlassMaterial(1.52f, float3(0.1f, 0.7f, 0.7f)));
		shared_ptr<CheckerboardMaterial> plane_mat = make_shared<CheckerboardMaterial>(CheckerboardMaterial(float3(1.f, 1.f, 0.f), float3(1.f, 0.8f, 0.f)));

		shared_ptr<Sphere> sphere1 = make_shared<Sphere>(Sphere(float3(-9.5, -2.5, 7), 0.5f, glass_mat));
		shared_ptr<Sphere> sphere2 = make_shared<Sphere>(Sphere(float3(-7, -2, 7), 1.f, glass_mat));
		shared_ptr<Sphere> sphere3 = make_shared<Sphere>(Sphere(float3(-3, -1, 7), 2.f, glass_mat));
		shared_ptr<Sphere> sphere4 = make_shared<Sphere>(Sphere(float3(4, 1, 7), 4.f, glass_mat));

		shared_ptr<DiffuseMaterial> off_white_mat = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0.95f, 0.9f, 0.f)));
		shared_ptr<Plane> floor = make_shared<Plane>(Plane(float3(0, -3, 0), float3(0, 1, 0), off_white_mat));

		list<shared_ptr<Hittable>> objects = list<shared_ptr<Hittable>>({ sphere1, sphere2, sphere3, sphere4, floor });

		shared_ptr<PointLight> point_light = make_shared<PointLight>(PointLight(float3(0, 10.f, 10.f), 14.f));
		shared_ptr<AmbientLight> ambient_light = make_shared<AmbientLight>(AmbientLight(0.6f));
		list<shared_ptr<Light>> lights = list<shared_ptr<Light>>({ point_light, ambient_light });

		Scene scene = Scene(objects, lights);

		return scene;
	}

	Scene SceneManager::DirectionalLightTest() {
		shared_ptr<DiffuseMaterial> purple_mat = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0.2, 0.1, 1)));
		shared_ptr<CheckerboardMaterial> plane_mat = make_shared<CheckerboardMaterial>(CheckerboardMaterial(float3(1.f, 1.f, 0.f), float3(1.f, 0.8f, 0.f)));

		shared_ptr<Sphere> sphere1 = make_shared<Sphere>(Sphere(float3(-9.5, -2.5, 7), 0.5f, purple_mat));
		shared_ptr<Sphere> sphere2 = make_shared<Sphere>(Sphere(float3(-7, -2, 7), 1.f, purple_mat));
		shared_ptr<Sphere> sphere3 = make_shared<Sphere>(Sphere(float3(-3, -1, 7), 2.f, purple_mat));
		shared_ptr<Sphere> sphere4 = make_shared<Sphere>(Sphere(float3(4, 1, 7), 4.f, purple_mat));

		shared_ptr<DiffuseMaterial> off_white_mat = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0.95f, 0.9f, 0.f)));
		shared_ptr<Plane> floor = make_shared<Plane>(Plane(float3(0, -3, 0), float3(0, 1, 0), off_white_mat));

		list<shared_ptr<Hittable>> objects = list<shared_ptr<Hittable>>({ sphere1, sphere2, sphere3, sphere4, floor });

		// Lighting with directional light.
		shared_ptr<DirectionalLight> directional_light = make_shared<DirectionalLight>(DirectionalLight(float3(0.2f, -0.7f, -0.1f), 0.8f));
		shared_ptr<AmbientLight> ambient_light = make_shared<AmbientLight>(AmbientLight(0.2f));
		list<shared_ptr<Light>> lights = list<shared_ptr<Light>>({ directional_light, ambient_light });

		Scene scene = Scene(objects, lights);
		return scene;

	}

	Scene SceneManager::Snowman() {
		shared_ptr<GlassMaterial> glass_mat = make_shared<GlassMaterial>(GlassMaterial(1.52f, float3(0.1f, 0.7f, 0.7f)));
		shared_ptr<DiffuseMaterial> off_white_mat = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0.85f, 0.85f, 0.9f)));
		shared_ptr<DiffuseMaterial> black_mat = make_shared<DiffuseMaterial>(DiffuseMaterial(float3(0.f, 0.f, 0.f)));
		shared_ptr<CheckerboardMaterial> check_mat = make_shared<CheckerboardMaterial>(CheckerboardMaterial(float3(1.f, 0.f, 0.f), float3(0.f, 1.f, 0.f)));
		shared_ptr<NormalMaterial> normal_mat = make_shared<NormalMaterial>(NormalMaterial());

		shared_ptr<Sphere> sphere1 = make_shared<Sphere>(Sphere(float3(-2.5, -2, 3), 1.f, off_white_mat));
		shared_ptr<Sphere> sphere2 = make_shared<Sphere>(Sphere(float3(-2.5, -1, 3), .7f, off_white_mat));
		shared_ptr<Sphere> sphere3 = make_shared<Sphere>(Sphere(float3(-2.5, -0.1, 3), .5f, off_white_mat));

		shared_ptr<Sphere> eye1 = make_shared<Sphere>(Sphere(float3(-2.2, -0.1, 2.5), .1f, black_mat));
		shared_ptr<Sphere> eye2 = make_shared<Sphere>(Sphere(float3(-2.7, -0.1, 2.5), .1f, black_mat));

		shared_ptr<Sphere> sphere4 = make_shared<Sphere>(Sphere(float3(-3, 0, 20), 5.f, check_mat));
		shared_ptr<Sphere> sphere5 = make_shared<Sphere>(Sphere(float3(-3, 5, 20), 3.5f, check_mat));
		shared_ptr<Sphere> sphere6 = make_shared<Sphere>(Sphere(float3(-3, 9, 20), 2.5f, check_mat));

		shared_ptr<Sphere> eye3 = make_shared<Sphere>(Sphere(float3(-2.6, 9, 17.5), .4f, black_mat));
		shared_ptr<Sphere> eye4 = make_shared<Sphere>(Sphere(float3(-3.4, 9, 17.5), .4f, black_mat));

		shared_ptr<Sphere> sphere7 = make_shared<Sphere>(Sphere(float3(1, -2, 13), 2.f, glass_mat));
		shared_ptr<Sphere> sphere8 = make_shared<Sphere>(Sphere(float3(1, 0, 13), 1.4f, glass_mat));
		shared_ptr<Sphere> sphere9 = make_shared<Sphere>(Sphere(float3(1, 1.8, 13), 1.f, glass_mat));

		shared_ptr<Sphere> eye5 = make_shared<Sphere>(Sphere(float3(1.4, 1.8, 12), .2f, black_mat));
		shared_ptr<Sphere> eye6 = make_shared<Sphere>(Sphere(float3(0.6, 1.8, 12), .2f, black_mat));

		shared_ptr<Sphere> sphere10 = make_shared<Sphere>(Sphere(float3(4, -2, 5), 1.f, normal_mat));
		shared_ptr<Sphere> sphere11 = make_shared<Sphere>(Sphere(float3(4, -1, 5), .7f, normal_mat));
		shared_ptr<Sphere> sphere12 = make_shared<Sphere>(Sphere(float3(4, -0.1, 5), .5f, normal_mat));

		shared_ptr<Sphere> eye7 = make_shared<Sphere>(Sphere(float3(4.2, -0.1, 4.5), .1f, black_mat));
		shared_ptr<Sphere> eye8 = make_shared<Sphere>(Sphere(float3(3.8, -0.1, 4.5), .1f, black_mat));

		shared_ptr<Plane> floor = make_shared<Plane>(Plane(float3(0, -3, 0), float3(0, 1, 0), off_white_mat));

		list<shared_ptr<Hittable>> objects = list<shared_ptr<Hittable>>({ sphere1, sphere2, sphere3, sphere3, sphere4, sphere5, sphere6, sphere7, sphere8, sphere9, sphere10, sphere11, sphere12, floor, eye1, eye2, eye3, eye4, eye5, eye6, eye7, eye8 });

		shared_ptr<DirectionalLight> directional_light = make_shared<DirectionalLight>(DirectionalLight(float3(0.2f, -0.7f, -0.1f), 0.8f));
		shared_ptr<PointLight> point_light = make_shared<PointLight>(PointLight(float3(0, 10.f, 10.f), 14.f));
		shared_ptr<AmbientLight> ambient_light = make_shared<AmbientLight>(AmbientLight(0.6f));
		list<shared_ptr<Light>> lights = list<shared_ptr<Light>>({ directional_light, ambient_light, point_light });

		Scene scene = Scene(objects, lights);

		return scene;
	}
}