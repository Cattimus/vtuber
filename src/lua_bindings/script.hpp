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

	//set environment of script
	void set_environment()
	{
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
		if(!lua_setfenv(L, -2))
		{
			std::cout << "Script: sanboxing has failed. " << file_path << " will be running in the global namespace with no restrictions." << std::endl;
		}
	}

public:

	Script(std::string path, lua_State* L)
	{
		file_path = path;
		this->L = L;

		//load script values
		luaL_loadfile(L, path.c_str());
		set_environment();
		if(lua_pcall(L, 0, 0, 0) != 0)
		{
			std::cout << "error running file " << path << ": " << lua_tostring(L, -1) << std::endl;
		}
	}

	//function to call lua functions here
};