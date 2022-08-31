#pragma once

//struct to hold references to C++ functions as lua userdata
const unsigned int HEADER_DEFAULT = 0xF0BA1234;
struct lua_obj
{
	unsigned header = HEADER_DEFAULT;
	void* data;
};

//struct to pass arguments to lua scripts
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
