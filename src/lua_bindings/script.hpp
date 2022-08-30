#pragma once

#include <iostream>

extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
}

class Script
{
private:
	std::string file_path;
	lua_State* L;

public:

	Script(std::string path, lua_State* L)
	{
		file_path = path;
		this->L = L;

		//load script values
		luaL_loadfile(L, path.c_str());

		//sandbox file
		lua_newtable(L);
		
		//give access to print function
		lua_pushstring(L, "print");
		lua_getglobal(L, "print");
		lua_settable(L, -3);

		//give access to getplayer function
		lua_pushstring(L, "get_player");
		lua_getglobal(L, "get_player");
		lua_settable(L, -3);

		//set environment
		lua_setfenv(L, -2);

		if(lua_pcall(L, 0, 0, 0) != 0)
		{
			std::cout << "error running file " << path << ": " << lua_tostring(L, -1) << std::endl;
		}
	}

	//function to call lua functions here
};