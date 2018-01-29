//
//  Vector.h
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 11/26/17.
//  Copyright © 2017 Maxwell Foley. All rights reserved.
//
#include <armadillo>

#ifndef Vector_h
#define Vector_h

namespace MFR {

	class Vector {
		private:
			arma::fvec arma_Vector;
		public:
			Vector()
			{
				arma_Vector << 0.0 << 0.0 << 0.0;
			}
			Vector(float x, float y, float z)
			{
				arma_Vector << x << y << z;
			}
			Vector(arma::fvec arma_Vector)
			{
				this->arma_Vector = arma_Vector;
			}
		
			//accessors
			float x(){
				return arma_Vector(0);
			}
		
			float y(){
				return arma_Vector(1);
			}
		
			float z(){
				return arma_Vector(2);
			}
		
		
			//unary negation
			Vector operator- ()
			{
				this->arma_Vector = -arma_Vector;
				return *this;
			}

			//addition
			Vector & operator+=(const Vector& rhs)
			{
				this->arma_Vector = this->arma_Vector + rhs.arma_Vector;
				return *this;
			}
			friend MFR::Vector operator+(MFR::Vector lhs, const MFR::Vector& rhs)
			{
				lhs += rhs;
				return lhs;
			}
		
			//subtraction
			Vector & operator-=(const Vector& rhs)
			{
				this->arma_Vector = this->arma_Vector - rhs.arma_Vector;
				return *this;
			}
			friend MFR::Vector operator-(MFR::Vector lhs, const MFR::Vector& rhs)
			{
				lhs -= rhs;
				return lhs;
			}
		
			//multiplication by scalar
			Vector & operator*=(float f)
			{
				this->arma_Vector = this->arma_Vector * f;
				return *this;
			}
			friend MFR::Vector operator*(const float rhs, MFR::Vector lhs)
			{
				lhs *= rhs;
				return lhs;
			}
			friend MFR::Vector operator*(MFR::Vector lhs, const float rhs)
			{
				lhs *= rhs;
				return lhs;
			}
		
			//division by scalar
			Vector & operator/=(float f)
			{
				this->arma_Vector = this->arma_Vector / f;
				return *this;
			}
		
			friend MFR::Vector operator/(MFR::Vector lhs, const float rhs)
			{
				lhs /= rhs;
				return lhs;
			}


			//division
			Vector & operator/=(const Vector& rhs)
			{
				this->arma_Vector = this->arma_Vector / rhs.arma_Vector;
				return *this;
			}
			friend MFR::Vector operator/(MFR::Vector lhs, const MFR::Vector& rhs)
			{
				lhs /= rhs;
				return lhs;
			}
		
		
			//length
			float length() {
				return arma::norm(arma_Vector);
				
			}
		
			//unit vector
			Vector unit() {
				Vector v = Vector((*this) / this->length());
				return v;
			}
		
			//dot product
			float dot(const Vector & rhs)
			{
				return arma::dot(this->arma_Vector, rhs.arma_Vector);
			}
		
			//cross product probably
			Vector cross(const Vector & rhs)
			{
				Vector v = Vector(arma::cross(this->arma_Vector, rhs.arma_Vector));
				return v;
			}
		
			//to string
			std::string toString()
			{
				return "(" + std::to_string(arma_Vector(0)) + ", " + std::to_string(arma_Vector(1)) + ", " + std::to_string(arma_Vector(2)) + ")";

			}
			friend std::ostream& operator<<(std::ostream &strm, const MFR::Vector &v) {
				return strm << "(" << v.arma_Vector(0) << ", " << v.arma_Vector(1) << ", "
				 << v.arma_Vector(2) <<  ")";
			}
		
			Vector & abs()
			{
				this->arma_Vector = arma::abs(this->arma_Vector);
				return *this;
			}
		
		
			Vector translate(Vector translation)
			{
				Vector v = *this + translation;
				return v;
			}
		
			Vector rotate(Vector rotation)
			{
				arma::fmat matX;
				matX << 1.0 << 0.0 << 0.0
				<< 0.0 << float(cos(rotation.x())) << float(-sin(rotation.x()))
				<< 0.0 << float(sin(rotation.x())) << float(cos(rotation.x()));
				
				arma::fmat matY;
				matY << float(cos(rotation.y())) << 0.0 << float(sin(rotation.y()))
				<< 0.0 << 1.0 << 0.0
				<< float(-sin(rotation.y())) << 0.0 << float(cos(rotation.y()));
				
				arma::fmat matZ;
				matZ << float(cos(rotation.z())) << float(-sin(rotation.z())) << 0.0
				<< float(sin(rotation.z())) << float(cos(rotation.z())) << 0.0
				<< 0.0 << 0.0 << 1.0;
				
				Vector v = Vector(arma_Vector*matX*matY*matZ);
				return v;
			}
		
			Vector scale(Vector scale)
			{
				arma::fvec returnVal;
				returnVal << arma_Vector(0) * scale.x() << arma_Vector(1) * scale.y() << arma_Vector(2) * scale.z();
				Vector v = Vector(returnVal);
				return v;
			}
		
			static Vector hemiRandom(Vector v)
			{
				//get random numbers between -pi/2 and pi/2
				float pi = 3.14159265358979323846;
				float xArcRot = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/pi);
				xArcRot -= (pi/2);
				float yArcRot = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/pi);
				yArcRot -= (pi/2);

				//convert vector from cartesian to spherical, add the random angles, convert back
				//http://tutorial.math.lamar.edu/Classes/CalcIII/SphericalCoords.aspx
				
				//cartesian to spherical
				float distance = v.length();
				float rho = acos(v.z()/distance);
				float theta = asin(v.y()/(distance*sin(rho)));
				
				rho+=xArcRot;
				theta+=yArcRot;
				
				//spherical to cartesian
				float x = distance*sin(rho)*cos(theta);
				float y = distance*sin(rho)*sin(theta);
				float z = distance*cos(rho);
				
				Vector returnVal = Vector(x,y,z);
				return returnVal;

			}


	};

	using Point = MFR::Vector;
}
#endif /* Vector_h */
