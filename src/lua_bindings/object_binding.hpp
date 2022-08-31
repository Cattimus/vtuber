#pragma once

extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
	#include "wrap_struct.h"
}

#include "primitives/object.hpp"

namespace lua_bindings
{
	Object* get_object(lua_State* L, int index)
	{
		//check object is userdata
		void* temp = luaL_checkudata(L, index, "vtuber.object");
		luaL_argcheck(L, temp != NULL, index, "'vtuber.object' expected.");
		lua_obj* data = (lua_obj*)temp;

		//check to ensure object is not corrupted
		bool correct_header = data->header == HEADER_DEFAULT;
		bool correct_type = data->type == LUA_OBJ_OBJECT;
		luaL_argcheck(L, correct_header, index, "Object is corrupted.");
		luaL_argcheck(L, correct_type, index, "type mismatch. type should be 'vtuber.object'");

		//return reference to object
		return (Object*)(data->data);
	}

	//create object from pointer as light userdata
	void create_object(lua_State* L, Object* data)
	{
		//create new userdata and set appropriate fields
		auto temp = new(lua_newuserdata(L, sizeof(lua_obj))) lua_obj;
		temp->data = data;
		temp->type = LUA_OBJ_OBJECT;

		//assign metatable
		luaL_getmetatable(L, "vtuber.object");
		lua_setmetatable(L, -2);
	}

	//wrapper for: bool clicked(int x, int y)
	int object_clicked(lua_State* L)
	{
		Object* obj = get_object(L, 1); //argument 1 will be self

		luaL_argcheck(L, lua_isnumber(L, 2), 2, "'number' expected");
		luaL_argcheck(L, lua_isnumber(L, 3), 3, "'number' expected");

		int x = lua_tonumber(L, 2); //argument 2 will be x
		int y = lua_tonumber(L, 3); //argument 3 will be y

		bool result = obj->clicked(x,y);
		lua_pushboolean(L, result);
		return 1;
	}

	//wrapper for: double get_width()
	int object_getw(lua_State* L)
	{
		Object* obj = get_object(L, 1);
		double w = obj->get_width();
		lua_pushnumber(L, w);
		return 1;
	}

	//wrapper for: double get_height()
	int object_geth(lua_State* L)
	{
		Object* obj = get_object(L, 1);
		double h = obj->get_height();
		lua_pushnumber(L, h);
		return 1;
	}

	//wrapper for: void reset_position()
	int object_reset_pos(lua_State* L)
	{
		Object* obj = get_object(L, 1);
		obj->reset_position();
		return 0;
	}

	//wrapper for: SDL_Rect get_position();
	//returns: {x, y, w, h}
	int object_get_pos(lua_State* L)
	{
		Object* obj = get_object(L, 1);
		SDL_Rect temp = obj->get_position();
		lua_pushnumber(L, temp.x);
		lua_pushnumber(L, temp.y);
		lua_pushnumber(L, temp.w);
		lua_pushnumber(L, temp.h);
		return 4;
	}

	//wrapper for: SDL_Rect get_origin();
	//returns: {x, y, w, h}
	int object_get_origin(lua_State* L)
	{
		Object* obj = get_object(L, 1);
		SDL_Rect temp = obj->get_origin();
		lua_pushnumber(L, temp.x);
		lua_pushnumber(L, temp.y);
		lua_pushnumber(L, temp.w);
		lua_pushnumber(L, temp.h);
		return 4;
	}

	//wrapper for: SDL_Rect get_offset();
	//returns: {x, y, w, h}
	int object_get_offset(lua_State* L)
	{
		Object* obj = get_object(L, 1);
		SDL_Rect temp = obj->get_offset();
		lua_pushnumber(L, temp.x);
		lua_pushnumber(L, temp.y);
		lua_pushnumber(L, temp.w);
		lua_pushnumber(L, temp.h);
		return 4;
	}

	//wrapper for: void set_origin(x, y)
	int object_set_origin(lua_State* L)
	{
		Object* obj = get_object(L, 1); //argument 1 will be self(object)

		luaL_argcheck(L, lua_isnumber(L, 2), 2, "'number' expected");
		luaL_argcheck(L, lua_isnumber(L, 3), 3, "'number' expected");

		int x = lua_tonumber(L, 2); //argument 2 will be x
		int y = lua_tonumber(L, 3); //argument 3 will be y

		obj->set_origin(x, y);
		return 0;
	}

	//wrapper for: void set_offset(x, y)
	int object_set_offset(lua_State* L)
	{
		Object* obj = get_object(L, 1); //argument 1 will be self(object)

		luaL_argcheck(L, lua_isnumber(L, 2), 2, "'number' expected");
		luaL_argcheck(L, lua_isnumber(L, 3), 3, "'number' expected");

		int x = lua_tonumber(L, 2); //argument 2 will be x
		int y = lua_tonumber(L, 3); //argument 3 will be y

		obj->set_offset(x, y);
		return 0;
	}

	//wrapper for void move_to(double x, double y)
	int object_move_to(lua_State* L)
	{
		Object* obj = get_object(L, 1); //argument 1 will be self(object)

		luaL_argcheck(L, lua_isnumber(L, 2), 2, "'number' expected");
		luaL_argcheck(L, lua_isnumber(L, 3), 3, "'number' expected");

		int x = lua_tonumber(L, 2); //argument 2 will be x
		int y = lua_tonumber(L, 3); //argument 3 will be y

		obj->move_to(x, y);
		return 0;
	}

	//wrapper for void move_to(double x, double y)
	int object_relative_move(lua_State* L)
	{
		Object* obj = get_object(L, 1); //argument 1 will be self(object)

		luaL_argcheck(L, lua_isnumber(L, 2), 2, "'number' expected");
		luaL_argcheck(L, lua_isnumber(L, 3), 3, "'number' expected");

		int x = lua_tonumber(L, 2); //argument 2 will be x
		int y = lua_tonumber(L, 3); //argument 3 will be y

		obj->relative_move(x, y);
		return 0;
	}

	//wrapper for void print_position()
	int object_print_position(lua_State* L)
	{
		Object* obj = get_object(L, 1);
		obj->print_position();
		return 0;
	}

	//define object methods
	static const struct luaL_Reg object_local [] = {
		{"get_width", object_getw},
		{"get_height", object_geth},
		{"get_position", object_get_pos},
		{"get_origin", object_get_origin},
		{"get_offset", object_get_offset},
		{"reset_position", object_reset_pos},
		{"set_origin", object_set_origin},
		{"set_offset", object_set_offset},
		{"move_to", object_move_to},
		{"relative_move", object_relative_move},
		{"print_position", object_print_position},
		{NULL, NULL}
	};

	static const struct luaL_Reg object_global [] = {
		{NULL, NULL}
	};

	//define metatable for object
	int luaopen_object(lua_State* L)
	{
		luaL_newmetatable(L, "vtuber.object");
		lua_pushstring(L, "__index");
		lua_pushvalue(L, -2);
		lua_settable(L, -3);

		luaL_openlib(L, NULL, object_local, 0);
		luaL_openlib(L, "object", object_global, 0);
		lua_pop(L, -1); //we need to pop the metatable off the stack, since lua isn't doing that for us
		return 1;
	}
}