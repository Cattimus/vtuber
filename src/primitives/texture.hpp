#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
using namespace std;

class Texture
{
private:
	SDL_Texture* texture;
	string file_path;
	int w;
	int h;

public:
	Texture(const char* path, SDL_Renderer* renderer)
	{
		this->file_path = path;
	
		//load image from file
		SDL_Surface* img_temp = IMG_Load(path);
		if(!img_temp)
		{
			cout << "failed to load image for texture\n";	
		}
		w = img_temp->w;
		h = img_temp->h;

		//create texture from the loaded surface
		texture = SDL_CreateTextureFromSurface(renderer, img_temp);
		SDL_FreeSurface(img_temp);
	}

	Texture(const Texture& to_copy)
	{
		*this = to_copy;
	}

	Texture& operator=(const Texture& to_copy)
	{
		if(this == &to_copy)
		{
			return *this;
		}

		texture = to_copy.texture;
		file_path = to_copy.file_path;
		w = to_copy.w;
		h = to_copy.h;
		return *this;
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

	string get_path()
	{
		return file_path;
	}
};
