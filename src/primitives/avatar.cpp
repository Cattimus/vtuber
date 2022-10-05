#include "avatar.hpp"

void Avatar::update_position()
{
	SDL_Rect position = get_position();

	top.set_origin(top_origin.x + position.x, top_origin.y + position.y);
	if(is_split)
	{
		bottom.set_origin(bottom_origin.x + position.x, bottom_origin.y + position.y);
	}
}

void Avatar::unsplit()
{
	top.set_origin(origin);
	bottom.set_origin(origin);
	top.reset_size();
	bottom.reset_size();
	is_split = false;
}

Avatar::Avatar(SDL_Rect size, Texture* texture) : Object(size)
{
	//initialize our entities
	top = Entity((SDL_Rect){0,0,size.w,size.h}, texture);
	bottom = Entity((SDL_Rect){0,0,size.w,size.h}, texture);

	talk_height = 100;
	is_split = false;
	top_origin = (SDL_Rect){0,0,size.w,size.h};
	bottom_origin = (SDL_Rect){0,0,size.w,size.h};
}

Avatar::Avatar(const Avatar& to_copy) : Object(to_copy)
{
	*this = to_copy;
}

Avatar::~Avatar()
{
	return;
}

Avatar& Avatar::operator=(const Avatar& to_copy)
{
	//self assignment guard
	if(this == &to_copy)
	{
		return *this;
	}
	Object::operator=(to_copy);

	//copy entities
	top = to_copy.top;
	bottom = to_copy.bottom;
	is_split = to_copy.is_split;
	talk_height = to_copy.talk_height;
	top_origin = to_copy.top_origin;
	bottom_origin = to_copy.bottom_origin;

	return *this;
}

void Avatar::draw(SDL_Renderer* renderer)
{
	update_position();

	top.draw(renderer);
	if(is_split)
	{
		bottom.draw(renderer);
	}
}

void Avatar::change_texture(Texture* to_change)
{
	top.change_texture(to_change);
	bottom.change_texture(to_change);
	unsplit();
}

Texture* Avatar::get_texture()
{
	return top.get_texture();
}

void Avatar::avatar_split(double y)
{
	if(!is_split)
	{
		double yoffset = y - top.get_position().y;

		//set origin to 0 in order to calculate properly
		top.set_origin(0,0);
		bottom = top;

		//split avatar
		top.entity_split(yoffset, 1);
		bottom.entity_split(yoffset, -1);

		//make a copy of the values
		top_origin = top.get_origin();
		bottom_origin = bottom.get_origin();

		is_split = true;
	}
}

Object* Avatar::get_top()
{
	return (Object*)&top;
}

Object* Avatar::get_bottom()
{
	return (Object*)&bottom;
}

void Avatar::talk(double height)
{	
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

	//if a script has been defined, call the talk function here
	if(script)
	{
		script->load_function("vtuber_avatar_talk");
		lua_bindings::create_object(script->get_state(), (Object*)&top);
		lua_bindings::create_object(script->get_state(), (Object*)&bottom);
		lua_pushnumber(script->get_state(), height * -1);
		script->call(3);

		return;
	}

	top.reset_position();
	top.relative_move(0, move_val);
}

void Avatar::reset_position()
{	
	offset.x = 0;
	offset.y = 0;
}

void Avatar::set_talk_height(double height)
{
	talk_height = height;
}

double Avatar::get_talk_height()
{
	return talk_height;
}

void Avatar::flip_horizontal()
{
	if(is_split)
	{
		top.flip_horizontal();
		bottom.flip_horizontal();
	}
	else
	{
		top.flip_horizontal();
	}
}

void Avatar::flip_vertical()
{
	if(is_split)
	{
		top.flip_vertical();
		bottom.flip_vertical();
	}
	else
	{
		top.flip_vertical();
	}
}

