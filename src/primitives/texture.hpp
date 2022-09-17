#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "globals.hpp"

class Texture
{
private:
	SDL_Texture* texture;
	int w;
	int h;

public:
	Texture(const char* path)
	{
		//load image from file
		SDL_Surface* img_temp = IMG_Load(path);
		if(!img_temp)
		{
			printf("Failed to load image for texture\n");
		}
		w = img_temp->w;
		h = img_temp->h;

		//create texture from the loaded surface
		texture = SDL_CreateTextureFromSurface(renderer, img_temp);
		SDL_FreeSurface(img_temp);
	}

	int getw()
	{
		return w;
	}

	int geth()
	{
		return h;
	}

	//clean memory
	~Texture()
	{
		SDL_DestroyTexture(texture);
	}

	SDL_Texture* get_tex()
	{
		return texture;
	}
};
