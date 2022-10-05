#include "script.hpp"

Script::Script(std::string path, lua_State* L)
{
	this->file_path = path;
	this->L = L;
	this->last_called = "";

	//create a new environment for script
	init_environment();

	//load script values to environment
	luaL_loadfile(L, path.c_str());
	get_environment();

	//set script to execute in sandbox
	if(!lua_setfenv(L, -2))
	{
		std::cout << "Script: sanboxing has failed. " << file_path << " will be running in the global namespace with no restrictions." << std::endl;
	}

	//execute lua file to define globals in the sandbox
	if(lua_pcall(L, 0, 0, 0) != 0)
	{
		std::cout << "error running file " << path << ": " << lua_tostring(L, -1) << std::endl;
	}
}

void Script::init_environment()
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

	//give access to math functions
	lua_pushstring(L, "math");
	lua_getglobal(L, "math");
	lua_settable(L, -3);

	//give access to print (for debugging purposes)
	lua_pushstring(L, "print");
	lua_getglobal(L, "print");
	lua_settable(L, -3);
	
	//set environment to the list of globals so it can be referenced later.
	lua_setglobal(L, (file_path + "env").c_str());
}

void Script::get_environment()
{
	lua_getglobal(L, (file_path + "env").c_str());
}

void Script::load_function(std::string function_name)
{
	get_environment();
	last_called = function_name;
	lua_getfield(L, -1, function_name.c_str());
	if(!lua_isfunction(L, -1))
	{
		std::cout << "Script.call(): " << function_name << " is not a defined function. " << lua_type(L, -1) << "\n";
		lua_pop(L, -1);
		lua_pop(L, -1);
		return;
	}

	//move function pointer below environment
	lua_insert(L, -2);

	//set script to execute in sandbox
	if(!lua_setfenv(L, -2))
	{
		std::cout << "Script: sanboxing has failed. " << file_path << " will be running in the global namespace with no restrictions." << std::endl;
	}
}

int Script::call(int arg_count)
{
	if(lua_pcall(L, arg_count, 0, 0) != 0)
	{
		std::cout << "Script.call(): call to function " << last_called << " has failed. " << lua_error(L) << "\n";
		return 0;
	}

	//successful call
	return 1;
}

lua_State* Script::get_state()
{
	return L;
}

string Script::get_path()
{
	return file_path;
}
