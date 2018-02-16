//
//  TriTree.h
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 2/13/18.
//  Copyright © 2018 Maxwell Foley. All rights reserved.
//

#ifndef TriTree_h
#define TriTree_h

#include <vector>

namespace MFR {
	class KDNode {
		public:
			KDNode * left;
			KDNode * right;
			std::vector<std::shared_ptr<Tri>> tris;
			Box boundingBox;
			KDNode(){}
	};

	class TriTree {
		private:
			KDNode * root;
		
			KDNode * build(std::vector<std::shared_ptr<Tri>> tris, int depth) {
				KDNode * node = new KDNode();
				node->tris = tris;

				if(tris.size() == 0) {
					return node;
				}
				
				node->boundingBox = tris[0]->getBoundingBox();
				//expand bounding box
				for(int i = 1; i < tris.size(); i++)
				{
					node->boundingBox.expand(tris[i]->getBoundingBox());
				}
				
				//if we have less than 5 triangles here stop subdividing, very arbitrary and maybe needs changing TODO
				if(tris.size() <= 5) {
					node->left = new KDNode();
					node->right = new KDNode();
					return node;
				}
	
				Point midpoint = Point(0,0,0);
				for(int i = 0; i < tris.size(); i++)
				{
					midpoint = midpoint + (tris[i]->getMidpoint()/tris.size());
				}
				
				std::vector<std::shared_ptr<Tri>> leftTris;
				std::vector<std::shared_ptr<Tri>> rightTris;
				
				//alternate x y and z axis every time we go another level deeper
				int axis = depth%3;
				bool rightSide;

				for(int i = 0; i < tris.size(); i++)
				{
					switch(axis) {
						case 0:
							rightSide = midpoint.x() > tris[i]->getMidpoint().x(); break;
						case 1:
							rightSide = midpoint.y() > tris[i]->getMidpoint().y(); break;
						case 2:
							rightSide = midpoint.z() > tris[i]->getMidpoint().z(); break;
					}
					
					if(rightSide) {
						rightTris.push_back(tris[i]);
					}
					else {
						leftTris.push_back(tris[i]);
					}
				}
				
				//idk why i need this but worried itll break without it
				if(leftTris.size() == 0 && rightTris.size() > 0) leftTris = rightTris;
				if(rightTris.size() == 0 && leftTris.size() > 0) rightTris = leftTris;
				
				node->left = build(leftTris, depth+1);
				node->right = build(rightTris, depth+1);
				return node;
			}
		
			std::shared_ptr<Tri> irHelper(KDNode * node, Ray ray, float b[3], float& dist)
			{				b[0] = 1.0;

				if(node->boundingBox.intersect(ray))
				{
				
					//if we are not at a leaf, recurse
					if(node->left->tris.size()>0 || node->right->tris.size()>0)
					{
						std::shared_ptr<Tri> hitLeft = irHelper(node->left, ray, b, dist);
						
						std::shared_ptr<Tri> hitRight = irHelper(node->right, ray, b, dist);
						if(hitLeft!=NULL) { return hitLeft; }
						else if(hitRight!=NULL) { return hitRight; }
						else{
							return NULL;
						}
					}
					else
					{
						//we are at a leaf, now just iterate through triangles like normal
						for(int i = 0; i < node->tris.size(); i++)
						{
							bool hit = node->tris[i]->intersect(ray,b,dist);
							if(hit) return node->tris[i];
						}
					}
				}
				//if the ray doesnt intersect the bounding box
				return NULL;
				
			}
		
		public:
			TriTree() {
			
			}
		
			TriTree(std::vector<std::shared_ptr<Tri>> tris)
			{
				root = build(tris,0);
			}
		
			std::shared_ptr<Tri> intersectRay(Ray ray, float * b, float & dist)
			{
				b[0] = 1.0;
				return irHelper(root,ray,b,dist);
			}
	};
}

#endif /* TriTree_h */

// https://en.wikipedia.org/wiki/K-d_tree
// http://tuprints.ulb.tu-darmstadt.de/893/2/Adamson_Kapitel5-7.pdf
// http://www.cs.utah.edu/~reinhard/papers/egwr2k.pdf
// https://blog.frogslayer.com/kd-trees-for-faster-ray-tracing-with-triangles/ this is the one
