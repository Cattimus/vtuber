#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "globals.hpp"

class Texture
{
private:
	SDL_Texture* texture;
	std::string file_path;
	int w;
	int h;

public:
	Texture(const char* path)
	{
		this->file_path = path;
	
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

	SDL_Texture* get_tex()
	{
		return texture;
	}
};
