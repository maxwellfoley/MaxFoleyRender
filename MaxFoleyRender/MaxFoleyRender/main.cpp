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

//testing scene
std::shared_ptr<MFR::Scene> makeSingleCubeScene() {
	std::shared_ptr<MFR::Scene> scene = std::make_shared<MFR::Scene>();

	std::shared_ptr<MFR::Material> whiteMaterial = std::make_shared<MFR::Material>(MFR::Color(1.0,0.0,0.0));
	std::shared_ptr<MFR::SceneObject> cube = std::make_shared<MFR::SceneObject>("cube.obj");
	cube->setMaterialOnAll(whiteMaterial);
	cube->scale = MFR::Point(1.0,4.0,1.0);
	cube->position = MFR::Point(0.0,0.0,-3.0);
	cube->rotation = MFR::Point(0,0,0);
	scene->objects.push_back(cube);
	
	std::shared_ptr<MFR::Camera> camera = std::make_shared<MFR::Camera>();
	camera->position = MFR::Point(0,0,10);
	camera->focalLength = -0.1;
	camera->fov = .436332;
	scene->camera = camera;
	
	std::shared_ptr<MFR::Light> light = std::make_shared<MFR::Light>();
	light->position = MFR::Point(0,5.0,0);
	scene->lights.push_back(light);

	return scene;

}

//there is not yet any way to import scenes or a UI to create them, so I define the scene I want rendered in this function
std::shared_ptr<MFR::Scene> makeCornellBoxScene() {
	std::shared_ptr<MFR::Scene> scene = std::make_shared<MFR::Scene>();
	
		std::shared_ptr<MFR::Material> greenMaterial = std::make_shared<MFR::Material>(MFR::Color(0.0,1.0,0.0));
	std::shared_ptr<MFR::Material> redMaterial = std::make_shared<MFR::Material>(MFR::Color(1.0,0.0,0.0));

	
	//left wall is red
	std::shared_ptr<MFR::SceneObject> leftWall = std::make_shared<MFR::SceneObject>("cube.obj");
	leftWall->setMaterialOnAll(redMaterial);
	leftWall->scale = MFR::Point(0.2,10.0,10.0);
	leftWall->position = MFR::Point(-5.0,0.0,0.0);
	leftWall->rotation = MFR::Point(90.0,0.0,0.0);
	scene->objects.push_back(leftWall);

	//right wall is green
	std::shared_ptr<MFR::SceneObject> rightWall = std::make_shared<MFR::SceneObject>("cube.obj");
	rightWall->setMaterialOnAll(greenMaterial);
	rightWall->scale = MFR::Point(0.2,10.0,10.0);
	rightWall->position = MFR::Point(5.0,0.0,0.0);
	rightWall->rotation = MFR::Point(90.0,0.0,0.0);
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
	backWall->rotation = MFR::Point(0.0,90.0,0.0);
	scene->objects.push_back(backWall);
	
	//front cube
	std::shared_ptr<MFR::SceneObject> frontCube = std::make_shared<MFR::SceneObject>("cube.obj");
	frontCube->setMaterialOnAll(greenMaterial);
	frontCube->scale = MFR::Point(2.0,2.0,2.0);
	frontCube->position = MFR::Point(.2,-4.0,.2);
	frontCube->rotation = MFR::Point(0.0,30.0,0.0);
	scene->objects.push_back(frontCube);
	
	//back cube
	std::shared_ptr<MFR::SceneObject> backCube = std::make_shared<MFR::SceneObject>("cube.obj");
	backCube->setMaterialOnAll(redMaterial);
	backCube->scale = MFR::Point(2.0,4.0,1.0);
	backCube->position = MFR::Point(-2.0,-3.0,-2.0);
	backCube->rotation = MFR::Point(0.0,16.0,0.0);
	scene->objects.push_back(backCube);
	
	std::shared_ptr<MFR::Camera> camera = std::make_shared<MFR::Camera>();
	camera->position = MFR::Point(0,0,25);
	camera->focalLength = -0.1;
	camera->fov = .436332;
	scene->camera = camera;
	
	std::shared_ptr<MFR::Light> light = std::make_shared<MFR::Light>();
	light->position = MFR::Point(1.0,3.0,-2.0);
	light->shadowsEnabled = false;
	light->strength = 1.0;
	scene->lights.push_back(light);
	
	std::shared_ptr<MFR::Light> light2 = std::make_shared<MFR::Light>();
	light2->position = MFR::Point(0.0,2.0,0.0);
	light2->shadowsEnabled = false;
	light2->strength = 1.0;
	scene->lights.push_back(light2);
	
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
     	png.plot(i%width,height-i/width, buf[i].r, buf[i].g, buf[i].b);
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

void rayTriangleIntersectionUnitTests() {
	//seems good based on these two basic ones, can do more later
	MFR::Tri t1 = MFR::Tri(MFR::Point(-5,-5,-5),MFR::Point(5,-5,-5),MFR::Point(0,5,-5));
	MFR::Ray r1 = MFR::Ray(MFR::Point(0,0,0),MFR::Vector(0,0,-1));
	float b1[3];
	float dist1;
	bool h1 = t1.intersect(r1,b1,dist1);
	std::cout << "test 1 straight in front: " << h1 << " " << "b: " << b1[0] << " " << b1[1] << " " << b1[2] << " dist: " << dist1 << std::endl;
	
	MFR::Tri t2 = MFR::Tri(MFR::Point(-5,-5,-5),MFR::Point(5,-5,-5),MFR::Point(0,5,-5));
	MFR::Ray r2 = MFR::Ray(MFR::Point(0,0,0),MFR::Vector(0,1,0));
	float b2[3];
	float dist2;
	bool h2 = t2.intersect(r2,b2,dist2);
	std::cout << "test 2 straight in front but ray goes up: " << h2 << " " << "b: " << b2[0] << " " << b2[1] << " " << b2[2] << " dist: " << dist2 << std::endl;
	
	MFR::Tri t3 = MFR::Tri(MFR::Point(-5,-5,0),MFR::Point(5,-5,0),MFR::Point(0,5,0));
	MFR::Ray r3 = MFR::Ray(MFR::Point(100,200,300),MFR::Vector(-1,-2,-3));
	float b3[3];
	float dist3;
	bool h3 = t1.intersect(r3,b3,dist3);
	std::cout << "test 3 ray comes from afar and hits origin: " << h3 << " " << "b: " << b3[0] << " " << b3[1] << " " << b3[2] << " dist: " << dist3 << std::endl;
	
	MFR::Tri t4 = MFR::Tri(MFR::Point(-5,-5,0),MFR::Point(5,-5,0),MFR::Point(0,5,0));
	MFR::Ray r4 = MFR::Ray(MFR::Point(100,300,300),MFR::Vector(-1,-2,-3));
	float b4[3];
	float dist4;
	bool h4 = t1.intersect(r4,b4,dist4);
	std::cout << "test 4 like the last one but it misses: " << h4 << " " << "b: " << b4[0] << " " << b4[1] << " " << b4[2] << " dist: " << dist4 << std::endl;
}

void rayBoxIntersectionUnitTests() {
	//all this shit seems to work as far as i can tell
	
	MFR::Box box1 = MFR::Box(MFR::Point(-10.0,-10.0,-10.0),20,20,5);
	MFR::Ray r1 = MFR::Ray(MFR::Point(0,0,0),MFR::Vector(0,0,-1));
	bool h1 = box1.intersect(r1);
	std::cout << "test 1 simple hit: " << h1 << std::endl;
	
	MFR::Box box2 = MFR::Box(MFR::Point(-10.0,-10.0,-10.0),20,20,5);
	MFR::Ray r2 = MFR::Ray(MFR::Point(0,0,0),MFR::Vector(0,-1,1));
	bool h2 = box2.intersect(r2);
	std::cout << "test 2 simple miss: " << h2 << std::endl;
	
	MFR::Box box3 = MFR::Box(MFR::Point(-30.0,-30.0,-30.0),60,60,60);
	MFR::Ray r3 = MFR::Ray(MFR::Point(0,0,0),MFR::Vector(0,-1,1));
	bool h3 = box3.intersect(r3);
	std::cout << "test 3 from tha inside: " << h3 << std::endl;

	MFR::Box box4 = MFR::Box(MFR::Point(-30.0,-30.0,-30.0),60,60,60);
	MFR::Ray r4 = MFR::Ray(MFR::Point(1,2,3),MFR::Vector(.4,1,-1));
	bool h4 = box4.intersect(r4);
	std::cout << "test 4 from tha inside another: " << h4 << std::endl;
	
	MFR::Box box5 = MFR::Box(MFR::Point(-1.0,-1.0,-1.0),2,2,2);
	MFR::Ray r5 = MFR::Ray(MFR::Point(100,200,300),MFR::Vector(-1,-2,-3));
	bool h5 = box5.intersect(r5);
	std::cout << "test 5 coming from an angle to hit the tiny box at the center: " << h5 << std::endl;
	
	MFR::Box box6 = MFR::Box(MFR::Point(-1.0,-1.0,-1.0),2,2,2);
	MFR::Ray r6 = MFR::Ray(MFR::Point(100,200,300),MFR::Vector(-1,-4,-3));
	bool h6 = box5.intersect(r6);
	std::cout << "test 6 similar to the last one but it should fail: " << h6 << std::endl;

	MFR::Box box7 = MFR::Box(MFR::Point(-1000.0,-1000.0,-1000.0),2000,2000,2000);
	MFR::Ray r7 = MFR::Ray(MFR::Point(-1000,-1005,-1010),MFR::Vector(1,-6,-11));
	bool h7 = box7.intersect(r7);
	std::cout << "test 7 barely missing a huge box: " << h7 << std::endl;

	MFR::Box box8 = MFR::Box(MFR::Point(-5.1,-5.1,-5.1),10.201,10.201,10.201);
	MFR::Ray r8 = MFR::Ray(MFR::Point(0, 0, 25),MFR::Vector(-0.131728, 0.0439092, -0.990313));
	bool h8 = box8.intersect(r8);
	std::cout << "test 8 this is the one that fails in the tree: " << h8 << std::endl;
	//this is where its all fucked up
}

void triangleBoundingBoxUnitTests() {
	MFR::Tri t1 = MFR::Tri(MFR::Point(-5,-5,-5),MFR::Point(5,-5,-5),MFR::Point(0,5,-5));
	MFR::Box b1 = t1.getBoundingBox();
	std::cout << "test1 " << b1 << std::endl;

}

int main(int argc, const char * argv[]) {
	std::cout << "threads: " << std::thread::hardware_concurrency() << std::endl;
	
	//ok time to do some unit tests of my ray triangle intersection rn
	rayBoxIntersectionUnitTests();
	//return 0;
	
	int width = 500;
	int height = 500;

	/* RENDER IMAGE TO SCREEN */
	
	//Start SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	
	//Set up screen
	SDL_Window *win = SDL_CreateWindow("Max Foley Render!", 100, 100, width, height, SDL_WINDOW_SHOWN);
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
	cornellBox->poseMesh();	

	//initialize pixel buffer
	MFR::Color * pixels = new MFR::Color[width*height];
	MFR::RaycasterOptions options;
	options.width = width;
	options.height = height;
	options.numRays = 1;
	options.numScatteringEvents = 7;
	MFR::Raycaster::RenderImage(cornellBox, pixels, options);
	
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
	
	delete[] pixels;
	
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	
	//http://www.willusher.io/sdl2%20tutorials/2013/08/17/lesson-1-hello-world
	//Quit SDL
	SDL_Quit();

    return 0;
}

/*TODO:

figure out why lighting isnt working
*/
