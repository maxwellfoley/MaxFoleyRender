//
//  main.c
//  MaxFoleyRender
//
//  Created by Maxwell Foley on 11/26/17.
//  Copyright © 2017 Maxwell Foley. All rights reserved.
//

#include <stdio.h>
#include "Vector.h"
#include "Color.h"
#include "SDL2/SDL.h"
#include <pngwriter.h>

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
     	png.plot(i%width,i/width, buf[i].r*255, buf[i].g*255, buf[i].b*255);
	}
     std::cout << " just making sure we actually did it " << std::endl;
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

SDL_Texture * getBlankTexture(SDL_Window * win, SDL_Renderer * ren)
{

 	//The final texture
	SDL_Texture* tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 640, 480);

	if (tex == nullptr){
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return nullptr;
	}
	return tex;
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
	
	//copy over the pixel buffer
	for(int i = 0; i < w*h; i++)
	{
		std::cout << i <<  sizeof(pixels[i]) << std::endl;
		
		Uint32 color = 0;
		Uint32 r = buf[i].r*255;
		Uint32 g = buf[i].g*255;
		Uint32 b = buf[i].b*255;
		Uint32 a = buf[i].a*255;
		color = (r << 24) + (g << 16) + (b << 8) + a;
		pixels[i] = color;
	}
	
	SDL_UnlockTexture(tex);

}

int main(int argc, const char * argv[]) {
    printf("Hello, World!\n");

	int width = 640;
	int height = 480;

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
	
	//initialize pixel buffer
	MFR::Color * pixels = new MFR::Color[width*height];
	calculateImage(pixels, width, height);
	
	writeImageToFile(pixels, (char *)"test.png", width, height);
	
	SDL_Texture * tex = getBlankTexture(win, ren);
	writeColorBufferToTexture(tex,pixels);
	
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

Implement all the rendering logic 

Figure out how to do the memory stuff

*/
