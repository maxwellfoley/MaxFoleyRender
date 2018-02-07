//
//  Raycaster.h
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 1/28/18.
//  Copyright © 2018 Maxwell Foley. All rights reserved.
//

#ifndef Raycaster_h
#define Raycaster_h

#include <memory>
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Material.h"
#include "Ray.h"
#include "Scene.h"
#include "SceneObject.h"
#include "Surfel.h"
#include "Tri.h"
#include "Vector.h"

namespace MFR {
	class Raycaster {
		
	public:
		static void RenderImage(std::shared_ptr<Scene> scene, Color * pixels, int width, int height, int indirectRays);
		static Color GetPixelColor(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene, int x, int y, int width, int height, int indirectRays);
		static std::shared_ptr<Surfel> CastSingleRay(std::shared_ptr<Scene> scene, Ray ray);
		static bool IntersectTriangle(Ray ray, Tri t, float b[3], float& dist);
		
	};
}
#endif /* Raycaster_h */
