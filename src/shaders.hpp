#pragma once

//Shaders will continue to be stored in files for debugging purposes

#include "primitives/shader.hpp"
#include "project_includes.hpp"

Shader* image_shader;
Shader* color_shader;
double* delta;

//this should only be called from main after opengl has loaded
void init_shaders()
{
	image_shader = new Shader("../shaders/basic_texture.vert", "../shaders/basic_texture.frag");
	color_shader = new Shader("../shaders/solid_color.vert", "../shaders/solid_color.frag");
}
