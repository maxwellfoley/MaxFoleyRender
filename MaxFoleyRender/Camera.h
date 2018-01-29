//
//  Camera.h
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 11/26/17.
//  Copyright © 2017 Maxwell Foley. All rights reserved.
//

#ifndef Camera_h
#define Camera_h

#include "Vector.h"

namespace MFR {

	class Camera
	{
		public:
			Point position;
			Point rotation;
			float fov;
			float focalLength;
	
			Camera(){}
	};
}

#endif /* Camera_h */
