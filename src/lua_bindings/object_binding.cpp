#include "object_binding.hpp"
	
Object* lua_bindings::get_object(lua_State* L, int index)
{
	//check object is userdata
	void* temp = luaL_checkudata(L, index, "vtuber.object");
	luaL_argcheck(L, temp != NULL, index, "'vtuber.object' expected.");
	lua_obj* data = (lua_obj*)temp;

	//check to ensure object is not corrupted
	bool correct_header = data->header == HEADER_DEFAULT;
	luaL_argcheck(L, correct_header, index, "Object is corrupted.");

	//return reference to object
	return (Object*)(data->data);
}

void lua_bindings::create_object(lua_State* L, Object* data)
{
	//create new userdata and set appropriate fields
	auto temp = new(lua_newuserdata(L, sizeof(lua_obj))) lua_obj;
	temp->data = data;

	//assign metatable
	luaL_getmetatable(L, "vtuber.object");
	lua_setmetatable(L, -2);
}

int lua_bindings::object_clicked(lua_State* L)
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

int lua_bindings::object_getrot(lua_State* L)
{
	Object* obj = get_object(L, 1);
	double rot = obj->get_rotation();
	lua_pushnumber(L, rot);
	return 1;
}

int lua_bindings::object_setrot_axis(lua_State* L)
{
	Object* obj = get_object(L, 1);
	luaL_argcheck(L, lua_isnumber(L, 2), 2, "'number' expected");
	luaL_argcheck(L, lua_isnumber(L, 3), 3, "'number' expected");
	double x = lua_tonumber(L, 2);
	double y = lua_tonumber(L, 3);

	obj->set_rotation_axis(x, y);
	return 0;
}

//wrapper for: double get_width()
int lua_bindings::object_getw(lua_State* L)
{
	Object* obj = get_object(L, 1);
	double w = obj->get_width();
	lua_pushnumber(L, w);
	return 1;
}

//wrapper for: double get_height()
int lua_bindings::object_geth(lua_State* L)
{
	Object* obj = get_object(L, 1);
	double h = obj->get_height();
	lua_pushnumber(L, h);
	return 1;
}

//wrapper for: void reset_rotation()
int lua_bindings::object_reset_rot(lua_State* L)
{
	Object* obj = get_object(L, 1);
	obj->reset_rotation();
	return 0;
}

//wrapper for: void reset_position()
int lua_bindings::object_reset_pos(lua_State* L)
{
	Object* obj = get_object(L, 1);
	obj->reset_position();
	return 0;
}

//wrapper for: SDL_Rect get_position();
//returns: {x, y, w, h}
int lua_bindings::object_get_pos(lua_State* L)
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
int lua_bindings::object_get_origin(lua_State* L)
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
int lua_bindings::object_get_offset(lua_State* L)
{
	Object* obj = get_object(L, 1);
	SDL_Rect temp = obj->get_offset();
	lua_pushnumber(L, temp.x);
	lua_pushnumber(L, temp.y);
	lua_pushnumber(L, temp.w);
	lua_pushnumber(L, temp.h);
	return 4;
}

//wrapper for: void set_rotation(rot)
int lua_bindings::object_setrot(lua_State* L)
{
	Object* obj = get_object(L, 1);
	luaL_argcheck(L, lua_isnumber(L, 2), 2, "'number' expected");

	double rot = lua_tonumber(L, 2);

	obj->set_rotation(rot);
	return 0;
}

//wrapper for: void set_origin(x, y)
int lua_bindings::object_set_origin(lua_State* L)
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
int lua_bindings::object_set_offset(lua_State* L)
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
int lua_bindings::object_move_to(lua_State* L)
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
int lua_bindings::object_relative_move(lua_State* L)
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
int lua_bindings::object_print_position(lua_State* L)
{
	Object* obj = get_object(L, 1);
	obj->print_position();
	return 0;
}

//define object methods for lua
static const struct luaL_Reg object_local [] = {
	{"get_width", lua_bindings::object_getw},
	{"get_height", lua_bindings::object_geth},
	{"get_position", lua_bindings::object_get_pos},
	{"get_origin", lua_bindings::object_get_origin},
	{"get_offset", lua_bindings::object_get_offset},
	{"get_rotation", lua_bindings::object_getrot},
	{"reset_position", lua_bindings::object_reset_pos},
	{"reset_rotation", lua_bindings::object_reset_rot},
	{"set_origin", lua_bindings::object_set_origin},
	{"set_offset", lua_bindings::object_set_offset},
	{"set_rotation", lua_bindings::object_setrot},
	{"set_rotation_axis", lua_bindings::object_setrot_axis},
	{"move_to", lua_bindings::object_move_to},
	{"relative_move", lua_bindings::object_relative_move},
	{"print_position", lua_bindings::object_print_position},
	{NULL, NULL}
};

//define object global methods (empty because we will be using only references not userdata)
static const struct luaL_Reg object_global [] = {
	{NULL, NULL}
};

//define metatable for object
int lua_bindings::luaopen_object(lua_State* L)
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

