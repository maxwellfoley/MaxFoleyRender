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
			Point position;
			float strength;
			LightType type;
			bool shadowsEnabled;
			Color color;
	
			Light() {
			
			}
			Light(Point _position, float _strength, LightType _type)
			{
				position = _position; strength = _strength; type = _type;
			}
		
			float biradiance(Point p)
			{
				//return 1.0;
				//if(LightType = PointLght)
				float intensity = 1/pow((p-position).length(),2);
				return intensity * strength;
			}
		};
}
#endif /* Light_h */
