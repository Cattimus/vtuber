#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "globals.hpp"

class Texture
{
private:
	SDL_Texture* texture;

public:
	Texture(const char* path)
	{
		SDL_Surface* img_temp = IMG_Load(path);
		if(!img_temp)
		{
			printf("Failed to load image for texture\n");
		}

		texture = SDL_CreateTextureFromSurface(renderer, img_temp);
		SDL_FreeSurface(img_temp);
	}

	~Texture()
	{
		SDL_DestroyTexture(texture);
	}

	SDL_Texture* get_tex()
	{
		return texture;
	}


};
