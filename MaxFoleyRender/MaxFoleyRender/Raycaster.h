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
#include "TriTree.h"
#include "Vector.h"
#include <thread>

namespace MFR {

	struct RaycasterOptions {
	public:
		int width;
		int height;
		int numRays;
		int numScatteringEvents;
	};
	
	class Raycaster {
		
	public:
		static void RenderImage(std::shared_ptr<Scene> scene, Color * pixels, RaycasterOptions options);
		static Color GetPixelColor(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene, int x, int y, int width, int height, int indirectRays);
		static std::shared_ptr<Surfel> CastSingleRay(std::shared_ptr<Scene> scene, Ray ray);
		static bool IntersectTriangle(Ray ray, Tri t, float b[3], float& dist);
		
		static Color GetPixelColor(std::shared_ptr<Camera> camera, std::shared_ptr<Scene>  scene, int x, int y, int width, int height, bool fixedPrimitives, int indirectRays);
		static std::shared_ptr<Surfel> CastSingleRay(std::shared_ptr<Scene> scene, Ray ray, bool fixedPrimitives);
		static void GenerateRays(int begin, int end, Ray * rayBuffer, std::shared_ptr<Camera> camera, RaycasterOptions options);
		static void IntersectRays(int begin, int end, Ray * rayBuffer, std::shared_ptr<Surfel> * surfelBuffer, TriTree* tt);
		static void GetLightInfo(int begin, int end, std::vector<std::shared_ptr<Light>> lights, std::shared_ptr<Surfel> * surfelBuffer, Color * biradianceBuffer, Ray* shadowRayBuffer);
		static void ShadowTests(int begin, int end, Ray * shadowRayBuffer, std::shared_ptr<Surfel> * surfelBuffer, bool* lightShadowedBuffer, TriTree* tt);
		static void ShadePixels(int begin, int end, std::shared_ptr<Surfel> * surfelBuffer, Ray * shadowRayBuffer, Color * colorBuffer, Color * biradianceBuffer, Color * modulationBuffer, bool * lightShadowedBuffer);
		static void ScatterRays(int begin, int end, Ray * rayBuffer, std::shared_ptr<Surfel> * surfelBuffer, Color * modulationBuffer, RaycasterOptions options);
		static void AddEmissiveTerms(int begin, int end, Ray * rayBuffer, Color * colorBuffer, std::shared_ptr<Surfel> * surfelBuffer, Color * modulationBuffer);

		template <class _Fp, class ..._Args>
			static void multithread(_Fp&& __f, int num, _Args&&... __args);

		template <class _Fp, class ..._Args>
			static void runIndexes(_Fp&& __f, int begin, int end, _Args&&... __args);

		static void KillThreads(std::thread * t, int numThreads);
		
	};
	

}
#endif /* Raycaster_h */
