//
//  Box.h
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 2/13/18.
//  Copyright © 2018 Maxwell Foley. All rights reserved.
//

#ifndef Box_h
#define Box_h

#include "Ray.h"

namespace MFR {
	class Box
	{
		public:
			Point origin;
			int width;
			int height;
			int depth;
		
			Box(){}
		
			Box(Point _origin, int _width, int _height, int _depth)
			{
				origin = _origin; width = _width; height = _height; depth = _depth;
			}
		
			Box expand(Box b)
			{
				float xMin = std::min(b.origin.x(),origin.x());
				xMin = std::min(xMin,b.origin.x()+b.width);
				xMin = std::min(xMin,origin.x()+width);
				
				float yMin = std::min(b.origin.y(),origin.y());
				yMin = std::min(yMin,b.origin.y()+b.height);
				yMin = std::min(yMin,origin.y()+height);
				
				float zMin = std::min(b.origin.z(),origin.z());
				zMin = std::min(zMin,b.origin.z()+b.depth);
				zMin = std::min(zMin,origin.z()+depth);
				
				
				float xMax = std::max(b.origin.x(),origin.x());
				xMax = std::max(xMax,b.origin.x()+b.width);
				xMax = std::max(xMax,origin.x()+width);
				
				float yMax = std::max(b.origin.y(),origin.y());
				yMax = std::max(yMax,b.origin.y()+b.height);
				yMax = std::max(yMax,origin.y()+height);
				
				float zMax = std::max(b.origin.z(),origin.z());
				zMax = std::max(zMax,b.origin.z()+b.depth);
				zMax = std::max(zMax,origin.z()+depth);
				
				origin = Point(xMin,yMin,zMin);
				width = xMax-xMin;
				height = yMax-yMin;
				depth = zMax-zMin;

				return *this;
				
			}
		
			//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
			bool intersect(Ray ray) {
				Point minBound = origin;
				Point maxBound = Point(width,height,depth);
				
				//contains the amount of traveling along the ray you would need to do to enter the bounds from the "near side" on x,y,and z planes
				Point entranceDists = (minBound-ray.origin)/ray.direction;
				
				//contains the amount of traveling along the ray you would need to do to enter the bounds from the "far side" on x,y,and z planes
				Point exitDists = (maxBound-ray.origin)/ray.direction;
				
				//either ALL the exit dists must be bigger than the entrance dists (if it enters from the front) or vice versa (if it enters from the back). otherwise it doesnt actually hit the cube
				
				bool enterFromTheFront = (exitDists.y() > entranceDists.x()) && (exitDists.z() > entranceDists.x()) &&
				(exitDists.x() > entranceDists.y()) && (exitDists.z() > entranceDists.y()) &&
				(exitDists.x() > entranceDists.z()) && (exitDists.y() > entranceDists.z());
				
				bool enterFromTheBack = (entranceDists.y() > exitDists.x()) && (entranceDists.z() > exitDists.x()) &&
				(entranceDists.x() > exitDists.y()) && (entranceDists.z() > exitDists.y()) &&
				(entranceDists.x() > exitDists.z()) && (entranceDists.y() > exitDists.z());
				
				if(!(enterFromTheFront || enterFromTheBack))
				{
					return false;
				}
				
				//the one that actually enters the box is the greatest one
				float trueEntranceDist = std::max(std::max(entranceDists.x(),entranceDists.y()),entranceDists.z());
				
				//the one that actually enters the box is the least one
				float trueExitDist = std::min(std::min(exitDists.x(),exitDists.y()),exitDists.z());

				//it only actually counts if these distances are positive
				if(trueEntranceDist >= 0 || trueExitDist >= 0)
				{
					return true;
				}
				else
				{
					return false;
				}


			}
	};
}
#endif /* Box_h */
