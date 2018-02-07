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
			Point position;
			Vector rotation;
			Vector scale;
			std::map<std::string, std::shared_ptr<Material>> materials;
			//std::map<std::shared_ptr<Material>, std::vector<int>> materialIndexes;
		
			SceneObject(std::string filename)
			{
				loadFromObjFile(filename);
			}
		
			void loadFromObjFile(std::string filename);
		
			void setMaterialOnAll(std::shared_ptr<Material> material)
			{
				for(std::vector<Tri>::iterator it = mesh.begin(); it != mesh.end(); ++it)
				{
					(*it).material = material;
				}
			}
		
			friend std::ostream& operator<<(std::ostream &strm, const MFR::SceneObject &obj) {
				return strm << "Object with " << obj.mesh.size() << " triangles";
			}
	};
}

#endif /* SceneObject_h */
