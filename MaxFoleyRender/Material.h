//
//  Material.h
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 11/26/17.
//  Copyright © 2017 Maxwell Foley. All rights reserved.
//

#ifndef Material_h
#define Material_h

#include "Color.h"

namespace MFR
{
	enum IllumMode {flat, lambert, blinnPhong};
	
	class Material
	{
		public:
			std::string name;
			//for now just lambertian
			Color ambient;
			Color diffuse;
			Color specular;
			float specularExponent;
			IllumMode illumMode;
		
			//default white material
			Material()
			{
				name = "default";
				ambient = Color(0.0,0.0,0.0);
				diffuse = Color(1.0,1.0,1.0);
				specular = Color(1.0,1.0,1.0);
				specularExponent = 1.0;
				illumMode = lambert;
			}
		
			//constructor with lambertian
			Material(MFR::Color diffuseColor)
			{
				name = "default";
				ambient = Color(0.0,0.0,0.0);
				diffuse = diffuseColor;
				specular = Color(1.0,1.0,1.0);
				specularExponent = 1.0;
				illumMode = lambert;
			}
		
			friend bool operator< (const Material& a, const Material& b){
				return a.name < b.name;
			}
		
			friend std::ostream& operator<<(std::ostream &strm, const MFR::Material &mat) {
				return strm << "Material with ambient " << mat.ambient << ", diffuse " << mat.diffuse << ", specular " << mat.specular << " specularExponent " << mat.specularExponent << ", illumination mode " << mat.illumMode;
			}
		
			static std::vector<std::shared_ptr<Material>> getMaterialsFromFile(std::string filename)
			{
				std::string line;
				std::ifstream infile(filename);
				std::string curMaterial = "";
				std::vector<std::shared_ptr<Material>> output;
				
				while (std::getline(infile, line))
				{
					std::stringstream iss = std::stringstream(line);
					std::string firstToken;
					iss >> firstToken;
					if(firstToken == "newmtl")
					{
						//create and add new material with this name
						std::string newMtlName;
						iss >> newMtlName;
						curMaterial = newMtlName;
						Material m;
						m.name = curMaterial;
						output.push_back(std::make_shared<Material>(m));
					}
					else if(firstToken == "Ka")
					{
						std::string r,g,b;
						iss >> r;
						iss >> g;
						iss >> b;
						output[output.size()-1]->ambient = Color(std::stof(r),std::stof(g),std::stof(b));
					}
					else if(firstToken == "Kd")
					{
						std::string r,g,b;
						iss >> r;
						iss >> g;
						iss >> b;
						output[output.size()-1]->diffuse = Color(std::stof(r),std::stof(g),std::stof(b));
					
					}
					else if(firstToken == "Ks")
					{
						std::string r,g,b;
						iss >> r;
						iss >> g;
						iss >> b;
						output[output.size()-1]->specular = Color(std::stof(r),std::stof(g),std::stof(b));
					
					}
					else if(firstToken == "Ns")
					{
						std::string e;
						iss >> e;
						output[output.size()-1]->specularExponent = std::stof(e);
					}
					else if(firstToken == "illum")
					{
						std::string illumToken;
						iss >> illumToken;
						int illumNum = std::stoi(illumToken);
						switch(illumNum)
						{
							case 0: output[output.size()-1]->illumMode = flat; break;
							case 1: output[output.size()-1]->illumMode = lambert; break;
							case 2: output[output.size()-1]->illumMode = blinnPhong; break;
								
						}
					}

				}

				return output;
			}
	};
}
#endif /* Material_h */
