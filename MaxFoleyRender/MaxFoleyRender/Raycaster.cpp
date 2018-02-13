//
//  Raycaster.cpp
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 1/28/18.
//  Copyright © 2018 Maxwell Foley. All rights reserved.
//

#include <stdio.h>
#include "Raycaster.h"
#include <thread>

using namespace MFR;


void Raycaster::RenderImage(std::shared_ptr<Scene> scene, Color * colorBuffer, RaycasterOptions options)
{
	int width = options.width;
	int height = options.height;
	
	int numPix = width*height;
	Color * modulationBuffer = new Color[numPix];

	Ray * rayBuffer = new Ray[numPix];
	std::shared_ptr<Surfel> * surfelBuffer = new std::shared_ptr<Surfel>[numPix];
	Color * biradianceBuffer = new Color[numPix];
	Ray * shadowRayBuffer = new Ray[numPix];
	bool * lightShadowedBuffer = new bool[numPix];


	scene->poseMesh();
	std::vector<std::shared_ptr<Light>> lights = scene->lights;

	/*
		for(int j = 0; j < height; j++)
		{
			for(int i = 0; i < width; i++)
			{	
				Color color = GetPixelColor(scene->camera, scene, i, j, width, height, indirectRays);
				pixels[j*width +i] = color;
			}
		}
		*/
	for(int i = 0; i < options.numRays; i++)
	{
		//generate primary rays
		std::thread *t = new std::thread[numPix];
		for (int i = 0; i < numPix; ++i) {
			t[i] = std::thread(GenerateRay, i, rayBuffer, scene->camera, options);
		}
		KillThreads(t,numPix);
		delete [] t;
		
		//initialize the modulationBuffer to Color3(1 / paths per pixel)
		for (int i2 = 0; i2 < numPix; ++i2) {
			modulationBuffer[i2] = Color(1.0/options.numRays,1.0/options.numRays,1.0/options.numRays);
		}
	
		for(int j = 0; j < options.numScatteringEvents; j++)
		{
			//intersect all rays, storing the result in surfelbuffer
			std::thread *t2 = new std::thread[numPix];
			for (int i = 0; i < numPix; ++i) {
				t2[i] = std::thread(IntersectRay, i, rayBuffer, surfelBuffer);
			}
			KillThreads(t2,numPix);
			delete [] t2;
		
			//add emissive terms
			std::thread *t7 = new std::thread[numPix];
			for (int i = 0; i < numPix; ++i) {
				t7[i] = std::thread(AddEmissiveTerms, i, rayBuffer, colorBuffer, surfelBuffer, modulationBuffer);
			}
			KillThreads(t7,numPix);
			delete [] t7;
			
			if(lights.size() > 0)
			{
				//get biradiance and ray for shadow calculation
				std::thread *t3 = new std::thread[numPix];
				for (int i = 0; i < numPix; ++i) {
					t3[i] = std::thread(GetLightInfo, i, lights, surfelBuffer, biradianceBuffer, shadowRayBuffer);
				}
				KillThreads(t3,numPix);
				delete [] t3;
				
				//figure out whether or not the surfel is in shadow
				std::thread *t4 = new std::thread[numPix];
				for (int i = 0; i < numPix; ++i) {
					t4[i] = std::thread(ShadowTest, i, shadowRayBuffer, surfelBuffer, lightShadowedBuffer);
				}
				KillThreads(t4,numPix);
				delete [] t4;

				//color the pixel
				std::thread *t5 = new std::thread[numPix];
				for (int i = 0; i < numPix; ++i) {
					t5[i] = std::thread(ShadePixel, i, surfelBuffer, shadowRayBuffer, colorBuffer, biradianceBuffer, modulationBuffer, lightShadowedBuffer);
				}
				KillThreads(t5,numPix);
				delete [] t5;
			}
			
			if(j < options.numScatteringEvents-1)
			{
			//if not the last iteration, scatter the rays
			std::thread *t6 = new std::thread[numPix];
			for (int i = 0; i < numPix; ++i) {
				t6[i] = std::thread(ScatterRay, i, rayBuffer, surfelBuffer, modulationBuffer);

			}
			KillThreads(t6,numPix);
			delete [] t6;
		}

		}
		

	}
}

void Raycaster::GenerateRay(int i, Ray * rayBuffer, std::shared_ptr<Camera> camera, RaycasterOptions options)
{
	int x = i % options.width;
	int y = i / options.width;
	
	//calculate ray direction and origin based on camera position
  	const float side = -2.0f * tan(	camera->fov  / 2.0f);

	Point p = Point( camera->focalLength * (float(x) / float(options.width) - 0.5f) * side * float(options.width) / float(options.height), camera->focalLength * -(float(y) / float(options.height) - 0.5f) * side, camera->focalLength)  + camera->position;
	
	Vector dir = p-camera->position;
	dir = dir.unit();	
	Ray ray = Ray(p,dir);
	//TODO: rotate ray if camera is rotated

	rayBuffer[i] = ray;

}

void Raycaster::AddEmissiveTerms(int i, Ray * rayBuffer, Color * colorBuffer, std::shared_ptr<Surfel> * surfelBuffer, Color * modulationBuffer)
{
	if(surfelBuffer[i])
	{
						//debugPrintf("modulation %f %f %f",modulationBuffer[img->width()*p.y + p.x].r,modulationBuffer[img->width()*p.y + p.x].g,modulationBuffer[img->width()*p.y + p.x].b);
		Vector wo = -rayBuffer[i].direction;
		Color addTo = surfelBuffer[i]->material->ambient*modulationBuffer[i];

		colorBuffer[i] += addTo;
	}
}
void Raycaster::IntersectRay(int i, Ray * rayBuffer, std::shared_ptr<Surfel>  * surfelBuffer, std::vector<Tri> tt)
{

	Ray ray = rayBuffer[i];
	
	Point P = ray.origin;
	Vector w = ray.direction;
	
	//std::cout << ray << std::endl;

	//find the closest triangle in the scene
	bool foundHit = false;
	float * minB;
	float minT = 10000000;
	int minI = -1;

	//loop through triangles and check for hits
	for(int j = 0; i < tt.size() ; i++)
	{
		float b [3];
		float t = 10000000;
		//MY VERSION
		foundHit = IntersectTriangle(ray, tt[j],b,t);

	
		if(t < minT && t >= 0.0f) { minB = b; minT = t; minI = i;}
	}
	
	
	//if it hit something
	if(minI > -1)
	{
			float distance = minT;

			Vector normal = -((tt[minI].points[1] - tt[minI].points[0]).cross(tt[minI].points[2] - tt[minI].points[1])).unit();
			std::shared_ptr<Surfel> surfel = std::make_shared<Surfel>(ray.origin + distance*ray.direction,normal,tt[minI].material);
			surfelBuffer[i] = surfel;
	}
	else
	{
		surfelBuffer[i] = nullptr;
	}
}

void Raycaster::GetLightInfo(int i, std::vector<std::shared_ptr<Light>> lights, std::shared_ptr<Surfel> * surfelBuffer, Color * biradianceBuffer, Ray* shadowRayBuffer)
{
	//make sure surfel was found
	if(surfelBuffer[i] != NULL)
	{
		std::shared_ptr<Light> light;
		float weight;
		std::shared_ptr<Surfel> surfel = surfelBuffer[i];
		
		if(lights.size()> 1)
		{
			//calculate total biradiance
			float totalBiradiance = 0.0;
			for(int i = 0; i < lights.size(); i++)
			{
				Color b = lights[i]->biradiance(surfel->position);
				totalBiradiance += b.r + b.g + b.b;
			}
			
			float randomCounter = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/totalBiradiance);

			light = lights[0];
			
			for(int i = 0; randomCounter > 0; i++)
			{
				light = lights[i];
				Color b = light->biradiance(surfel->position);
				randomCounter -= b.r+b.g+b.b;
				weight = totalBiradiance/(b.r+b.g+b.b);
			}
			
		}
		//if theres just one light, just choose it
		else
		{
			light = lights[0];
			weight = 1.0;
		}
		
		Point surfelPos = surfel->position;
		Point subtraction = surfelPos - light->position;
		
		biradianceBuffer[i] = light->biradiance(surfelPos) *weight;
		
		Vector pos = light->position;
		Vector lightDir = pos - surfelPos;
		lightDir = lightDir.unit();
		shadowRayBuffer[i] = Ray(pos, -lightDir);
		
		
	}
}
void Raycaster::ShadowTest(int i, Ray * shadowRayBuffer, std::shared_ptr<Surfel> * surfelBuffer, bool* lightShadowedBuffer, std::vector<Tri> tt) {
		if(surfelBuffer[i])
		{
			//TODO: work out this shit
			float * b;
			float dist;
			//bool foundHit = IntersectTriangle(shadowRayBuffer[i], b,dist);
			
			for(int j = 0; j < tt.size(); j++)
			{
				bool foundHit = IntersectTriangle(shadowRayBuffer[i], tt[j],b,dist);
				if(foundHit && dist < (shadowRayBuffer[i].origin - surfelBuffer[i]->position).length() -.1)
				{
					lightShadowedBuffer[i] = true;
				}
				else
				{
					lightShadowedBuffer[i] = false;
				}

			}
		}
}

void Raycaster::ShadePixel(int i, std::shared_ptr<Surfel> * surfelBuffer, Ray * shadowRayBuffer, Color * colorBuffer, Color * biradianceBuffer, Color * modulationBuffer, bool * lightShadowedBuffer) {
	
		if(!lightShadowedBuffer[i] && surfelBuffer[i])
		{
			std::shared_ptr<Surfel> surfel = surfelBuffer[i];
			Vector lightDir = -shadowRayBuffer[i].direction;
			float multiplier = lightDir.dot(surfel->normal);

			//shade the pixel

			Color addTo = surfel->material->diffuse * multiplier * biradianceBuffer[i];
			if(addTo.r < 0.0){ addTo.r = 0.0;}
			if(addTo.g < 0.0){ addTo.g = 0.0;}
			if(addTo.b < 0.0){ addTo.b = 0.0;}

			colorBuffer[i] += addTo*modulationBuffer[i];
		}
		
		//make it gray if the ray missed the scene
		if(!surfelBuffer[i] )
		{
			colorBuffer[i] += Color(0.5f, 0.5f, 0.5f)*modulationBuffer[i];
		}
	
}
void Raycaster::ScatterRay(int i, Ray * rayBuffer, std::shared_ptr<Surfel> * surfelBuffer, Color * modulationBuffer, RaycasterOptions options)
{
		if(surfelBuffer[i])
		{
			std::shared_ptr<Surfel> surfel = surfelBuffer[i];

			Vector w_before = -1 * rayBuffer[i].direction;
			
			//TODO:: figure out how to do this`
			Vector w_after;
			Color weight;
			
			//TODO::implement thisscatter thing
			surfel->scatter(w_before, weight, w_after);
			modulationBuffer[i]*=weight;
			
			float dotSign = w_after.dot(surfel->normal);
			dotSign = dotSign / abs(dotSign);
			rayBuffer[i] = Ray(surfel->position +  surfel->normal * .1 * dotSign, w_after);
		}

}

Color Raycaster::GetPixelColor(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene, int x, int y, int width, int height, int indirectRays)
{			
	//calculate ray direction and origin based on camera position
  const float side = -2.0f * tan(	camera->fov  / 2.0f);

	Point p = Point( camera->focalLength * (float(x) / float(width) - 0.5f) * side * float(width) / float(height) ,
	                camera->focalLength * -(float(y) / float(height) - 0.5f) * side,
	                camera->focalLength)  + camera->position;
	
	Vector dir = p-camera->position;
	dir = dir.unit();	
	Ray ray = Ray(p,dir);
	//TODO: rotate ray if camera is rotated
	
	std::shared_ptr<Surfel> surfel = CastSingleRay(scene, ray);
	
	//simply tell if it hits something
	if(surfel)
	{
		//lambertian
		Color lamb =  surfel->material->diffuse;
		Color color;
		if(indirectRays==0) {color = lamb*.2f;}

		//get all lights in scene
		std::vector<std::shared_ptr<Light>> lights = scene->lights;
		//loop through
		//for all lights
		for(int i = 0; i < lights.size(); i++)
		{
				// multiply by dot product of light ray and normal
				Vector pos = lights[i]->position;
				Vector lightDir = pos - surfel->position;
				lightDir = lightDir.unit();
				
				bool shadowed = false;
				
				if(lights[i]->shadowsEnabled)
				{
					//test for shadow, cast ray from light
					Ray shadowTestRay = Ray(pos, -lightDir);
					std::shared_ptr<Surfel> shadowTestSurfel = CastSingleRay(scene, shadowTestRay);
					
					
					if(shadowTestSurfel && std::abs((pos - shadowTestSurfel->position).length())+.1 < (pos-surfel->position).abs().length())
					{
						shadowed = true;
					}
				}
				if(!shadowed)
				{
					float multiplier = lightDir.dot(surfel->normal);
					
					Color br = lights[i]->biradiance(surfel->position);
					Color addTo = lamb * multiplier * br;
					if(addTo.r < 0.0){ addTo.r = 0.0;}
					if(addTo.g < 0.0){ addTo.g = 0.0;}
					if(addTo.b < 0.0){ addTo.b = 0.0;}
					color += addTo;
			}
		}		
		//ambient lighting
		for(int i = 0; i < indirectRays; i++)
		{
			Vector indirectLightDir = Vector::hemiRandom(surfel->normal);
			Ray indirectLightRay = Ray(surfel->position, indirectLightDir.unit());
			Color otherLamb = Color(0.0,0.0,0.0);
			std::shared_ptr<Surfel> indirectSurfel = CastSingleRay(scene,indirectLightRay);
			if(indirectSurfel) {otherLamb = indirectSurfel->material->diffuse; }
			float multiplier = indirectLightDir.dot(surfel->normal);

			Color addTo = otherLamb * multiplier / indirectRays;
			if(addTo.r < 0.0){ addTo.r = 0.0;}
			if(addTo.g < 0.0){ addTo.g = 0.0;}
			if(addTo.b < 0.0){ addTo.b = 0.0;}
			color += addTo;

		}
		std::cout << x << ", " << y << std::endl;

		return color;
	}
	else {
		std::cout << "not found " << x << ", " << y << std::endl;

		return Color(0.,0.,0.);
	}
}



std::shared_ptr<Surfel> Raycaster::CastSingleRay(std::shared_ptr<Scene> scene, Ray ray)
{
	
	//get all triangles and etc. in the scene
	std::vector<Tri> tt = scene->posedMesh;

	Point P = ray.origin;
	Vector w = ray.direction;
	
	//std::cout << ray << std::endl;

	//find the closest triangle in the scene
	bool foundHit = false;
	float * minB;
	float minT = 10000000;
	int minI = -1;

	//loop through triangles and check for hits
	for(int i = 0; i < tt.size() ; i++)
	{
		float b [3];
		float t = 10000000;
		//MY VERSION
		foundHit = IntersectTriangle(ray, tt[i],b,t);

	
		if(t < minT && t >= 0.0f) { minB = b; minT = t; minI = i;}
	}
	
	
	//if it hit something
	if(minI > -1)
	{
			float distance = minT;

			Vector normal = -((tt[minI].points[1] - tt[minI].points[0]).cross(tt[minI].points[2] - tt[minI].points[1])).unit();
			std::shared_ptr<Surfel> surfel = std::make_shared<Surfel>(ray.origin + distance*ray.direction,normal,tt[minI].material);
			return surfel;
		

	}
	else
	{
		return NULL;
	}
}


bool Raycaster::IntersectTriangle(Ray ray, Tri t, float b[3], float& dist)
{
	Point P = ray.origin;
	Vector w = ray.direction;

	//edge vectors
	 Vector e_1 = t.points[1] - t.points[0];
	 Vector e_2 = t.points[2] - t.points[0];

	// Face normal
	 Vector n = e_1.cross(e_2).unit();

	 Vector q = w.cross(e_2);
	 float a = e_1.dot(q);

	// Backfacing / nearly parallel, or close to the limit of precision?
 	if (n.dot(w) >= 0)
	{
		return false;
	}
	if(std::abs(a) <= .0000001)
	{
		//std::cout << "e1" << e_1 << "e2" << e_2 << "q" << q <<std::endl;
		return false;
	}
	
	
	 Vector s = (P - t.points[0]) / a;
	 Vector r = s.cross(e_1);
	
  b[0] = s.dot(q);
	b[1] = r.dot(w);
	b[2] = 1.0f - b[0] - b[1];

	// Intersected outside triangle?
	if ((b[0] < 0.0f) || (b[1] < 0.0f) || (b[2] < 0.0f))
	{
		return false;
	}

  	dist = e_2.dot(r);
	return (dist >= 0.0f);
}

void Raycaster::KillThreads(std::thread * t, int numThreads)
{
        for (int i = 0; i < numThreads; ++i)
		{
             t[i].join();
		}
}

/* TODO:
Use tri tree data structure in order to calculate the nearest tri
*/

