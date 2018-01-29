//
//  Light.h
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 11/26/17.
//  Copyright © 2017 Maxwell Foley. All rights reserved.
//

#ifndef Light_h
#define Light_h

namespace MFR {

	//for now just point light
	enum LightType{PointLight};

	class Light {
		public:
			Point origin;
			float strength;
			LightType type;
			bool shadowsEnabled;
	
			Light(Point _origin, float _strength, LightType _type)
			{
				origin = _origin; strength = _strength; type = _type;
			}
		};
}
#endif /* Light_h */
