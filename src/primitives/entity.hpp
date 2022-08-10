#pragma once

#include "globals.hpp"
#include "object.hpp"
#include "texture.hpp"

class Entity : public Object
{
protected:
	Texture* texture;
	SDL_Rect texture_coords;

public:

	Entity(SDL_Rect size, Texture* texture) : Object(size)
	{	
		this->texture = texture;
		texture_coords = (SDL_Rect){0, 0, texture->getw(), texture->geth()};
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
		this->texture_coords = to_copy.texture_coords;
		return *this;
	}

	//split the object into two rectangles at position y
	void entity_split(double y, int position)
	{
		SDL_Rect curpos = get_position();
		double top_height = y;
		double bottom_height = curpos.h - y;
		double top_texcoord_h = round((texture->geth() / (double)curpos.h) * top_height);

		//update origin to reflect slice
		if(position > 0)
		{
			texture_coords.h = top_texcoord_h;
			origin.h = top_height;
			offset.h = top_height;
		}

		if(position < 0)
		{
			origin.y += top_height;
			origin.h = bottom_height;
			offset.h = bottom_height;
			texture_coords.h -= top_texcoord_h;
			texture_coords.y += top_texcoord_h;
		}
	}

	//reset size and position
	void reset_size()
	{
		origin.x = 0;
		origin.y = 0;
		offset = origin;
	}

	void draw()
	{
		SDL_Rect position = get_position();
		SDL_RenderCopy(renderer, texture->get_tex(), &texture_coords, &position);
	}
	
};
