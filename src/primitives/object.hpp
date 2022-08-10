#pragma once

#include <SDL.h>
#include "shaders.hpp"
#include "project_includes.hpp"

class Object
{
protected:
	//translation data
	SDL_Rect origin;
	SDL_Rect offset;
	
public:
	explicit Object(SDL_Rect size)
	{	
		origin.x = 0;
		origin.y = 0;

		offset.x = 0;
		offset.y = 0;

		this->offset.w = (size.w / 2);
		this->offset.h = (size.h / 2);
		this->origin.w = offset.w;
		this->origin.h = offset.h;
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
		this->origin = to_copy.origin;
		this->offset = to_copy.offset;

		return *this;
	}

	//AABB collision detection
	bool clicked(double x, double y)
	{
		SDL_Rect current_pos = get_position();
		
		double xmin = current_pos.x - current_pos.w;
		double xmax = current_pos.x + current_pos.w;
		double ymin = current_pos.y - current_pos.h;
		double ymax = current_pos.y + current_pos.h;
		
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

	void reset_position()
	{
		offset.x = 0;
		offset.y = 0;
	}
	
	//update origin
	void set_origin(SDL_Rect new_origin)
	{
		origin = new_origin;
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
