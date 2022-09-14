#pragma once

#include <SDL.h>
#include "globals.hpp"
#include "lua_bindings/script.hpp"

class Object
{
protected:
	//translation data
	SDL_Rect origin;
	SDL_Rect offset;
	double rotation;
	SDL_Point rotation_axis;
	Script* script;

	void init(SDL_Rect& size)
	{
		origin.x = size.x;
		origin.y = size.y;

		offset.x = 0;
		offset.y = 0;

		this->offset.w = size.w;
		this->offset.h = size.h;
		this->origin.w = size.w;
		this->origin.h = size.h;

		this->rotation_axis.x = size.w / 2;
		this->rotation_axis.y = size.h / 2;
		rotation = 0;

		this->script = NULL;
	}

	void init(int w, int h)
	{
		origin.x = 0;
		origin.y = 0;

		offset.x = 0;
		offset.y = 0;

		this->offset.w = w;
		this->offset.h = h;
		this->origin.w = w;
		this->origin.h = h;

		this->rotation_axis.x = w / 2;
		this->rotation_axis.y = h / 2;
		rotation = 0;

		this->script = NULL;
	}

public:
	explicit Object(SDL_Rect& size)
	{	
		init(size);
	}

	Object(int w, int h)
	{
		init(w, h);
	}

	Object(int w, int h, Script* script)
	{
		init(w, h);

		this->script = script;
	}

	Object(SDL_Rect& size, Script* script)
	{
		init(size);

		this->script = script;
	}

	//copy constructor
	Object(const Object& to_copy)
	{
		*this = to_copy;
	}

	//overload assignment operator
	Object& operator=(const Object& to_copy)
	{
		//self assignment gaurd
		if(this == &to_copy)
		{
			return *this;
		}

		//copy data
		this->origin = to_copy.origin;
		this->offset = to_copy.offset;
		this->rotation = to_copy.rotation;
		this->rotation_axis = to_copy.rotation_axis;
		this->script = to_copy.script;

		return *this;
	}

	//AABB collision detection
	bool clicked(double x, double y)
	{
		SDL_Rect current_pos = get_position();
		
		double xmin = current_pos.x;
		double xmax = current_pos.x + offset.w;
		double ymin = current_pos.y;
		double ymax = current_pos.y + offset.h;
		
		if((x >= xmin && x <= xmax) &&
		   (y >= ymin && y <= ymax))
		{
			return true;
		}

		return false;
	}
	
	double get_width()
	{
		return offset.w;
	}

	double get_height()
	{
		return offset.y;
	}

	double get_rotation()
	{
		return rotation;
	}

	void set_rotation(double rot)
	{
		rotation = rot;
	}

	//argument should be a value between 0 and 1 for x and y
	void set_rotation_axis(double x, double y)
	{
		rotation_axis.x = offset.w * x;
		rotation_axis.y = offset.h * y;

		if(rotation_axis.x > offset.w)
		{
			rotation_axis.x = offset.w;
		}

		if(rotation_axis.y > offset.h)
		{
			rotation_axis.y = offset.h;
		}
	}

	void set_rotation_axis_pixels(double x, double y)
	{
		rotation_axis.x = x;
		rotation_axis.y = y;
	}

	SDL_Point get_rotation_axis()
	{
		return rotation_axis;
	}

	//calculate position (origin + offset)
	SDL_Rect get_position()
	{
		SDL_Rect output;
		output.x = origin.x + offset.x;
		output.y = origin.y + offset.y;
		output.w = offset.w;
		output.h = offset.h;
		return output;
	}
	
	SDL_Rect get_origin()
	{
		return origin;
	}

	SDL_Rect get_offset()
	{
		return offset;
	}

	//return position to origin
	void reset_position()
	{
		offset.x = 0;
		offset.y = 0;
	}

	void reset_rotation()
	{
		rotation = 0;
	}

	void set_script(Script* script)
	{
		this->script = script;
	}
	
	//update origin
	void set_origin(SDL_Rect& new_origin)
	{
		origin = new_origin;
	}
	void set_origin(double x, double y)
	{
		origin.x = x;
		origin.y = y;
	}

	//set offset to an absolute value
	void set_offset(double x, double y)
	{
		offset.x = x;
		offset.y = y;
	}

	//move to absolute location on the screen
	void move_to(double x, double y)
	{
		double adjusted_x = x - origin.x;
		double adjusted_y = y - origin.y;
		
		offset.x = adjusted_x;
		offset.y = adjusted_y;
	}

	//move in respect to the current position
	void relative_move(double x, double y)
	{
		offset.x += x;
		offset.y += y;
	}

	//debug function to print the object's current location
	void print_position()
	{
		SDL_Rect position = get_position();

		std::cout << "x: " << position.x;
		std::cout << " y: " << position.y;
		std::cout << std::endl;
	}
};
