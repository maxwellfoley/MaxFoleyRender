//
//  Raycaster.cpp
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 1/28/18.
//  Copyright © 2018 Maxwell Foley. All rights reserved.
//

#include <stdio.h>
#include "Raycaster.h"

using namespace MFR;

void Raycaster::RenderImage(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene, Color * pixels, int width, int height, int indirectRays)
{
		for(int j = 0; j < height; j++)
		{
			for(int i = 0; i < width; i++)
			{	
				Color color = GetPixelColor(camera, scene, i, j, width, height, indirectRays);
				pixels[j*width +i] = color;
			}
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
				Vector pos = lights[i]->origin;
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

					Color addTo = lamb * multiplier * lights[i]->biradiance(surfel->position);
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
	std::vector<Tri> tt = scene->poseMesh();

	Point P = ray.origin;
	Vector w = ray.direction;


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

			Vector normal = ((tt[minI].points[1] - tt[minI].points[0]).cross(tt[minI].points[2] - tt[minI].points[1])).unit();
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
 	if ((n.dot(w) >= 0) || (std::abs(a) <= .0000001)) return false;
	
	 Vector s = (P - t.points[0]) / a;
	 Vector r = s.cross(e_1);
	
  b[0] = s.dot(q);
	b[1] = r.dot(w);
	b[2] = 1.0f - b[0] - b[1];

	// Intersected outside triangle?
	if ((b[0] < 0.0f) || (b[1] < 0.0f) || (b[2] < 0.0f)) return false;

  dist = e_2.dot(r);
	return (dist >= 0.0f);
}


/* TODO:

Consider refactoring the way materials are attached to tris

*/

