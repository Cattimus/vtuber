#pragma once

#include <SDL.h>
#include "globals.hpp"
#include "lua_bindings/script.hpp"
#include "primitives/texture.hpp"

//forward declaration of texture
class Texture;

class Object
{
protected:
	//translation data
	SDL_Rect origin;
	SDL_Rect offset;
	double rotation;
	SDL_Point rotation_axis;
	Script* script;

	int render_priority; //determines the order objects are drawn in. higher renders first

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

		this->render_priority = 0;
	}

	void init(int w, int h)
	{
		SDL_Rect temp;
		temp.w = w;
		temp.h = h;
		temp.x = 0;
		temp.y = 0;

		init(temp);
	}

public:
	explicit Object(SDL_Rect& size)
	{	
		init(size);
	}

	//w = width. h = height
	Object(int w, int h)
	{
		init(w, h);
	}

	//w = width. h = height. script is a lua script object
	Object(int w, int h, Script* script)
	{
		init(w, h);

		this->script = script;
	}

	//size is an SDL_Rect that contains x, y, w, h. script is a lua script object
	Object(SDL_Rect& size, Script* script)
	{
		init(size);

		this->script = script;
	}
	
	Object(const Object& to_copy)
	{
		*this = to_copy;
	}

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
		this->render_priority = to_copy.render_priority;

		return *this;
	}
	
	//draw function that can be overridden by child classes
	virtual void draw()
	{
		return;
	}

	//function that will be defined by child classes to change textures
	virtual void change_texture(Texture* to_change)
	{
		return;
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

	//argument will be between 0 and 1. This will calculate to a point inside the object.
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

	//argument will be x and y coordinates in screen space pixels
	void set_rotation_axis_pixels(double x, double y)
	{
		rotation_axis.x = x;
		rotation_axis.y = y;
	}

	//returns the current rotation axis
	SDL_Point get_rotation_axis()
	{
		return rotation_axis;
	}

	//return the current location on the screen of the object (origin + position)
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

	//reset rotation to 0 degrees
	void reset_rotation()
	{
		rotation = 0;
	}

	//assign an already loaded script to the object
	void set_script(Script* script)
	{
		this->script = script;
	}
	
	//update origin values
	void set_origin(SDL_Rect& new_origin)
	{
		origin = new_origin;
	}

	void reset_size()
	{
		offset.w = origin.w;
		offset.h = origin.h;
	}

	//update origin values (x and y)
	void set_origin(double x, double y)
	{
		origin.x = x;
		origin.y = y;
	}

	//update offset values
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

	void set_priority(int val)
	{
		render_priority = val;
	}

	int get_priority()
	{
		return render_priority;
	}

	void inc_priority()
	{
		render_priority = render_priority + 1;
	}

	void dec_priority()
	{
		render_priority = render_priority - 1;
	}
};
