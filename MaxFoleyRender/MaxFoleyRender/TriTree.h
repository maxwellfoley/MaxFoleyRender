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
			{				

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
			std::string preorderPrint(KDNode * node, int depth) const
			{
				//if node is null or empty, we hit bottom, dont print anything
				if(!node || node->tris.size() == 0)
				{
					return "";
				}
				
				//otherwise form a string to represent this node
				std::string str;
				
				for(int i = 0; i < depth; i++)
				{
					str.append("-");
				}
				
				str.append(std::to_string(node->tris.size()));
				str.append(" tris");
				
				str.append(", box size: ");
				str.append(std::to_string(node->boundingBox.width));
				str.append(" x ");
				str.append(std::to_string(node->boundingBox.height));
				str.append(" x ");
				str.append(std::to_string(node->boundingBox.depth));
				str.append(" origin: ");
				str.append(std::to_string(node->boundingBox.origin.x()));
				str.append(", ");
				str.append(std::to_string(node->boundingBox.origin.y()));
				str.append(", ");
				str.append(std::to_string(node->boundingBox.origin.z()));
				str.append("\n");
				
				//TEMPORARY:: check to see if this leaf has the triangle we are looking for
				/*
				for(std::vector<std::shared_ptr<MFR::Tri>>::iterator it = node->tris.begin(); it != node->tris.end(); ++it) {
					if((*it)->points[0].x() == -5 &&
					(*it)->points[0].y() == 5 &&
					(*it)->points[0].z() == -4.9 &&
					(*it)->points[1].x() == -5 &&
					(*it)->points[1].y() == -5 &&
					(*it)->points[1].z() == -4.9 &&
					(*it)->points[2].x() == 5 &&
					(*it)->points[2].y() == -5 &&
					(*it)->points[2].z() == -4.9)
					{
						str.append("THIS IS THE NODE WHERE OUR TRIANGLE IS ^^^ \n");
					}
				}*/
				
				//if it is a leaf, print the triangles
				if(node->left->tris.size() == 0 && node->right->tris.size() == 0)
				{

					for(std::vector<std::shared_ptr<MFR::Tri>>::iterator it = node->tris.begin(); it != node->tris.end(); ++it) {
						for(int i = 0; i < depth+1; i++)
						{
							str.append("-");
						}
					
						std::stringstream ss;
						ss << *(*it);char  c[100];
						ss.get(c,100,'@');
						str.append(c);
						str.append(")\n");
					}
					
				}
				
				//then inorder print
				std::string first = preorderPrint(node->left, depth+1);
				str.append(first);
				std::string last = preorderPrint(node->right, depth+1);
				str.append(last);
				return str;
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
				return irHelper(root,ray,b,dist);
			}
		
			std::vector<std::shared_ptr<Tri>> getTriVector()
			{
				return root->tris;
			}
		
			friend std::ostream& operator<<(std::ostream &strm, const TriTree &tt) {
				return strm << tt.preorderPrint(tt.root,1);
			}
		
	};
}

#endif /* TriTree_h */

// https://en.wikipedia.org/wiki/K-d_tree
// http://tuprints.ulb.tu-darmstadt.de/893/2/Adamson_Kapitel5-7.pdf
// http://www.cs.utah.edu/~reinhard/papers/egwr2k.pdf
// https://blog.frogslayer.com/kd-trees-for-faster-ray-tracing-with-triangles/ this is the one
