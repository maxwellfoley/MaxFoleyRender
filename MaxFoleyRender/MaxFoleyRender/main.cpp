//
//  main.c
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 11/26/17.
//  Copyright © 2017 Maxwell Foley. All rights reserved.
//

#include <stdio.h>
#include <algorithm>

#include "Vector.h"
#include "Color.h"
#include "Raycaster.h"
#include "Scene.h"
#include "SceneObject.h"
#include "SDL2/SDL.h"
#include <pngwriter.h>

std::shared_ptr<MFR::Scene> makeCornellBoxScene() {
	std::shared_ptr<MFR::Scene> scene = std::make_shared<MFR::Scene>();
	
	//left wall is red
	std::shared_ptr<MFR::Material> redMaterial = std::make_shared<MFR::Material>(MFR::Color(1.0,0.0,0.0));
	std::shared_ptr<MFR::SceneObject> leftWall = std::make_shared<MFR::SceneObject>("cube.obj");
	leftWall->setMaterialOnAll(redMaterial);
	leftWall->scale = MFR::Point(0.2,10.0,10.0);
	leftWall->position = MFR::Point(-5.0,0.0,0.0);
	leftWall->rotation = MFR::Point(0.0,90.0,0.0);
	scene->objects.push_back(leftWall);

	
	//right wall is green
	std::shared_ptr<MFR::Material> greenMaterial = std::make_shared<MFR::Material>(MFR::Color(0.0,1.0,0.0));
	std::shared_ptr<MFR::SceneObject> rightWall = std::make_shared<MFR::SceneObject>("cube.obj");
	rightWall->setMaterialOnAll(greenMaterial);
	rightWall->scale = MFR::Point(0.2,10.0,10.0);
	rightWall->position = MFR::Point(5.0,0.0,0.0);
	rightWall->rotation = MFR::Point(0.0,90.0,0.0);
	scene->objects.push_back(rightWall);
	
	//top wall
	std::shared_ptr<MFR::Material> whiteMaterial = std::make_shared<MFR::Material>(MFR::Color(1.0,1.0,1.0));
	std::shared_ptr<MFR::SceneObject> topWall = std::make_shared<MFR::SceneObject>("cube.obj");
	topWall->setMaterialOnAll(whiteMaterial);
	topWall->scale = MFR::Point(0.2,10.0,10.0);
	topWall->position = MFR::Point(0.0,5.0,0.0);
	topWall->rotation = MFR::Point(0.0,0.0,90.0);
	scene->objects.push_back(topWall);
	
	//bottom wall aka floor
	std::shared_ptr<MFR::SceneObject> bottomWall = std::make_shared<MFR::SceneObject>("cube.obj");
	bottomWall->setMaterialOnAll(whiteMaterial);
	bottomWall->scale = MFR::Point(0.2,10.0,10.0);
	bottomWall->position = MFR::Point(0.0,-5.0,0.0);
	bottomWall->rotation = MFR::Point(0.0,0.0,90.0);
	scene->objects.push_back(bottomWall);

	//back wall
	std::shared_ptr<MFR::SceneObject> backWall = std::make_shared<MFR::SceneObject>("cube.obj");
	backWall->setMaterialOnAll(whiteMaterial);
	backWall->scale = MFR::Point(0.2,10.0,10.0);
	backWall->position = MFR::Point(0.0,0.0,-5.0);
	backWall->rotation = MFR::Point(0.0,0.0,90.0);
	scene->objects.push_back(backWall);
	
	//front cube
	std::shared_ptr<MFR::SceneObject> frontCube = std::make_shared<MFR::SceneObject>("cube.obj");
	frontCube->setMaterialOnAll(whiteMaterial);
	frontCube->scale = MFR::Point(2.0,2.0,2.0);
	frontCube->position = MFR::Point(.2,-4.0,.2);
	frontCube->rotation = MFR::Point(30.0,0.0,0.0);
	scene->objects.push_back(frontCube);
	
	//back cube
	std::shared_ptr<MFR::SceneObject> backCube = std::make_shared<MFR::SceneObject>("cube.obj");
	backCube->setMaterialOnAll(whiteMaterial);
	backCube->scale = MFR::Point(2.0,4.0,1.0);
	backCube->position = MFR::Point(-2.0,-3.0,-2.0);
	backCube->scale = MFR::Point(16.0,0.0,0.0);
	scene->objects.push_back(backCube);
	
	std::shared_ptr<MFR::Camera> camera = std::make_shared<MFR::Camera>();
	camera->position = MFR::Point(0,1,6);
	camera->focalLength = -0.1;
	camera->fov = .436332;
	scene->camera = camera;
	
	std::shared_ptr<MFR::Light> light = std::make_shared<MFR::Light>();
	light->position = MFR::Point(0,5.0,0);
	scene->lights.push_back(light);
	
	return scene;
}

void calculateImage(MFR::Color * buf, int width, int height)
{
	for(int i = 0; i < width*height; i++)
	{
		buf[i] = MFR::Color(1.0,0.0,0.0);
	}
}

void writeImageToFile(MFR::Color * buf, char * filename, int width, int height)
{
   pngwriter png(width,height,0,filename);
   for(int i = 0; i < width*height; i++)
	{
     	png.plot(i%width,i/width, buf[i].r, buf[i].g, buf[i].b);
	}
   png.close();
}

SDL_Texture * getImageTexture(char * filename, SDL_Window * win, SDL_Renderer * ren)
{
	SDL_Surface *bmp = SDL_LoadBMP(filename);
	if (bmp == nullptr){
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return nullptr;
	}
	
	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
	SDL_FreeSurface(bmp);
	if (tex == nullptr){
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return nullptr;
	}
	return tex;
}

SDL_Texture * getBlankTexture(SDL_Window * win, SDL_Renderer * ren, int width, int height)
{

 	//The final texture
	SDL_Texture* tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);

	if (tex == nullptr){
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return nullptr;
	}
	return tex;
}

int clamp(int v, int lo, int hi)
{
	if(v<lo) return lo;
	if(v>hi) return hi;
	return v;
}
void writeColorBufferToTexture(SDL_Texture * tex, MFR::Color * buf)
{

	Uint32* pixels = nullptr;
	int pitch = 0;
	Uint32 format;
	
	// Get the size of the texture.
	int w, h;
	SDL_QueryTexture(tex, &format, nullptr, &w, &h);
	
	// Now let's make our "pixels" pointer point to the texture data.
	//will return zero if success, another number on failure
	if (SDL_LockTexture(tex, nullptr, (void**)&pixels, &pitch))
	{
		std::cout << "Locking texture failed: " << SDL_GetError() << std::endl;
	}
	
	MFR::Color * ptr = buf;
	//copy over the pixel buffer
	for(int i = 0; i < w*h; i++)
	{
		//std::cout << "buf i " << *ptr << std::endl;
		Uint32 color = 0;
		/*
		Uint32 r = clamp(buf[i].r,0,1)*255;
		Uint32 g = clamp(buf[i].g,0,1)*255;
		Uint32 b = clamp(buf[i].b,0,1)*255;*/
		Uint32 r = ptr->r*255;
		Uint32 g = ptr->g*255;
		Uint32 b = ptr->b*255;
		
		color = (r << 24) + (g << 16) + (b << 8) + 255;
		//color = (255 << 24) + (0 << 16) + (255 << 8) + 255;

		pixels[i] =color;
		ptr++;
	}
	
	SDL_UnlockTexture(tex);

}

int main(int argc, const char * argv[]) {
    std::cout << "Hello, World!\n" << std::endl;

	int width = 60;
	int height = 60;

	/* RENDER IMAGE TO SCREEN */
	
	//Start SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	
	//Set up screen
	SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, width, height, SDL_WINDOW_SHOWN);
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
	
	//initialize scene
	std::shared_ptr<MFR::Scene> cornellBox = makeCornellBoxScene();
	
	//initialize pixel buffer
	MFR::Color * pixels = new MFR::Color[width*height];
//	calculateImage(pixels, width, height);
	MFR::Raycaster::RenderImage(cornellBox, pixels, width, height, 0);
	
	SDL_Texture * tex = getBlankTexture(win, ren, width, height);
	writeColorBufferToTexture(tex,pixels);

	writeImageToFile(pixels, (char *)"test.png", width, height);
	
	
	
	//SDL_Texture * unusedTex = getImageTexture("hello_world.bmp", win, ren);

	
	//Main loop flag
	bool quit = false;
	
	//Event handler
	SDL_Event e;
	
	while(!quit)
	{
		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
			if( e.type == SDL_QUIT )
			{
				quit = true;
			}
		}
			
		//First clear the renderer
		SDL_RenderClear(ren);
		//Draw the texture
		SDL_RenderCopy(ren, tex, NULL, NULL);
		//Update the screen
		SDL_RenderPresent(ren);
		//Take a quick break after all that hard work
		SDL_Delay(1000);
		
	}
	
	delete pixels;
	
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	
	//http://www.willusher.io/sdl2%20tutorials/2013/08/17/lesson-1-hello-world
	//Quit SDL
	SDL_Quit();

    return 0;
}

/*TODO:
	
Figure out how to load scenes in order to get camera, light, material, and mesh info
And what format to use?
	-- first just bring objects in, make a reader for the .obj format
	-- then just put lights and camera in manually in the code, do the rendering logic and figure out shit for that later

Implement all the rendering logic 

Figure out how to scale and rotate the objects

Figure out how to do the memory stuff

*/
