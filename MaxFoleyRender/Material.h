//
//  Material.h
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 11/26/17.
//  Copyright © 2017 Maxwell Foley. All rights reserved.
//

#ifndef Material_h
#define Material_h

class MFR::Material
{
	public:
		//for now just lambertian
		MFR::Color lambertian;
	
		MFR::Material(MFR::Color _lambertian)
		{
			lambertian = _lambertian;
		}
}
#endif /* Material_h */
