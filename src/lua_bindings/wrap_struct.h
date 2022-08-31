#pragma once


const unsigned int HEADER_DEFAULT = 0xF0BA1234;
enum LUA_OBJ_TYPE {LUA_OBJ_NULL, LUA_OBJ_OBJECT};

typedef struct 
{
	unsigned header = HEADER_DEFAULT;
	LUA_OBJ_TYPE type;
	void* data;
}lua_obj;