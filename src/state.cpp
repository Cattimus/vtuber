#include "state.hpp"
//CONSTRUCTORS
State::State()
{
	last_selected = NULL;
	delta = NULL;
	renderer = NULL;
	L = NULL;
}

State& State::operator=(const State& to_copy)
{
	//self-assignment guard
	if(this == &to_copy)
	{
		return *this;
	}

	avatars = to_copy.avatars;
	entities = to_copy.entities;
	scripts = to_copy.scripts;
	textures = to_copy.textures;
	draw_list = to_copy.draw_list; //this causes problems. This will need to be reconstructed.
	last_selected = to_copy.last_selected;
	delta = to_copy.delta;
	renderer = to_copy.renderer;
	L = to_copy.L;
	return *this;
}

State::State(const State& to_copy)
{
	*this = to_copy;
}

State::~State()
{
	for(auto val : avatars)
	{
		delete val;
	}

	for(auto val : entities)
	{
		delete val;
	}

	for(auto val : scripts)
	{
		delete val;
	}

	for(auto val: textures)
	{
		delete val;
	}
}

//this will clean up unused textures
void State::reclaim_textures(string path)
{
	for(size_t i = 0; i < avatars.size(); i++)
	{
		if(avatars[i]->get_texture()->get_path() == path)
		{
			return;	
		}
	}

	for(size_t i = 0; i < entities.size(); i++)
	{
		if(entities[i]->get_texture()->get_path() == path)
		{
			return;
		}
	}
	
	//if execution has gotten to this point, nothing has a reference to the texture
	delete_texture(path);
}

//delete the texture for the file from the list
void State::delete_texture(string path)
{
	int index = tex_index(path);

	if(index > -1)
	{
		delete[] textures[index];
		textures.erase(textures.begin() + index);
	}
}

//search texture for the specified file
Texture* State::search_textures(string path)
{
	for(size_t i = 0; i < textures.size(); i++)
	{
		if(textures[i]->get_path() == path)
		{
			return textures[i];
		}
	}

	return NULL;
}

//search the texture array for the specified texture (returns -1 on failure)
int State::tex_index(string path)
{
	for(size_t i = 0; i < textures.size(); i++)
	{
		if(textures[i]->get_path() == path)
		{
			return i;
		}
	}

	return -1;
}

//helper function for sort drawlist
static bool drawlist_comp(Object* a, Object* b)
{
	return a->get_priority() < b->get_priority();
}

//sort the list of drawable objects in render priority order
void State::sort_drawlist()
{
	sort(draw_list.begin(), draw_list.end(), drawlist_comp);
}

//create a new entity and push it to the proper arrays
Entity* State::new_entity(int x, int y,
						  int w, int h,
						  int priority,
						  string tex_path)
{
	Entity* to_return = NULL;

	//fetch or create texture for file
	Texture* tex = new_texture(tex_path);

	//create new entity and push it to the draw list
	entities.push_back(new Entity((SDL_Rect){x,y,w,h}, tex));
	to_return = entities[entities.size() - 1];
	draw_list.push_back((Object*)to_return);

	sort_drawlist(); //sort the rendering array
	return to_return; //return a reference to the new entity
}

//create a new avatar and push it to the proper arrays
Avatar* State::new_avatar(int x, int y,
						  int w, int h,
						  int priority,
						  string tex_path,
						  string script_path)
{
	Avatar* to_return = NULL;
	
	//fetch or create texture for file
	Texture* tex = new_texture(tex_path);
	
	//fetch or create script for file
	Script* script = NULL;
	if(script_path != "")
	{
		script = new_script(script_path);
	}	
	
	//create a new avatar and push it to the draw list
	avatars.push_back(new Avatar((SDL_Rect){x,y,w,h}, tex));
	to_return = avatars[avatars.size() - 1];
	draw_list.push_back((Object*)to_return);
	sort_drawlist(); //sort the rendering array

	//attach script to avatar
	if(script)
	{
		to_return->set_script(script);
	}

	return to_return;
}

//create new texture from file
Texture* State::new_texture(string path)
{
	Texture* to_return = search_textures(path);

	if(!to_return && renderer != NULL)
	{
		textures.push_back(new Texture(path.c_str(), renderer));
		to_return = textures[textures.size() - 1];
	}

	return to_return;
}

//create a new script or find a reference to existing script
Script* State::new_script(string path)
{
	Script* to_return = search_scripts(path);
	
	if(!to_return && L != NULL)
	{
		scripts.push_back(new Script(path.c_str(), L));
		to_return = scripts[scripts.size() - 1];
	}

	return to_return;
}

//search scripts array for script that shares the same path
Script* State::search_scripts(string path)
{
	Script* to_return = NULL;

	for(size_t i = 0; i < scripts.size(); i++)
	{
		if(scripts[i]->get_path() == path)
		{
			to_return = scripts[i];
			return to_return;
		}
	}

	return NULL;
}

//search the scripts array for the index
int State::script_index(string path)
{
	for(size_t i = 0; i < scripts.size(); i++)
	{
		if(scripts[i]->get_path() == path)
		{
			return i;
		}
	}

	return -1;
}

//delete an entity from drawlist and entity list
void State::delete_entity(Object* ref)
{
	//erase from entity list
	for(size_t i = 0; i < entities.size(); i++)
	{
		if((Object*)(entities[i]) == ref)
		{
			delete[] entities[i];
			entities.erase(entities.begin()+i);
			break;
		}
	}
	
	//erase from draw list
	for(size_t i = 0; i < draw_list.size(); i++)
	{
		if(draw_list[i] == ref)
		{
			draw_list.erase(draw_list.begin()+i);
			break;
		}
	}
}

//delete an avatar from the list and drawlist
void State::delete_avatar(Object* ref)
{
	//erase from avatar list
	for(size_t i = 0; i < avatars.size(); i++)
	{
		if((Object*)(&avatars[i]) == ref)
		{
			delete[] avatars[i];
			avatars.erase(avatars.begin()+i);
		}
	}

	//erase from draw list
	for(size_t i = 0; i < draw_list.size(); i++)
	{
		if(draw_list[i] == ref)
		{
			draw_list.erase(draw_list.begin()+i);
			break;
		}
	}
}
