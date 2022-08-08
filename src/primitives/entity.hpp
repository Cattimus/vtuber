#pragma once

#include "project_includes.hpp"
#include "object.hpp"
#include "texture.hpp"
#include "shader.hpp"

class Entity : public Object
{
protected:
    //references to texture and shader, can be reused for other objects
	Texture* texture;
	Shader* shader;

	//buffers to hold object data
	float vertices[12];
	float texture_coords[12];

	//opengl data
	unsigned int VAO;
	unsigned int VBO;
	unsigned int VBO_TEXTURE;

private:
    //set individual vertexes/texture coords
	void set_point(float* to_set, glm::vec3 point)
	{
		*to_set = point.x;
		*(to_set+1) = point.y;
	}

	//calculate triangles for square
	void calculate_vertices(double yoffset, int position)
	{
		//calculate proper offset positions
		if(position > 0)
		{
			size.y = (size.y - yoffset) / 2;
		}
		else if(position < 0)
		{
			size.y = (size.y + yoffset) / 2;
		}
		
		//calculate points we need
		glm::vec3 top_left(size.x * -1, size.y, 0);
		glm::vec3 top_right(size.x, size.y, 0);
		glm::vec3 bottom_left(size.x * -1, size.y * -1, 0);
		glm::vec3 bottom_right(size.x, size.y * -1, 0);

		//set left triangle
		set_point(vertices+0, top_left);
		set_point(vertices+2, bottom_left);
		set_point(vertices+4, bottom_right);

		//set right triangle
		set_point(vertices+6, top_right);
		set_point(vertices+8, bottom_right);
		set_point(vertices+10, top_left);
	}

	//yoffset is where to split the texture
	//position is positive for top, negative for bottom
	void calculate_texture(double yoffset, int position)
	{
		//default texture coordinates
		glm::vec3 top_left(0,1,0);
		glm::vec3 top_right(1,1,0);
		glm::vec3 bottom_left(0,0,0);
		glm::vec3 bottom_right(1,0,0);

		//calculate texture coords based on offset
		if(yoffset != 0)
		{
			if(position > 0)
			{
				bottom_left.y = yoffset;
				bottom_right.y = yoffset;
			}
			else if(position < 0)
			{
				top_left.y = yoffset;
				top_right.y = yoffset;
			}
		}

		//set left triangle
		set_point(texture_coords+0, top_left);
		set_point(texture_coords+2, bottom_left);
		set_point(texture_coords+4, bottom_right);

		//set right triangle
		set_point(texture_coords+6, top_right);
		set_point(texture_coords+8, bottom_right);
		set_point(texture_coords+10, top_left);
	}

	//update vertexes and texture coords after a split
	void update_buffers()
	{
		glBindVertexArray(VAO);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 12, vertices);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_TEXTURE);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 12, texture_coords);

		glBindVertexArray(0);
	}

	void init_buffers()
	{
		//initialize opengl objects
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &VBO_TEXTURE);

		//assign data to VAO
		glBindVertexArray(VAO);

		//bind buffers to GPU
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//assign shader attrib arrays
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_TEXTURE);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coords), texture_coords, GL_STATIC_DRAW);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

public:

	Entity(glm::vec3 size, Texture* texture) : Object(size)
	{	
		this->texture = texture;
		this->shader = image_shader;

		//assign vertex data
		calculate_vertices(0,0);
		calculate_texture(0,0);
		init_buffers();
	}

	//copy constructor
	Entity(const Entity& to_copy) : Object(to_copy)
	{
		*this = to_copy;
	}

	//assignment operator
	Entity& operator=(const Entity& to_copy)
	{
		if(this == &to_copy)
		{
			return *this;
		}

		Object::operator=(to_copy);
		this->texture = to_copy.texture;
		this->shader = to_copy.shader;

		//assign vertex data
		calculate_vertices(0,0);
		calculate_texture(0,0);
		init_buffers();

		return *this;
	}

	void split(double y, int position)
	{
		double relative_y = y - offset.y; //y relative to the origin
		
		double bottom_portion = relative_y + size.y; //portion of the object taken up by the bottom (split at y)
		double top_portion = (size.y * 2) - bottom_portion; //portion of the object taken up by the top (split at y)

		//new texture coordinate offset, will always use bottom portion
		double texture_offset = bottom_portion / (size.y * 2);

		calculate_vertices(relative_y, position);
		calculate_texture(texture_offset, position);
		update_buffers();

		//update origin to reflect slice
		if(position > 0)
		{
			origin.y += bottom_portion / 2;
		}

		if(position < 0)
		{
			origin.y -= top_portion / 2;
		}
	}

	//reset size and position
	void reset_size()
	{
		size = original_size;
		origin.x = 0;
		origin.y = 0;
		offset = origin;

		calculate_vertices(0, 0);
		calculate_texture(0, 0);
		update_buffers();
	}

	void draw()
	{
		update_position();

		texture->use();
		shader->set_mat4f("model", model);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	
};
