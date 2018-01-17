//
//  main.c
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 11/26/17.
//  Copyright © 2017 Maxwell Foley. All rights reserved.
//

#include <stdio.h>
#include "Vector.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
	MFR::Vector v = MFR::Vector(3,3,3);
	MFR::Vector v2 = MFR::Vector(1,2,3);
	MFR::Vector v3 = v / v2;
	
	std::cout << v.toString() << std::endl;
	std::cout << v3.toString() << std::endl;
	
	MFR::Vector v3unit = v3.unit();
	float length = v3unit.length();
	
	std::cout << v3unit.toString() << std::endl;
	std::cout << length << std::endl;
	
	MFR::Vector v4 = v / 3;
	std::cout << v.toString() << std::endl;
	std::cout << v4.toString() << std::endl;

	

    return 0;
}
