//
//  Tri.h
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 1/22/18.
//  Copyright © 2018 Maxwell Foley. All rights reserved.
//

#ifndef Tri_h
#define Tri_h

namespace MFR {

	class Tri {
		public:
			Point points[3];
			std::shared_ptr<Material> material;
			Tri(Point a, Point b, Point c)
			{
				points[0] = a;
				points[1] = b;
				points[2] = c;
			}
		
			Tri()
			{
				points[0] = Point();
				points[1] = Point();
				points[2] = Point();
			}
	
			friend std::ostream& operator<<(std::ostream &strm, const MFR::Tri &tri) {
				return strm << "Triangle with points " << tri.points[0] << " " << tri.points[1]
				<< " " << tri.points[2];
			}
	};
}

#endif /* Tri_h */
