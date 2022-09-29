// OpenGL includes
#include <glad/glad.h>
#include <SDL.h>
#include <SDL_image.h>

//C++ includes
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

//user input objects
#include "user_input/mic.hpp"
#include "user_input/console.hpp"

//lua bindings for objects
#include "lua_bindings/object_binding.hpp"
#include "lua_bindings/script.hpp"

//primitive objects
#include "primitives/object.hpp"
#include "primitives/avatar.hpp"
#include "primitives/entity.hpp"
#include "primitives/texture.hpp"

#include "state.hpp"

//lua includes
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
int screen_height = DEFAULT_HEIGHT;
int screen_width = DEFAULT_WIDTH;

double cursor_offset_x;
double cursor_offset_y;

Avatar* player; 		 // avatar that the user will be controlling
Entity* hat;
Object* selected_object; // currently clicked on object
lua_State* L;

SDL_Renderer* renderer;
SDL_Window* window;
SDL_Surface* screen;

State state;

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

int main()
{
	if(!init_sdl()) {return 0;}
	state.renderer = renderer;
	state.delta = &delta_val;
	init_lua();
	state.L = L;
	
	//new style player allocation
	player = state.new_avatar((screen_height * 0.25), (screen_height * 0.25),
							  (screen_width * 0.75), (screen_height * 0.75),
							  0,
							  "../assets/catt_transparent.png",
							  "../src/scripts/test.lua");
	
	//new style hat allocation
	hat = state.new_entity(0,0,500,200,1,
								   "../assets/tophat.png");

	hat->clamp_to(player->get_top());
	hat->set_offset(175, -70);
	hat->flip_horizontal();
			
	// audio input from microphone
	mic voice;

	//main loop
	bool running = true;
	while(running)
	{
		SDL_RenderClear(renderer);
		handle_input(running);
		
		//TODO - make wrappers for this
		//perform talk operation for every avatar
		double db = voice.get();
		for(size_t i = 0; i < state.avatars.size(); i++)
		{
			state.avatars[i]->talk(db);
		}

		//draw all drawable objects
		for(size_t i = 0; i < state.draw_list.size(); i++)
		{
			state.draw_list[i]->draw(renderer);
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
		for(size_t i = 0; i < state.draw_list.size(); i++)
		{
			Object* obj = state.draw_list[i];
			if(obj->clicked(x,y))
			{
				selected_object = obj;
				state.last_selected = obj;
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
			state.last_selected = NULL;
		break;

		case SDLK_EQUALS:
			if(state.last_selected)
			{	
				state.last_selected->inc_priority();
				state.sort_drawlist();
			}
		break;

		case SDLK_MINUS:
			if(state.last_selected)
			{
				state.last_selected->dec_priority();
				state.sort_drawlist();
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
