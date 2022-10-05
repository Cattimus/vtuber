#pragma once

#include "object.hpp"
#include "texture.hpp"
#include "entity.hpp"
#include "lua_bindings/object_binding.hpp"
#include <SDL.h>

class Avatar : public Object
{
private:
	//copies of origins for both values
	SDL_Rect top_origin;
	SDL_Rect bottom_origin;

	//entities for top and bottom
	Entity top;
	Entity bottom;
	bool is_split;

	//controls the distance between top and bottom during animation
	double talk_height;

	void update_position();
	void unsplit();


public:
	
	//constructors and operators
	Avatar(SDL_Rect size, Texture* texture);
	Avatar(const Avatar& to_copy);
	Avatar& operator=(const Avatar& to_copy);
	virtual ~Avatar();

	//getters
	Texture* get_texture();
	Object* get_top();	
	Object* get_bottom();
	double get_talk_height();
	
	//mutators
	void change_texture(Texture* to_change);
	void avatar_split(double y);
	void reset_position();
	void set_talk_height(double height);

	//animations
	void draw(SDL_Renderer* renderer);
	void talk(double height);
	void flip_horizontal();
	void flip_vertical();
};
