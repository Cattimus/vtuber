#pragma once

#include <iostream>
#include <vector>
#include <thread>
using namespace std;

#include "globals.hpp"

//User input manager from console (runs on separate thread)
class Console
{

private:
	//thread input_thread; //thread that will handle IO
	bool running;
	
	//[manipulation]
	//image  (path-to-image)    <- checks for existing instances of texture
	//size   (width, height)
	//origin
	//reset_position
	//reset_origin

	//[allocation]
	//new (type) (texture path) <- checks for instances of that texture
	//delete (prompt yes/no)
	
	//split the input string at every [to_split] character
	vector<string> strsplit(string input, char to_split)
	{
		size_t pos = 0;
		size_t at = 0;
		vector<string> to_return;

		while(at != string::npos)
		{
			at = input.find(to_split, at+1);
			to_return.push_back(input.substr(pos, at - pos));
			pos = at + 1;
		}
		
		return to_return;
	}
	/*
	//thread that will control grabbing user input
	void input_thread()
	{
		string input;
		
		//main input loop
		while(running)
		{

		getline(cin, input);
		vector<string> arg_list = strsplit(input, ' ');

		//replace texture of last selected object
		if(arg_list[0] == "image")
		{
			if(arg_list.size() < 2)
			{
				cout << "Not enough arguments. type 'help' for details.\n";
			}
			
			//skip the rest of the loop if selected object isn't set
			if(!last_selected)
			{
				continue;
			}

			//check if a texture has already been created for that file
			for(size_t i = 0; i < textures.size(); i++)
			{
				//if path matches, set texture and continue next loop
				if(textures[i].get_path() == arg_list[1])
				{
					last_selected->set_texture(&textures[i]);
					continue;
				}
			}
			
			//create a new texture for the file
			textures.push_back(Texture(arg_list[1].c_str()));
			last_selected->set_texture(&textures[textures.size() - 1]);
		}

		//change size of last selected object (x, y)
		else if(arg_list[0] == "size")
		{

		}

		//change the origin location of last selected object(to current position)
		else if(arg_list[0] == "origin")
		{

		}
		
		//reset the object's position to it's origin
		else if(arg_list[0] == "reset_position")
		{
		
		}

		//reset the object's origin to (0,0)
		else if(arg_list[0] == "reset_origin")
		{

		}

		//create a new object
		else if (arg_list[0] == "new")
		{

		}

		//delete an existing object
		else if(arg_list[0] == "delete")
		{

		}

		//display help data
		else if(arg_list[0] == "help")
		{
			//helptext menu here
		}
		else
		{
			cout << "Entered option was not found. Enter 'help' to see a list of options.\n";
		}

		}
	}
	*/

	public:
	
	
	
	
};
