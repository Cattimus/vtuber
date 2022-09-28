#pragma once

class State;
#include "primitives/object.hpp"
#include "primitives/entity.hpp"
#include "primitives/avatar.hpp"
#include "primitives/texture.hpp"

#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

#include <SDL.h>
#include <SDL_image.h>

//TODO - reclaim scripts when script api is more finalized

class State
{
private:
	//memory reclaim functions
	void reclaim_textures(string path);	

	//search functions
	Texture* search_textures(string path);
	int tex_index(string path);

public:
	//vectors that will hold our state data
	vector<Avatar>  avatars;   //all instances of avatar
	vector<Entity>  entities;  //all instances of entity
	vector<Script>  scripts;   //all instances of script
	vector<Texture> textures;  //all instances of texture
	vector<Object*> draw_list; //list of drawable objects

	Object* last_selected;
	double* delta;
	SDL_Renderer* renderer;
	
	//constructors and assignment operator
	State();
	State(const State& to_copy);
	State& operator=(const State& to_copy);

	//object create functions
	void new_entity(int w, int h, int x, int y, int priority, string tex_path);
	void new_avatar(int w, int h, int x, int y, int priority, string tex_path);

	//object delete functions
	void delete_entity(Object* ref);
	void delete_texture(string path);
	void delete_avatar(Object* ref);

	//sort functions
	void sort_drawlist();
};

