//
//  Surfel.h
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 11/26/17.
//  Copyright © 2017 Maxwell Foley. All rights reserved.
//

#ifndef Surfel_h
#define Surfel_h

#include "Vector.h"
#include "Material.h"

namespace MFR {
	class Surfel {
		public:
			Point position;
			Vector normal;
			std::shared_ptr<Material> material;
		
			Surfel()
			{}
	
			Surfel(Point _position, Vector _normal, std::shared_ptr<Material> _material)
			{
				position = _position; normal = _normal; material = _material;
			}

	};
}

#endif /* Surfel_h */
