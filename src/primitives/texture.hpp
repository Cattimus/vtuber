#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "project_includes.hpp"

class Texture
{
private:
	unsigned int id;
	int width;
	int height;
	int channels;

public:
	Texture(std::string filename, GLenum type)
	{
		//create new texture object
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		//set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		if(type == GL_NEAREST)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		//load image
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);

		if(!data)
		{
			std::cout << "Texture: Failed to load image.\n";
			return;
		}

		//generate texture
		if(channels == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else if(channels == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Texture: input image has an unsupported number of channels.\n";
			stbi_image_free(data);
			return;
		}
		
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}

	~Texture()
	{
		glDeleteTextures(1, &id);
	}

	void use()
	{
		glBindTexture(GL_TEXTURE_2D, id);
	}

	float aspect_ratio()
	{
		return (float)width / height;
	}

	double get_width()
	{
		return width;
	}

	double get_height()
	{
		return height;
	}

	double get_channels()
	{
		return channels;
	}
};
