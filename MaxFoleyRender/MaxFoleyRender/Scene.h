//
//  Scene.h
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 1/24/18.
//  Copyright © 2018 Maxwell Foley. All rights reserved.
//

#ifndef Scene_h
#define Scene_h

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

namespace MFR {

	class Scene {
		public:
			std::vector<std::shared_ptr<SceneObject>> objects;
			std::vector<std::shared_ptr<Light>> lights;
			std::shared_ptr<Camera> camera;
			std::vector<std::shared_ptr<Material>> materials;
			TriTree posedMesh;
		
			Scene() {
			
			}
		
			TriTree poseMesh()
			{
				std::vector<std::shared_ptr<Tri>> returnData;
				
				//loop through all objects in the scene
				for(std::vector<std::shared_ptr<SceneObject>>::iterator it = objects.begin(); it!=objects.end(); ++it)
				{
					std::vector<Tri> mesh = (*it)->mesh;
					
					//turn this into shared_ptrs here, as good of a place as any aaa idk
					std::vector<std::shared_ptr<Tri>> spMesh;
					for(std::vector<Tri>::iterator it2 = mesh.begin(); it2!=mesh.end(); ++it2)
					{
						spMesh.push_back(std::make_shared<Tri>(*it2));
					}
					
					/*
					//unwind materialIndexes for this object so that we get array of triangle index to material
					std::map<std::shared_ptr<Material>, std::vector<int>> materialIndexes = (*it).materialIndexes;
					
					std::vector<std::shared_ptr<Material>> materialList;
					
					for(std::map< std::shared_ptr<Material>, std::vector<int> >::iterator it2 = materialIndexes.begin(); it2 != materialIndexes.end(); ++it2) {
						
						Material material = it2->first;
						std::vector<int> indexVector = it2->second;
						
						//loop through the set of indexes associated with the material
						for(std::vector<int>::iterator it3 = indexVector.begin(); it3 != indexVector.end(); ++it3) {
							
							int index = *it3;
							//add it to materials vector at specified index
							materialList.insert(materials.begin() + materials, material);
						}
					}*/
					
				
					//now prep the tris
					for(std::vector<std::shared_ptr<Tri>>::iterator it2b = spMesh.begin(); it2b!=spMesh.end(); ++it2b)
					{
						std::shared_ptr<Tri> t = *it2b;
						

						t->points[0].scale((*it)->scale);
						t->points[1].scale((*it)->scale);
						t->points[2].scale((*it)->scale);

						t->points[0].rotateInDegrees((*it)->rotation);
						t->points[1].rotateInDegrees((*it)->rotation);
						t->points[2].rotateInDegrees((*it)->rotation);

						t->points[0].translate((*it)->position);
						t->points[1].translate((*it)->position);
						t->points[2].translate((*it)->position);

						returnData.push_back(t);
						
					}
				}
				
				posedMesh = TriTree(returnData);
				return posedMesh;
			}
	};
}
#endif /* Scene_h */
