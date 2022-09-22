// OpenGL includes
#include <glad/glad.h>
#include <SDL.h>
#include <SDL_image.h>

//C++ includes
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

// local file includes
#include "primitives/texture.hpp"
#include "primitives/object.hpp"
#include "primitives/avatar.hpp"
#include "primitives/entity.hpp"
#include "user_input/mic.hpp"

//lua bindings for objects
#include "lua_bindings/object_binding.hpp"
#include "lua_bindings/script.hpp"

//lua global functions and lua includes
extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

// screen size defaults
const unsigned int DEFAULT_WIDTH = 960;
const unsigned int DEFAULT_HEIGHT = 540;
double delta_val = 0;

double cursor_offset_x;
double cursor_offset_y;

Avatar* player; 		 // avatar that the user will be controlling
Object* selected_object; // currently clicked on object
lua_State* L;

//rendering vectors
vector<Object*> draw_list; //list of drawable objects
vector<Avatar> avatars;    //all instances of avatar
vector<Entity> entities;   //all instances of entity
vector<Script> scripts;	   //all instances of script
vector<Texture> textures;  //all instances of texture

//function declarations
bool init_sdl();
void init_lua();
void quit_sdl();
void sort_drawlist();
void handle_input(bool& running);

extern "C"
{
	static int get_player(lua_State* L);
}

// TODO - make an "engine" api for lua functions
int main()
{
	if(!init_sdl()) {return 0;}
	delta = &delta_val;
	init_lua();

	//load textures
	textures.push_back(Texture("../assets/catt_transparent.png"));
	textures.push_back(Texture("../assets/tophat.png"));
	textures.push_back(Texture("../assets/crep.png"));

	//player object
	avatars.push_back(
			Avatar((SDL_Rect){
				(int)(screen_height * 0.25),
				(int)(screen_height * 0.25), 
				(int)(screen_width  * 0.75), 
				(int)(screen_height * 0.75)},
			&textures[0]));
	player = &avatars[0];
	
	//hat object
	entities.push_back(Entity(500, 200, &textures[1]));
	auto hat = &entities[0];
	hat->clamp_to(player->get_top());
	hat->set_offset(175, -70);
	hat->flip_horizontal();

	//test cat object
	entities.push_back(Entity(500, 300, &textures[2]));	
			
	// audio input from microphone
	mic voice;
	
	//load test scripts
	scripts.push_back(Script("../src/scripts/test.lua", L));
	player->set_script(&scripts[0]);
	avatars[0].set_priority(1);
	entities[0].set_priority(2);
	entities[1].set_priority(0);

	//set drawable objects
	for(size_t i = 0; i < avatars.size(); i++)
	{
		draw_list.push_back((Object*)&avatars[i]);
	}

	for(size_t i = 0; i < entities.size(); i++)
	{
		draw_list.push_back((Object*)&entities[i]);
	}
	sort_drawlist();

	//main loop
	bool running = true;
	while(running)
	{
		SDL_RenderClear(renderer);
		handle_input(running);
		
		//perform talk operation for every avatar
		double db = voice.get();
		for(size_t i = 0; i < avatars.size(); i++)
		{
			avatars[i].talk(db);
		}

		//draw all drawable objects
		for(size_t i = 0; i < draw_list.size(); i++)
		{
			draw_list[i]->draw();
		}
		SDL_RenderPresent(renderer);
	}

	//clean up libraries
	quit_sdl();
	lua_close(L);
	return 0;
}

//initialize sdl values and return the result
bool init_sdl()
{
	//init SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "Failed to init SDL" << endl;
		return false;
	}
	
	//init SDL_IMG
	int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
	if(IMG_Init(img_flags) != img_flags)
	{
		cout << "SDL_Image init failed" << endl;
		return false;
	}

	//create window
	window = SDL_CreateWindow(
		"Cattimus' Vtuber Simulator",
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED,
		DEFAULT_WIDTH,
		DEFAULT_HEIGHT,
		SDL_WINDOW_SHOWN);

	if(!window)
	{
		std::cout << "SDL window creation failed" << std::endl;
		return false;
	}

	//create screen and renderer
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1"); //enable vsync
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); //bilinear scaling
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(!renderer)
	{
		std::cout << "SDL Renderer creation failed" << std::endl;
		return false;
	}
	
	//initialize miscellaneous variables
	screen = SDL_GetWindowSurface(window);
	SDL_SetRenderDrawColor(renderer, 51, 51, 77, 0xFF);
	return true;
}

//initialize the lua environment for calling scripts
void init_lua()
{
	L = lua_open();
	luaL_openlibs(L);

	lua_bindings::luaopen_object(L);

	//TODO - in the future this will be wrapped in an "engine" module along with other functions like it
	lua_pushcfunction(L, get_player);
	lua_setglobal(L, "get_player");
}

//exit SDL and clean up values
void quit_sdl()
{
	if(renderer)
	{
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}

	if(window)
	{
		SDL_DestroyWindow(window);
		window = NULL;
	}

	IMG_Quit();
	SDL_Quit();
}

//handle mouse button down event (helper for handle input)
static void handle_mousedown(SDL_Event &e)
{
	auto x = e.button.x;
	auto y = e.button.y;

	// split avatar at y value. player has left clicked the player object
	if (e.button.button == SDL_BUTTON_RIGHT && e.button.state == SDL_PRESSED)
	{
		if (player->clicked(x, y))
		{
			player->avatar_split(y);
		}
	}

	// move player/object
	if (e.button.button == SDL_BUTTON_LEFT && e.button.state == SDL_PRESSED)
	{
		cursor_offset_x = x;
		cursor_offset_y = y;

		//drag any object on the screen that has been clicked	
		for(auto obj : draw_list)
		{
			if(obj->clicked(x,y))
			{
				selected_object = obj;
				last_selected = obj;
			}
		}
	}
}

//handle mouse button up event (helper for handle input)
static void handle_mouseup(SDL_Event &e)
{	
	//reset selected_object when mouse button has been released
	if(e.button.button == SDL_BUTTON_LEFT && e.button.state == SDL_RELEASED)
	{
		selected_object = NULL;
	}
}

//handle mouse motion event (helper for handle input)
static void handle_mousemotion(SDL_Event &e)
{
	if (selected_object)
	{
		auto x = e.button.x;
		auto y = e.button.y;

		selected_object->relative_move(x - cursor_offset_x, y - cursor_offset_y);
		cursor_offset_x = x;
		cursor_offset_y = y;
	}	
}

//helper function for handling key input (helper for handle input)
static void handle_keyinput(SDL_Event &e)
{
	switch(e.key.keysym.sym)
	{
		case SDLK_ESCAPE:
			last_selected = NULL;
		break;

		case SDLK_r:
			if(last_selected)
			{
				last_selected->change_texture(&textures[0]);
			}
		break;

		case SDLK_EQUALS:
			if(last_selected)
			{	
				last_selected->inc_priority();
				sort_drawlist();
			}
		break;

		case SDLK_MINUS:
			if(last_selected)
			{
				last_selected->dec_priority();
				sort_drawlist();
			}
		break;
	}
}

//helper function to handle user input
void handle_input(bool& running)
{
	SDL_Event e;
	while(SDL_PollEvent(&e) != 0)
	{

	switch(e.type)
	{
		case SDL_QUIT:
			running = false;
		break;
		case SDL_MOUSEBUTTONDOWN:
			handle_mousedown(e);
		break;
		case SDL_MOUSEBUTTONUP:
			handle_mouseup(e);
		break;
		case SDL_MOUSEMOTION:
			handle_mousemotion(e);
		break;
		case SDL_KEYDOWN:
			handle_keyinput(e);
		break;
	}
	}
}

//return a reference to the player object for manipulation by lua scripts
static int get_player(lua_State* L)
{
	//don't return a reference if player is not initialized
	if(!player)
	{
		return 0;
	}

	//give lua the player as converted to an object.
	lua_bindings::create_object(L, (Object*)player);
	return 1;
}

static bool comp(Object* a, Object* b)
{
	return a->get_priority() < b->get_priority();
}

//sort elements in the draw list by priority
void sort_drawlist()
{
	sort(draw_list.begin(), draw_list.end(), comp);
}
