#pragma once

#include "object.hpp"
#include "texture.hpp"

class Entity : public Object
{
protected:
	//texture data
	Texture* texture;
	SDL_Rect texture_coords;

	//object to clamp to (follow)
	Object* clamp;
	
	//flip values
	bool h_flip;
	bool v_flip;
	SDL_RendererFlip flipval;
	
	//helper functions
	void init(Texture* texture);
	void set_flip();

public:

	//constructors and operator overloads
	Entity();
	Entity(int w, int h, Texture* texture);
	Entity(SDL_Rect size, Texture* texture);
	Entity(const Entity& to_copy);
	Entity& operator=(const Entity& to_copy);
	virtual ~Entity();

	//getters
	Texture* get_texture();
	
	//mutators
	void change_texture(Texture* to_change);
	void reset_size();
	void clamp_to(Object* to);
	
	//drawing functions
	void draw(SDL_Renderer* renderer);
	void entity_split(double y, int position);
	void flip_vertical();
	void flip_horizontal();
};
