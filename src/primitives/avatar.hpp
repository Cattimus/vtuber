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

	//flag to see if value is split or not
	bool is_split;

	//default talk height value
	double talk_height;

	//move both sub-objects by their origins in respect to the parent object. This means they can move freely when talking
	void update_position();

	//reverse the splitting operation on texture change
	void unsplit();


public:
	Avatar(SDL_Rect size, Texture* texture);
	Avatar(const Avatar& to_copy);
	Avatar& operator=(const Avatar& to_copy);
	virtual ~Avatar();

	//draw object to the screen
	void draw(SDL_Renderer* renderer);
	
	//update texture
	void change_texture(Texture* to_change);

	Texture* get_texture();

	//split the avatar along the horizontal line at position y
	void avatar_split(double y);

	//get a reference to the top object
	Object* get_top();

	//get a reference to the bottom object
	Object* get_bottom();

	//move top jaw
	void talk(double height);

	//reset object to origin
	void reset_position();

	//set the maximum height of the "talk" function
	void set_talk_height(double height);

	double get_talk_height();

	//flip the top and bottom horizontally
	void flip_horizontal();

	//flip the top and bottom vertically
	void flip_vertical();
};
