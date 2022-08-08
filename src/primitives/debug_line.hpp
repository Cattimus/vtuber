#pragma once

#include "shaders.hpp"
#include "project_includes.hpp"

class Debug_Line
{
private:
	Shader* shader;
	
	//attributes
	glm::vec3 start_pos;
	glm::vec3 end_pos;
	glm::vec3 color;
	glm::vec3 offset;
	glm::mat4 model;
	bool has_moved;

	//opengl data
	unsigned int VAO;
	unsigned int VBO;

	void update_position()
	{
		model = glm::translate(glm::mat4(1.0f), (end_pos - start_pos) + offset);
	}

	void update_line()
	{
		float vertices[4];
		vertices[0] = start_pos.x;
		vertices[1] = start_pos.y;
		vertices[2] = end_pos.x;
		vertices[3] = end_pos.y;

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 4, vertices);
	}

public:
	Debug_Line(glm::vec3 start_pos, glm::vec3 end_pos, glm::vec3 color)
	{
		this->start_pos = start_pos;
		this->end_pos = end_pos;
		this->color = color;
		this->model = glm::mat4(1.0f);
		this->has_moved = false;
		this->shader = color_shader;
		
		this->offset.x = 0;
		this->offset.y = 0;
		this->offset.z = 0;

		//assign vertex data
		float vertices[4];
		vertices[0] = start_pos.x;
		vertices[1] = start_pos.y;
		vertices[2] = end_pos.x;
		vertices[3] = end_pos.y;

		//create VAO
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);

		//create VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4, vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		
		glBindVertexArray(0);
		
	}

	~Debug_Line()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	void change_startpos(glm::vec3 startpos)
	{
		start_pos = startpos;
		update_line();
	}

	void change_endpos(glm::vec3 endpos)
	{
		end_pos = endpos;
		update_line();
	}

	void draw()
	{
		if(has_moved)
		{
			update_position();
		}

		shader->set_mat4f("model", model);
		shader->set_vec3f("color", color.x, color.y, color.z);
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, 2);
	}

	//move to location
	void move(double x, double y)
	{
		offset.x = x;
		offset.y = y;
	}
};
