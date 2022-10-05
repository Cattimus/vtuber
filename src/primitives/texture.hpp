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
	//constructors and operator overloads
	Texture(const char* path, SDL_Renderer* renderer);
	Texture(const Texture& to_copy);
	Texture& operator=(const Texture& to_copy);
	
	//getters
	int getw() {return w;}
	int geth() {return h;}
	string get_path() {return file_path;}
	SDL_Texture* get_tex() {return texture;}
};
