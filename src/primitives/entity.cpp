#include "entity.hpp"

void Entity::init(Texture* texture)
{
	this->texture = texture;
	texture_coords = (SDL_Rect){0, 0, texture->getw(), texture->geth()};
	clamp = NULL;

	h_flip = false;
	v_flip = false;
	flipval = SDL_FLIP_NONE;
}

//calculate the appropriate flipping flag for sdl
void Entity::set_flip()
{
	if(v_flip && h_flip)
	{
		flipval = (SDL_RendererFlip)(SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL);
	}
	else if(v_flip)
	{
		flipval = SDL_FLIP_VERTICAL;
	}
	else if(h_flip)
	{
		flipval = SDL_FLIP_HORIZONTAL;
	}
	else
	{
		flipval = SDL_FLIP_NONE;
	}
}

Entity::Entity() : Object(0,0)
{
	texture = NULL;
	clamp = NULL;
	h_flip = false;
	v_flip = false;
	flipval = SDL_FLIP_NONE;
}

Entity::Entity(int w, int h, Texture* texture) : Object(w, h)
{
	init(texture);

}

Entity::Entity(SDL_Rect size, Texture* texture) : Object(size)
{	
	init(texture);
}

Entity::Entity(const Entity& to_copy) : Object(to_copy)
{
	*this = to_copy;
}

Entity::~Entity()
{
	return;
}

Entity& Entity::operator=(const Entity& to_copy)
{
	if(this == &to_copy)
	{
		return *this;
	}

	Object::operator=(to_copy);
	texture = to_copy.texture;
	texture_coords = to_copy.texture_coords;
	clamp = to_copy.clamp;
	flipval = to_copy.flipval;
	h_flip = to_copy.h_flip;
	v_flip = to_copy.v_flip;
	return *this;
}

//split the object into two rectangles at vertical position y
void Entity::entity_split(double y, int position)
{
	SDL_Rect curpos = get_position();
	double top_height = y;
	double bottom_height = curpos.h - y;

	//texture coordinate height for the top value
	double top_texcoord_h = (texture->geth() / (double)curpos.h) * top_height;

	//update origin to reflect slice
	if(position > 0)
	{
		//texture coordinate is sliced down to half the value
		texture_coords.h = top_texcoord_h;
		origin.h = top_height;
		offset.h = top_height;
	}

	if(position < 0)
	{
		//y value will be moved down to reflect the changed origin
		origin.y += top_height;
		origin.h = bottom_height;
		offset.h = bottom_height;

		//texture coordinates will be modified to slice the texture in half
		texture_coords.h -= top_texcoord_h;
		texture_coords.y += top_texcoord_h;
	}
}

void Entity::change_texture(Texture* to_change)
{
	texture = to_change;
	texture_coords.w = to_change->getw();
	texture_coords.h = to_change->geth();
}

Texture* Entity::get_texture()
{
	return texture;
}

//flip texture vertically
void Entity::flip_vertical()
{
	v_flip = !v_flip;
	set_flip();
}

//flip texture horizontally
void Entity::flip_horizontal()
{
	h_flip = !h_flip;
	set_flip();
}

//reset size and position
void Entity::reset_size()
{
	origin.x = 0;
	origin.y = 0;
	offset = origin;
}

//clamp an entity to an object
void Entity::clamp_to(Object* to)
{
	clamp = to;

	//set rotation axis to the clamped object's rotation axis
	rotation_axis = to->get_rotation_axis();
	rotation = to->get_rotation();
}

//draw object on the screen
void Entity::draw(SDL_Renderer* renderer)
{
	//prevent segfaults if default initialized
	if(!texture)
	{
		return;
	}

	//TODO - this will need to be scriptable in the future
	if(clamp)
	{
		origin = clamp->get_position();
		rotation = clamp->get_rotation();
		origin.y += (rotation_axis.x - origin.x + origin.w / 2) * tan(clamp->get_rotation() * 3.1415 / 180.0);
	}

	SDL_Rect position = get_position();
	SDL_RenderCopyEx(renderer, texture->get_tex(), &texture_coords, &position, rotation, &rotation_axis, flipval);
}

