//
//  SceneObject.cpp
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 1/23/18.
//  Copyright © 2018 Maxwell Foley. All rights reserved.
//

#include <stdio.h>
#include "SceneObject.h"
#include <sstream>
#include <string>

void MFR::SceneObject::loadFromObjFile(std::string filename) {
	
	//step 1: figure out how to read in a file line by line
	//step 2: get all lines starting with v and add them to an array of points
	//step 3: get all lines starting with f and using them to add all these points to the tris array
	//step 4: put in usemtl, this will require figuring out how to load materials as well
	//https://www.cs.cmu.edu/~mbz/personal/graphics/obj.html
	
	
	#include <fstream>
	std::ifstream infile(filename);
	
	std::string line;
	std::vector<Point> vertices;
	Material currentMaterial = Material();

	while (std::getline(infile, line))
	{
		std::stringstream iss = std::stringstream(line);
		std::string firstToken;
		iss >> firstToken;
		
		//v is for vertex, add these to an array of points
		if(firstToken == "v") {
			std::string a, b, c;
			iss >> a;
			iss >> b;
			iss >> c;
			vertices.push_back(Point(std::stof(a),std::stof(b),std::stof(c)));
		}
		//f is for face, add these to our tri array
		else if(firstToken == "f") {
		
			//first just get all the tokens
			std::string token;
			std::string tokens[3];
			for(int i = 0; iss >> token; i++)
			{
				if(i > 2) {//TODO: throw error, mesh should only be tris
				}
				tokens[i] = token;
			}
			
			//remove everything from the token which occurs after the first slash
			for(int i = 0; i < 3; i++){
				tokens[i] = tokens[i].substr(tokens[i].find("/"));
			}
			
			//now add the face information to the vector
			mesh.push_back(Tri(vertices[std::stoi(tokens[0])],
				vertices[std::stoi(tokens[1])],
				vertices[std::stoi(tokens[2])]));
			
			//add link to the material
			materialIndexes[currentMaterial].push_back(mesh.size()-1);
		}
		else if(firstToken == "mtllib") {
			//load the materials from the file and place them in the list
			std::string filename;
			iss >> filename;
			std::vector<Material> mats = Material::getMaterialsFromFile(filename);
			
			for(std::vector<Material>::iterator it = mats.begin(); it != mats.end(); ++it)
			{
				materials[(*it).name] = *it;
			}

		}
		else if(firstToken == "usemtl") {
			std::string materialName;
			iss >> materialName;
			currentMaterial = materials[materialName];
			//if material isn't found, it fails gracefully by just using a blank material
		}
	}
}
