//
//  SceneObject.h
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 1/22/18.
//  Copyright © 2018 Maxwell Foley. All rights reserved.
//

#include "Vector.h"
#include "Material.h"
#include "Tri.h"
#include <vector>
#include <string>

#ifndef SceneObject_h
#define SceneObject_h

namespace MFR {

	class SceneObject {
		public:
			std::vector<Tri> mesh;
			Point origin;
			std::map<std::string, Material> materials;
			std::map<Material, std::vector<int>> materialIndexes;
		
			SceneObject(std::string filename)
			{
				loadFromObjFile(filename);
			}
		
			void loadFromObjFile(std::string filename);
	};
}

#endif /* SceneObject_h */
