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
	//The selected object will be whatever is clicked with the mouse
	//The selection can be cleared by pressing esc
	//Objects should be given a render priority. This will determine whether they're rendered on top of or below other objects.	
	//Objects that have been clicked should automatically be brought to the top of the render priority
	
	

	public:
	
};
