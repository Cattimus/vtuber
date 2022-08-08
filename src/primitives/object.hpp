#pragma once

#include "shaders.hpp"
#include "project_includes.hpp"

class Object
{
protected:
	//translation data
	glm::vec3 original_size;
	glm::vec3 size;
	glm::vec3 origin;
	glm::vec3 offset;
	glm::mat4 model;
	float move_speed;

	void update_position()
	{
		model = glm::translate(glm::mat4(1.0f), this->get_position());
	}

	void delta_move(float x, float y)
	{
		offset.x += (x * *delta);
		offset.y += (y * *delta);
	}

public:
	//movement flags
	bool up;
	bool down;
	bool left;
	bool right;
	
	explicit Object(glm::vec3 size)
	{	
		this->move_speed = 0;
		this->model = glm::mat4(1.0f);

		this->up = false;
		this->down = false;
		this->left = false;
		this->right = false;

		origin.x = 0;
		origin.y = 0;
		origin.z = 0;

		offset.x = 0;
		offset.y = 0;
		offset.z = 0;

		this->size.x = (size.x / 2);
		this->size.y = (size.y / 2);
		this->original_size = this->size;
	}

	//copy constructor
	Object(const Object& to_copy)
	{
		*this = to_copy;
	}

	//overload assignment operator(shallow copy)
	Object& operator=(const Object& to_copy)
	{
		//self assignment gaurd
		if(this == &to_copy)
		{
			return *this;
		}

		//copy data
		this->model = to_copy.model;
		this->up = to_copy.up;
		this->down = to_copy.down;
		this->left = to_copy.left;
		this->right = to_copy.right;
		this->origin = to_copy.origin;
		this->offset = to_copy.offset;
		this->size = to_copy.size;
		this->move_speed = to_copy.move_speed;
		this->original_size = to_copy.original_size;

		return *this;
	}

	//AABB collision detection
	bool clicked(double x, double y)
	{
		glm::vec3 current_pos = get_position();
		
		double xmin = current_pos.x - size.x;
		double xmax = current_pos.x + size.x;
		double ymin = current_pos.y - size.y;
		double ymax = current_pos.y + size.y;
		
		if((x >= xmin && x <= xmax) &&
		   (y >= ymin && y <= ymax))
		{
			return true;
		}

		return false;
	}
	
	double get_width()
	{
		return size.x;
	}

	double get_height()
	{
		return size.y;
	}

	glm::vec3 get_position()
	{
		return origin + offset;
	}

	glm::vec3 get_origin()
	{
		return origin;
	}

	glm::vec3 get_offset()
	{
		return offset;
	}

	void reset_position()
	{
		offset.x = 0;
		offset.y = 0;
	}
	
	//update origin
	void set_origin(glm::vec3 new_origin)
	{
		origin = new_origin;
	}

	void set_speed(float move_speed)
	{
		this->move_speed = move_speed;
	}

	void move()
	{
		if(up)
		{
			delta_move(0, move_speed);
		}

		if(down)
		{
			delta_move(0, move_speed * -1);
		}

		if(left)
		{
			delta_move(move_speed * -1, 0);
		}

		if(right)
		{
			delta_move(move_speed, 0);
		}
	}

	//move to absolute location
	void move_to(double x, double y)
	{
		double adjusted_x = x - origin.x;
		double adjusted_y = y - origin.y;
		
		offset.x = adjusted_x;
		offset.y = adjusted_y;
	}

	void relative_move(double x, double y)
	{
		offset.x += x;
		offset.y += y;
	}
};
