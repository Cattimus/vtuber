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
		texture_coords = {0, 0, texture->getw(), texture->geth()};
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
		return *this;
	}

	void split(double y, int position)
	{
		double relative_y = y - offset.y; //y relative to the object's position
		
		double bottom_portion = relative_y + offset.h; //portion of the object taken up by the bottom (split at y)
		double top_portion = (offset.h * 2) - bottom_portion; //portion of the object taken up by the top (split at y)

		//new texture coordinate offset, will always use bottom portion
		//double texture_offset = bottom_portion / (offset.h * 2);

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
