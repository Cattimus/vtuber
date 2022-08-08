// OpenGL includes
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

void handle_keyboard_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void handle_mouse_input(GLFWwindow* window, int button, int action, int mods);
void handle_cursor_pos(GLFWwindow *window, double xpos, double ypos);

int main()
{
	// initialize glfw
	glfwInit();

	// targeting opengl 3.3 core profile (better efficiency)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// disable window resizing
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	// enable vsync
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

	// create window
	auto window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Cattimus weird vtuber thing", NULL, NULL);

	// initialize opengl
	glfwMakeContextCurrent(window);

	// load opengl extensions with glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetKeyCallback(window, handle_keyboard_input);
	glfwSetMouseButtonCallback(window, handle_mouse_input);
	glfwSetCursorPosCallback(window, handle_cursor_pos);

	// set initial size of virtual canvas
	glViewport(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);

	// enable transparent textures
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
	double last = glfwGetTime();

	// audio input from microphone
	mic_input voice;

	// perform opengl rendering
	while (!glfwWindowShouldClose(window))
	{
		double now = glfwGetTime();
		delta_val = now - last;
		last = now;

		// clear previous frame
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// perform user inputs
		glfwPollEvents();

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

		// push render to screen
		glfwSwapBuffers(window);
	}

	// clean up memory
	delete player;
	delete player_tex;
	delete image_shader;
	delete color_shader;

	// free GLFW memory
	glfwTerminate();

	return 0;
}

void handle_keyboard_input(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_R)
		{
			player->reset_position();
			player->set_origin(glm::vec3(0, -0.25, 0));
		}
	}
}

void handle_cursor_pos(GLFWwindow* window, double xpos, double ypos)
{
	if (selected_object)
	{
		double adjusted_xpos = xpos / (DEFAULT_WIDTH / (double)2) - 1;
		double adjusted_ypos = (ypos / (DEFAULT_HEIGHT / (double)2) - 1) * -1;

		selected_object->relative_move(adjusted_xpos - cursor_offset_x, adjusted_ypos - cursor_offset_y);
		cursor_offset_x = adjusted_xpos;
		cursor_offset_y = adjusted_ypos;
	}
}

void handle_mouse_input(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		// calculate where click is in opengl coordinates
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		double adjusted_xpos = xpos / (DEFAULT_WIDTH / (double)2) - 1;
		double adjusted_ypos = (ypos / (DEFAULT_HEIGHT / (double)2) - 1) * -1;

		// split player at y value
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			if (player->clicked(adjusted_xpos, adjusted_ypos))
			{
				player->split(adjusted_ypos);
			}
		}

		// move player/object
		if (button == GLFW_MOUSE_BUTTON_LEFT)
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

	else if (action == GLFW_RELEASE)
	{
		selected_object = NULL;
	}
}
