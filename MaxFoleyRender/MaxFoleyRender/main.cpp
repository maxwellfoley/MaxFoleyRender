//
//  main.c
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 11/26/17.
//  Copyright © 2017 Maxwell Foley. All rights reserved.
//

#include <stdio.h>
#include "Vector.h"
#include "SDL2/SDL.h"

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

	//Start SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	
	//Set up screen
	SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if (win == nullptr){
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr){
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	
	SDL_Surface *bmp = SDL_LoadBMP("hello_world.bmp");
	if (bmp == nullptr){
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	
	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
	SDL_FreeSurface(bmp);
	if (tex == nullptr){
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	
	//A sleepy rendering loop, wait for 3 seconds and render and present the screen each time
	for (int i = 0; i < 3; ++i){
		//First clear the renderer
		SDL_RenderClear(ren);
		//Draw the texture
		SDL_RenderCopy(ren, tex, NULL, NULL);
		//Update the screen
		SDL_RenderPresent(ren);
		//Take a quick break after all that hard work
		SDL_Delay(1000);
	}
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	
	//http://www.willusher.io/sdl2%20tutorials/2013/08/17/lesson-1-hello-world
	//Quit SDL
	SDL_Quit();

    return 0;
}

/*TODO:
Figure out how to display images when the program is run and write image to file
	- sdl?? http://www.lazyfoo.net/SDL_tutorials/lesson01/mac/xcode/index.php
	- write to file how???
	
Figure out how to load scenes in order to get camera, light, material, and mesh info
And what format to use?

Implement all the rendering logic 

Figure out how to do the memory stuff

*/
