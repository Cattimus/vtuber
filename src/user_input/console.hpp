#pragma once

#include <iostream>
#include <vector>
#include <hash>
#include <thread>
using namespace std;

//User input manager from console (runs on separate thread)
class Console
{

private:
	thread input_thread; //thread that will handle IO
	
	//Each option level will have it's own hashed vector.
	//User input will be hashed and compared to the selection list for speed
	//The selection can be cleared by pressing esc
	//Objects that have been clicked should automatically be brought to the top of the render priority
	//all operations will be conducted on the selected object	
	

	public:

	
};
