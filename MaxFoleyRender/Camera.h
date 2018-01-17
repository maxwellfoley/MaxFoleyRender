//
//  Camera.h
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 11/26/17.
//  Copyright © 2017 Maxwell Foley. All rights reserved.
//

#ifndef Camera_h
#define Camera_h

class MFR::Camera
{
	public:
		MFR::Point position;
		MFR::Point rotation;
		float fov;
		float focalLength;
	
		MFR::Camera(){}
}


#endif /* Camera_h */
