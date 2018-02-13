//
//  Ray.h
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 11/26/17.
//  Copyright © 2017 Maxwell Foley. All rights reserved.
//

#ifndef Ray_h
#define Ray_h

#include "Vector.h"

namespace MFR {
	class Ray {
    	public:
        	Point origin;
        	Vector direction;
			
			Ray(){}
		
			Ray(Point o, Vector dir)
			{
				origin = o;
				direction = dir;
			}
			friend std::ostream& operator<<(std::ostream &strm, const MFR::Ray &ray) {
				return strm << "Ray with position " << ray.origin << " and direction" << ray.direction;
			}
		
	};
}
#endif /* Ray_h */

