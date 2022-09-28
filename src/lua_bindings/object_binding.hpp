#pragma once

extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
	#include "wrap_struct.h"
}

#include "primitives/object.hpp"

namespace lua_bindings
{
	//get a pointer to the object stored at argument index
	Object* get_object(lua_State* L, int index);

	//create object from pointer as light userdata. data is a pointer to the object to be converted.
	void create_object(lua_State* L, Object* data);
	
	//wrapper for: bool clicked(int x, int y)
	int object_clicked(lua_State* L);

	//wrapper for: double get_rotation()
	int object_getrot(lua_State* L);

	//wrapper for: void set_rotation_axis()
	int object_setrot_axis(lua_State* L);

	//wrapper for: double get_width()
	int object_getw(lua_State* L);

	//wrapper for: double get_height()
	int object_geth(lua_State* L);

	//wrapper for: void reset_rotation()
	int object_reset_rot(lua_State* L);

	//wrapper for: void reset_position()
	int object_reset_pos(lua_State* L);

	//wrapper for: SDL_Rect get_position();
	//returns: {x, y, w, h}
	int object_get_pos(lua_State* L);

	//wrapper for: SDL_Rect get_origin();
	//returns: {x, y, w, h}
	int object_get_origin(lua_State* L);

	//wrapper for: SDL_Rect get_offset();
	//returns: {x, y, w, h}
	int object_get_offset(lua_State* L);

	//wrapper for: void set_rotation(rot)
	int object_setrot(lua_State* L);

	//wrapper for: void set_origin(x, y)
	int object_set_origin(lua_State* L);

	//wrapper for: void set_offset(x, y)
	int object_set_offset(lua_State* L);

	//wrapper for void move_to(double x, double y)
	int object_move_to(lua_State* L);

	//wrapper for void move_to(double x, double y)
	int object_relative_move(lua_State* L);

	//wrapper for void print_position()
	int object_print_position(lua_State* L);	

	//define metatable for object
	int luaopen_object(lua_State* L);
}
