#pragma once

//struct to hold references to C++ objects as lua userdata
const unsigned int HEADER_DEFAULT = 0xF0BA1234;
struct lua_obj
{
	unsigned header = HEADER_DEFAULT;
	void* data;
};
