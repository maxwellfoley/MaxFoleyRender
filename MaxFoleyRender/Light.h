//
//  Light.h
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 11/26/17.
//  Copyright © 2017 Maxwell Foley. All rights reserved.
//

#ifndef Light_h
#define Light_h

//for now just point light
enum MFR::LightType{Point};

class MFR::Light {
	public:
		MFR::Point origin;
		float strength;
		MFR::LightType type;
	
		MFR::Light(MFR::Point _origin, float _strength, MFR::LightType _type)
		{
			origin = _origin; strength = _strength; type = _type;
		}

}

#endif /* Light_h */
