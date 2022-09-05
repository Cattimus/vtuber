#pragma once

#include "globals.hpp"
#include "object.hpp"
#include "texture.hpp"
#include "entity.hpp"
#include "lua_bindings/object_binding.hpp"
#include <SDL.h>

//TODO - tie an entity to another entity (accessories)
//TODO - implement flipping image horizontally and vertically

class Avatar : public Object
{
private:
	//copies of origins for both values
	SDL_Rect top_origin;
	SDL_Rect bottom_origin;

	Entity* avatar_top;
	Entity* avatar_bottom;
	bool is_split;

	double talk_height = 100;

	//move both sub-objects by their origins in respect to the parent object. This means they can move freely when talking
	void update_position()
	{
		SDL_Rect position = get_position();
		if(!avatar_bottom)
		{
			avatar_top->set_origin(top_origin.x + position.x, top_origin.y + position.y);
		}
		else
		{
			avatar_top->set_origin(top_origin.x + position.x, top_origin.y + position.y);
			avatar_bottom->set_origin(bottom_origin.x + position.x, bottom_origin.y + position.y);
		}
	}

public:

	//create new (unsplit) avatar
	Avatar(SDL_Rect size, Texture* texture) : Object(size)
	{
		this->is_split = false;
		this->top_origin = (SDL_Rect){0,0, size.w, size.h};
		this->bottom_origin = (SDL_Rect){0,0, size.w, size.h};
		
		//avatar will initially just be the "top" entity. once split the bottom will be initialized
		this->avatar_top = new Entity((SDL_Rect){0,0, size.w, size.h}, texture);
		avatar_bottom = NULL;
	}

	//copy constructor
	Avatar(const Avatar& to_copy) : Object(to_copy)
	{
		*this = to_copy;
	}

	//assignment operator (copy values)
	Avatar& operator=(const Avatar& to_copy)
	{
		//self assignment guard
		if(this == &to_copy)
		{
			return *this;
		}

		Object::operator=(to_copy);
		this->avatar_top = NULL;
		this->avatar_bottom = NULL;
		this->is_split = false;
		this->talk_height = to_copy.talk_height;

		if(to_copy.avatar_top)
		{
			this->avatar_top = new Entity(*to_copy.avatar_top);
		}

		if(to_copy.avatar_bottom)
		{
			this->avatar_bottom = new Entity(*to_copy.avatar_bottom);
			this->is_split = true;
		}

		return *this;
	}

	~Avatar()
	{
		if(avatar_top)
		{
			delete avatar_top;
		}

		if(avatar_bottom)
		{
			delete avatar_bottom;
		}
	}

	//draw values to screen
	void draw()
	{
		update_position();
		if(avatar_top)
		{
			avatar_top->draw();
		}

		if(avatar_bottom)
		{
			avatar_bottom->draw();
		}
	}

	//split the avatar along the selected line
	void avatar_split(double y)
	{
		if(!is_split)
		{
			double yoffset = y - avatar_top->get_position().y;

			//set origin to 0 in order to calculate properly
			avatar_top->set_origin(0,0);
			avatar_bottom = new Entity(*avatar_top);

			avatar_top->entity_split(yoffset, 1);
			avatar_bottom->entity_split(yoffset, -1);

			top_origin = avatar_top->get_origin();
			bottom_origin = avatar_bottom->get_origin();

			is_split = true;
		}
	}

	//move top jaw
	void talk(double height)
	{
		//get references to top and bottom object
		//send to script
		if(!is_split)
		{
			return;
		}

		if(height < 0)
		{
			height = 0;
		}
		if(height > 1)
		{
			height = 1;
		}
		
		double move_val = height * talk_height * -1;
		avatar_top->reset_position();

		if(script)
		{
			script->load_function("vtuber_avatar_talk");
			lua_bindings::create_object(script->get_state(), (Object*)avatar_top);
			lua_bindings::create_object(script->get_state(), (Object*)avatar_bottom);
			lua_pushnumber(script->get_state(), move_val);
			script->call(3);

			return;
		}

		avatar_top->relative_move(0, move_val);
	}

	//move all entities back to origin
	void reset_position()
	{	
		offset.x = 0;
		offset.y = 0;
	}

	//set the maximum height of the "talk" function
	void set_talk_height(double height)
	{
		talk_height = height;
	}

	double get_talk_height()
	{
		return talk_height;
	}

};
