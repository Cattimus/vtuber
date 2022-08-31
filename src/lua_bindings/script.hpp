#pragma once

#include <iostream>
#include <vector>

extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

#include "wrap_struct.h"

class Script
{
private:
	std::string file_path;
	lua_State* L;

	//create a new environment and set it in globals
	void init_environment()
	{
		//env table
		lua_newtable(L);
		
		//give access to print function
		lua_pushstring(L, "print");
		lua_getglobal(L, "print");
		lua_settable(L, -3);

		//give access to getplayer function
		lua_pushstring(L, "get_player");
		lua_getglobal(L, "get_player");
		lua_settable(L, -3);
		
		//set environment to the list of globals so it can be referenced later.
		lua_setglobal(L, (file_path + "env").c_str());
	}

	//push environment on stack
	void get_environment()
	{
		lua_getglobal(L, (file_path + "env").c_str());
	}

	//set function at top of stack to execute in environment
	void set_environment()
	{
		get_environment();

		if(!lua_setfenv(L, -2))
		{
			std::cout << "Script: sanboxing has failed. " << file_path << " will be running in the global namespace with no restrictions." << std::endl;
		}
	}

	//get function from the script's environment
	void get_function(std::string function_name)
	{
		get_environment();
		lua_getfield(L, -1, function_name.c_str());
		if(!lua_isfunction(L, -1))
		{
			std::cout << "Script.call(): " << function_name << " is not a defined function. " << lua_type(L, -1) << "\n";
			lua_pop(L, -1);
			lua_pop(L, -1);
			return;
		}

		//set script to execute in sandbox
		set_environment();
	}

public:

	Script(std::string path, lua_State* L)
	{
		file_path = path;
		this->L = L;

		//create a new environment for script
		init_environment();

		//load script values to environment
		luaL_loadfile(L, path.c_str());
		set_environment(); 
		if(lua_pcall(L, 0, 0, 0) != 0)
		{
			std::cout << "error running file " << path << ": " << lua_tostring(L, -1) << std::endl;
		}
	}
	
	//call function
	void call(lua_State* L, std::string function_name, std::vector<lua_arg> args)
	{
		get_function(function_name);

		//push arguments to stack. this is how they are passed to lua functions
		for(size_t i = 0; i < args.size(); i++)
		{
			lua_arg* temp = &args[i];
			switch(temp->type)
			{
				case LUA_TBOOLEAN:
					lua_pushboolean(L, temp->data.boolean);
				break;

				case LUA_TNUMBER:
					lua_pushnumber(L, temp->data.number);
				break;

				case LUA_TSTRING:
					lua_pushstring(L, temp->data.string);
				break;
			}
		}

		if(lua_pcall(L, args.size(), 0, 0) != 0)
		{
			std::cout << "Script.call(): call to function " << function_name << " has failed. " << lua_error(L) << "\n";
		}
	}
};