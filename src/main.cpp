// OpenGL includes
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// local file includes
#include "primitives/shader.hpp"
#include "primitives/texture.hpp"
#include "primitives/object.hpp"
#include "primitives/debug_line.hpp"
#include "primitives/avatar.hpp"
#include "primitives/entity.hpp"
#include "shaders.hpp"
#include "project_includes.hpp"
#include "user_input/mic_input.hpp"

// TODO - hot swap image while program is running

// screen size defaults
const unsigned int DEFAULT_WIDTH = 960;
const unsigned int DEFAULT_HEIGHT = 540;

int screen_width = 960;
int screen_height = 540;
double delta_val = 0;

glm::mat4 projection(1.0f);
glm::mat4 view(1.0f);

Avatar* player; // avatar that the user will be controlling
Texture* player_tex;
Object* selected_object; // currently clicked on object

double cursor_offset_x;
double cursor_offset_y;

SDL_Window* window = NULL;
SDL_GLContext context;

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

	//set opengl attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);

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

	//create openGL context
	context = SDL_GL_CreateContext(window);
	if(!context)
	{
		std::cout << "OpenGL context creation failed" << std::endl;
		return false;
	}

	// load opengl extensions with glad
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	// set initial size of virtual canvas
	glViewport(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);

	// enable transparent textures
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

void quit_sdl()
{
	if(window)
	{
		SDL_DestroyWindow(window);
		window = NULL;
	}

	SDL_Quit();
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
				// calculate where click is in opengl coordinates
				double adjusted_xpos = e.button.x / (DEFAULT_WIDTH / (double)2) - 1;
				double adjusted_ypos = (e.button.y / (DEFAULT_HEIGHT / (double)2) - 1) * -1;

				// split player at y value
				if (e.button.button == SDL_BUTTON_RIGHT && e.button.state == SDL_PRESSED)
				{
					if (player->clicked(adjusted_xpos, adjusted_ypos))
					{
						player->split(adjusted_ypos);
					}
				}

				// move player/object
				if (e.button.button == SDL_BUTTON_LEFT && e.button.state == SDL_PRESSED)
				{
					cursor_offset_x = adjusted_xpos;
					cursor_offset_y = adjusted_ypos;

					// TODO - check if any entities have been clicked on
					if (player->clicked(adjusted_xpos, adjusted_ypos))
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
					double adjusted_xpos = e.motion.x / (DEFAULT_WIDTH / (double)2) - 1;
					double adjusted_ypos = (e.motion.y / (DEFAULT_HEIGHT / (double)2) - 1) * -1;

					selected_object->relative_move(adjusted_xpos - cursor_offset_x, adjusted_ypos - cursor_offset_y);
					cursor_offset_x = adjusted_xpos;
					cursor_offset_y = adjusted_ypos;
				}
			}

			case SDL_KEYDOWN:
			{
				switch(e.key.keysym.sym)
				{
					case SDLK_r:
					{
						player->reset_position();
						player->set_origin(glm::vec3(0, -0.25, 0));
					}
				}
			}
		}
	}
}

int main()
{
	auto result = init_sdl();
	if(!result)
	{
		return 0;
	}

	// transform view
	view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.1f));

	// set projection to perspective matrix
	projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);

	// setup resources
	init_shaders();
	delta = &delta_val;
	player_tex = new Texture("../assets/catt_transparent.png", GL_LINEAR);
	player = new Avatar(glm::vec3(1.5, 1.5, 0), player_tex);
	player->set_origin(glm::vec3(0, -0.25, 0));

	// audio input from microphone
	mic_input voice;

	bool running = true;
	while(running)
	{
		handle_input(running);

		// clear previous frame
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (player)
		{
			// draw objects with textures
			image_shader->use();
			image_shader->set_mat4f("projection", projection);
			image_shader->set_mat4f("view", view);
			player->talk(voice.get());
			player->move();
			player->draw();
		}

		SDL_GL_SwapWindow(window);
	}

	// clean up memory
	delete player;
	delete player_tex;
	delete image_shader;
	delete color_shader;

	quit_sdl();
	return 0;
}