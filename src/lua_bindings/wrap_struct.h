#pragma once

//type for lua object(userdata objects that reference C++ objects)
const unsigned int HEADER_DEFAULT = 0xF0BA1234;
struct lua_obj
{
	unsigned header = HEADER_DEFAULT;
	void* data;
};

struct lua_arg
{
	int type;
	union Data
	{
		bool boolean;
		double number;
		const char* string;
	}data;
};
