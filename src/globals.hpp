#pragma once

//standard library includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

//project includes
#include "primitives/object.hpp"

#include<SDL.h>
#include<SDL_image.h>

double* delta;
Object* last_selected = NULL;
SDL_Window* window;
SDL_Surface* screen;
SDL_Renderer* renderer;
int screen_width = 960;
int screen_height = 540;
