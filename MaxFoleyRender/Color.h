//
//  Color.h
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 11/26/17.
//  Copyright © 2017 Maxwell Foley. All rights reserved.
//

#ifndef Color_h
#define Color_h

class MFR::Color {
	public:
		float r;
		float g;
		float b;
		float a;
	
		MFR::Color(float _r, float _g, float _b, float _a)
		{
			r = _r; g = _g; b = _b; a = _a;
		}

		MFR::Color(float _r, float _g, float _b)
		{
			r = _r; g = _g; b = _b; a = 1.0;
		}
}

#endif /* Color_h */
