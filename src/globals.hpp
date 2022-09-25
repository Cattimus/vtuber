#pragma once

//standard library includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include<SDL.h>
#include<SDL_image.h>

//global primitive data
double* delta;
int screen_width = 960;
int screen_height = 540;

//global SDL data
SDL_Window* window;
SDL_Surface* screen;
SDL_Renderer* renderer;

//object that was last clicked on
#include "primitives/object.hpp"
Object* last_selected = NULL;

#include "primitives/avatar.hpp"
#include "primitives/entity.hpp"
#include "primitives/texture.hpp"
//vectors that will hold our state data
vector<Avatar> avatars;    //all instances of avatar
vector<Entity> entities;   //all instances of entity
vector<Script> scripts;	   //all instances of script
vector<Texture> textures;  //all instances of texture
vector<Object*> draw_list;

