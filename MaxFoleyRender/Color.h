//
//  Color.h
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 11/26/17.
//  Copyright © 2017 Maxwell Foley. All rights reserved.
//

#ifndef Color_h
#define Color_h

namespace MFR {
	class Color {
		public:
			float r;
			float g;
			float b;
		
			Color(void)
			{/*
				a = 1.0;
				r = 0.0;
				g = 0.0;
				b = 0.0;				
			*/
				r = 0.0;
				g = 0.0;
				b = 0.0;
			}


			Color(float _r, float _g, float _b)
			{
				r = _r; g = _g; b = _b;
			}
		

	};
}

#endif /* Color_h */

