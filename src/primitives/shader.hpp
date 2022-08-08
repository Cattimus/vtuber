#pragma once

#include "project_includes.hpp"

class Shader
{
private:
	unsigned int id; //unique ID given to shader by opengl
	bool valid; //flag to detect if something has gone wrong during shader compilation

	unsigned int compile_shader(const char* shader, GLenum type)
	{
		//compile shader
		unsigned int shader_id = glCreateShader(type);
		glShaderSource(shader_id, 1, &shader, NULL);
		glCompileShader(shader_id);

		//check shader compilation
		int success;
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

		//print errors if compilation fails
		if (!success)
		{
			char log[1024];
			glGetShaderInfoLog(shader_id, 1024, NULL, log);

			//helps to understand where the error is occuring
			if(type == GL_VERTEX_SHADER)
			{
				std::cout << "vertex ";
			}
			else
			{
				std::cout << "fragment ";
			}
			std::cout << "shader compilation failed!\n" << log << std::endl;
			return 0;
		}

		return shader_id;
	}

public:
	Shader(std::string vert, std::string frag)
	{
		unsigned int vert_id;
		unsigned int frag_id;

		std::stringstream vert_text;
		std::stringstream frag_text;

		//read files to strings
		try
		{
			std::ifstream vert_stream(vert);
			std::ifstream frag_stream(frag);

			vert_text << vert_stream.rdbuf();
			frag_text << frag_stream.rdbuf();

			vert_stream.close();
			frag_stream.close();
		}

		//failure to read files
		catch (const std::ifstream::failure& e)
		{
			std::cout << "Shader: Failure to open/read file - " << e.what() << std::endl;
			id = 0;
			valid = false;
			return;
		}
		
		valid = true;

		//compile fragment and vertex shader
		vert_id = compile_shader(vert_text.str().c_str(), GL_VERTEX_SHADER);
		frag_id = compile_shader(frag_text.str().c_str(), GL_FRAGMENT_SHADER);

		//compile shader program
		id = glCreateProgram();
		glAttachShader(id, vert_id);
		glAttachShader(id, frag_id);
		glLinkProgram(id);

		//check compilation success
		int success;
		glGetProgramiv(id, GL_LINK_STATUS, &success);

		if (!success)
		{
			char log[1024];
			glGetProgramInfoLog(id, 1024, NULL, log);
			std::cout << "Shader: failed to link shader.\n" << log << std::endl;
			valid = false;
		}

		//free resources
		glDeleteShader(vert_id);
		glDeleteShader(frag_id);
	}

	//free resources
	~Shader()
	{
		glDeleteProgram(id);
	}

	//use shader for rendering
	void use()
	{
		glUseProgram(id);
	}

	void set_1i(std::string name, int value)
	{
		glUniform1i(glGetUniformLocation(id, name.c_str()), value);
	}
	void set_1f(std::string name, float value)
	{
		glUniform1f(glGetUniformLocation(id, name.c_str()), value);
	}
	void set_vec3f(std::string name, float x, float y, float z)
	{
		glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
	}
	void set_mat4f(std::string name, glm::mat4 &data)
	{
		glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &data[0][0]);
	}
};
