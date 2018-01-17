//
//  Surfel.h
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 11/26/17.
//  Copyright © 2017 Maxwell Foley. All rights reserved.
//

#ifndef Surfel_h
#define Surfel_h

class MFR::Surfel {
	public:
		MFR::Point position;
		MFR::Vector normal;
		MFR::Material material;
	
		MFR::Surfel(MFR::Point _position, MFR::Vector _normal, MFR::Material _material)
		{
			position = _position; normal = _normal; material = _material;
		}

}

#endif /* Surfel_h */
