//
//  Tri.h
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 1/22/18.
//  Copyright © 2018 Maxwell Foley. All rights reserved.
//

#ifndef Tri_h
#define Tri_h
#include <cmath>
#include "Box.h"
#include "Ray.h"

namespace MFR {

	class Tri {
		public:
			Point points[3];
			std::shared_ptr<Material> material;
			Tri(Point a, Point b, Point c)
			{
				points[0] = a;
				points[1] = b;
				points[2] = c;
			}
		
			Tri()
			{
				points[0] = Point();
				points[1] = Point();
				points[2] = Point();
			}
	
			friend std::ostream& operator<<(std::ostream &strm, const MFR::Tri &tri) {
				return strm << "Triangle with points " << tri.points[0] << " " << tri.points[1]
				<< " " << tri.points[2] << " @";
			}
		
			Point getMidpoint() {
				//just average the three points, this returns the centroid
				return (points[0] + points[1] + points[2])/3;
			}
		
			Box getBoundingBox(){
			/*
				//figure out which of the possible distances between midpoints and opposing vertices is longest
				Point midpoints[3];
				midpoints[0] = (points[1]+points[2])/2;
				midpoints[1] = (points[0]+points[2])/2;
				midpoints[2] = (points[0]+points[1])/2;
				
				float midpointDists[3];
				midpointDists[0] = std::abs(midpoints[0].distance(points[0]));
				midpointDists[1] = std::abs(midpoints[1].distance(points[1]));
				midpointDists[2] = std::abs(midpoints[2].distance(points[2]));

				int minAxis = 0;
				int minAxisLength = midpointDists[0];
				for(int i = 1; i < 3; i++)
				{
					if(midpointDists[i] < minAxisLength)
					{
						minAxisLength = midpointDists[i];
						minAxis = i;
					}
				}
				
				//might be wrong
				Point origin = points[(minAxis+1)%3];
				Vector direction = points[minAxis]-midpoints[minAxis];
				float width = origin.distance(points[(minAxis+2)%3]);
				*/
				
				float minX = std::min(points[0].x(), points[1].x());
				minX = std::min(minX, points[2].x());
				
				float minY = std::min(points[0].y(), points[1].y());
				minY = std::min(minY, points[2].y());
				
				float minZ = std::min(points[0].z(), points[1].z());
				minY = std::min(minY, points[2].z());

				float maxX = std::max(points[0].x(), points[1].x());
				maxX = std::max(maxX, points[2].x());
				
				float maxY = std::max(points[0].y(), points[1].y());
				maxY = std::max(maxY, points[2].y());
				
				float maxZ = std::max(points[0].z(), points[1].z());
				maxZ = std::max(maxZ, points[2].z());
				
				return Box(Point(minX,minY,minZ),.001+maxX-minX,.001+maxY-minY,.001+maxZ-minZ);
			}
		
		//TODO: implement, just copy the logic over from raycaster
		bool intersect(Ray ray, float b[3], float & dist)
		{
			Point P = ray.origin;
			Vector w = ray.direction;
			//edge vectors
			Vector e_1 = points[1] - points[0];
			Vector e_2 = points[2] - points[0];
			
			// Face normal
			Vector n = e_1.cross(e_2).unit();
			
			Vector q = w.cross(e_2);
			float a = e_1.dot(q);
			
			// Backfacing / nearly parallel, or close to the limit of precision?
			if (n.dot(w) >= 0)
			{
			//	std::cout << "false because backfacing or close to limit of precision" << std::endl;
				
				return false;
			}
			if(std::abs(a) <= .0000001)
			{
			///	std::cout << "false because  a is close to zero" << std::endl;
				return false;
			}
			
			Vector s = (P - points[0]) / a;
			Vector r = s.cross(e_1);
			
  			b[0] = s.dot(q);
			b[1] = r.dot(w);
			b[2] = 1.0f - b[0] - b[1];
			
			/*
			float b0 = b[0];
			float b1 = b[1];
			float b2 = b[2];
			
			float t0x = points[0].x();
			float t0y = points[0].y();
			float t0z = points[0].z();
		
			float px = P.x();
			float py = P.y();
			float pz = P.z();
			float wx = w.x();
			float wy = w.y();
			float wz = w.z();
			
			float t1x = points[1].x();
			float t1y = points[1].y();
			float t1z = points[1].z();
			float t2x = points[2].x();
			float t2y = points[2].y();
			float t2z = points[2].z();*/
			
			// Intersected outside triangle?
			if ((b[0] < 0.0f) || (b[1] < 0.0f) || (b[2] < 0.0f))
			{
			//	std::cout << "false because it intersected outside triangle" << std::endl;
				return false;
			}
			
  			dist = e_2.dot(r);
			return (dist >= 0.0f);
			
		}
	};
}

#endif /* Tri_h */
