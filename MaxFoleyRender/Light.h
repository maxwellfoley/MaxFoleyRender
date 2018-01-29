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
			Color color;
	
			Light(Point _origin, float _strength, LightType _type)
			{
				origin = _origin; strength = _strength; type = _type;
			}
		
			float biradiance(Point p)
			{
				//if(LightType = PointLght)
				float intensity = 1/pow((p-origin).length(),2);
				return intensity * strength;
			}
		};
}
#endif /* Light_h */
