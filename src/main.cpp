// OpenGL includes
#include <glad/glad.h>
#include <SDL.h>
#include <SDL_image.h>

// local file includes
#include "primitives/texture.hpp"
#include "primitives/object.hpp"
#include "primitives/avatar.hpp"
#include "primitives/entity.hpp"
#include "user_input/mic_input.hpp"

//lua bindings for object
#include "lua_bindings/object_binding.hpp"
#include "lua_bindings/script.hpp"

// TODO - hot swap image while program is running

// screen size defaults
const unsigned int DEFAULT_WIDTH = 960;
const unsigned int DEFAULT_HEIGHT = 540;
double delta_val = 0;

Avatar* player; // avatar that the user will be controlling
Texture* player_tex;
Object* selected_object; // currently clicked on object

lua_State* L;

double cursor_offset_x;
double cursor_offset_y;

extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>

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
}

bool init_sdl()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Failed to init SDL" << std::endl;
		return false;
	}

	int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
	if(IMG_Init(img_flags) != img_flags)
	{
		std::cout << "SDL_Image init failed" << std::endl;
		return false;
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

	//create window
	window = SDL_CreateWindow("Cattimus' Vtuber Simulator",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		DEFAULT_WIDTH, DEFAULT_HEIGHT,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if(!window)
	{
		std::cout << "SDL window creation failed" << std::endl;
		return false;
	}
	screen = SDL_GetWindowSurface(window);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(!renderer)
	{
		std::cout << "SDL Renderer creation failed" << std::endl;
		return false;
	}
	SDL_SetRenderDrawColor(renderer, 51, 51, 77, 0xFF);

	return true;
}

void init_lua()
{
	L = lua_open();
	luaL_openlibs(L);

	lua_bindings::luaopen_object(L);

	//TODO - in the future this will be wrapped in an "engine" module
	lua_pushcfunction(L, get_player);
	lua_setglobal(L, "get_player");
}

void quit_sdl()
{
	if(window)
	{
		SDL_DestroyWindow(window);
		window = NULL;
	}

	if(renderer)
	{
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}

	SDL_Quit();
	IMG_Quit();
}

void handle_input(bool& running)
{
	SDL_Event e;
	while(SDL_PollEvent(&e) != 0)
	{
		switch(e.type)
		{
			case SDL_QUIT:
			{
				running = false;
			}

			case SDL_MOUSEBUTTONDOWN:
			{
				auto x = e.button.x;
				auto y = e.button.y;
				// split player at y value
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

					// TODO - check if any entities have been clicked on
					if (player->clicked(x, y))
					{
						selected_object = player;
					}
				}
			}

			case SDL_MOUSEBUTTONUP:
			{
				if(e.button.button == SDL_BUTTON_LEFT && e.button.state == SDL_RELEASED)
				{
					selected_object = NULL;
				}
			}

			case SDL_MOUSEMOTION:
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
		}
	}
}

int main()
{
	auto result = init_sdl();
	init_lua();
	if(!result)
	{
		return 0;
	}

	delta = &delta_val;
	player_tex = new Texture("../assets/catt_transparent.png");
	player = new Avatar((SDL_Rect){(int)(screen_height * 0.25),
								   (int)(screen_height * 0.25), 
								   (int)(screen_width  * 0.75), 
								   (int)(screen_height * 0.75)}, 
								   player_tex);

	// audio input from microphone
	mic_input voice;

	bool running = true;
	
	//run lua test script to see if it works
	luaL_loadfile(L, "../src/scripts/test.lua");
	if(lua_pcall(L, 0, 0, 0) != 0)
	{
		std::cout << "error running file test.lua: " << lua_tostring(L, -1) << std::endl;
		running = false;
	}

	Script test("../src/scripts/loaded_test.lua", L);

	lua_getglobal(L, "check_sandbox");
	if(lua_pcall(L, 0, 0, 0) != 0)
	{
		std::cout << "error running function check_sandbox: " << lua_tostring(L, -1) << std::endl;
		running = false;
	}
	
	while(running)
	{
		handle_input(running);

		SDL_RenderClear(renderer);
		if (player)
		{
			// draw objects with textures
			player->talk(voice.get());
			player->draw();
		}
		SDL_RenderPresent(renderer);
	}

	// clean up memory
	delete player;
	delete player_tex;

	quit_sdl();
	lua_close(L);
	return 0;
}