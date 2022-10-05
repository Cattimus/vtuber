#pragma once

#include <iostream>
#include <vector>
using namespace std;

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
	string file_path;
	string last_called;
	lua_State* L;

	//create a new environment and set it in globals
	void init_environment();

	//push environment on stack
	void get_environment();

public:

	//path is the path to the script. L is a reference to the current lua state.
	Script(std::string path, lua_State* L);

	//load function and push it to the lua stack. This should be called first when executing a function. Arguments should be pushed to the stack next.
	void load_function(std::string function_name);

	//call function. This should be called after load_function() and pushing arguments to the stack.
	int call(int arg_count);

	//return a reference to the lua_state the script has been created under.
	lua_State* get_state();
	string get_path();
};
