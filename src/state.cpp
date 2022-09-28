#include "state.hpp"
//CONSTRUCTORS
State::State()
{
	delta = NULL;
	last_selected = NULL;
	renderer = NULL;
	return;
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
	return *this;
}

State::State(const State& to_copy)
{
	*this = to_copy;
}

//this will clean up unused textures
void State::reclaim_textures(string path)
{
	for(size_t i = 0; i < avatars.size(); i++)
	{
		if(avatars[i].get_texture()->get_path() == path)
		{
			return;	
		}
	}

	for(size_t i = 0; i < entities.size(); i++)
	{
		if(entities[i].get_texture()->get_path() == path)
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
		textures.erase(textures.begin() + index);
	}
}

//search texture for the specified file
Texture* State::search_textures(string path)
{
	for(size_t i = 0; i < textures.size(); i++)
	{
		if(textures[i].get_path() == path)
		{
			return &textures[i];
		}
	}

	return NULL;
}

//search the texture array for the specified texture (returns -1 on failure)
int State::tex_index(string path)
{
	for(size_t i = 0; i < textures.size(); i++)
	{
		if(textures[i].get_path() == path)
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
void State::new_entity(int w, int h, int x, int y, int priority, string tex_path)
{
	//fetch or create texture for file
	Texture* tex = search_textures(tex_path);
	if(!tex)
	{
		textures.push_back(Texture(tex_path.c_str(), renderer));
		tex = &textures[textures.size() - 1];
	}
	
	//create new entity and push it to the draw list
	entities.push_back(Entity((SDL_Rect){x,y,w,h}, tex));
	draw_list.push_back((Object*)&entities[entities.size() - 1]);
	sort_drawlist(); //sort the rendering array
}

//create a new avatar and push it to the proper arrays
void State::new_avatar(int w, int h, int x, int y, int priority, string tex_path)
{
	//fetch or create texture for file
	Texture* tex = search_textures(tex_path);
	if(!tex)
	{
		textures.push_back(Texture(tex_path.c_str(), renderer));
		tex = &textures[textures.size() - 1];
	}
	
	//create a new avatar and push it to the draw list
	avatars.push_back(Avatar((SDL_Rect){x,y,w,h}, tex));
	draw_list.push_back((Object*)&avatars[avatars.size() - 1]);
	sort_drawlist(); //sort the rendering array
}

//delete an entity from drawlist and entity list
void State::delete_entity(Object* ref)
{
	//erase from entity list
	for(size_t i = 0; i < entities.size(); i++)
	{
		if((Object*)(&entities[i]) == ref)
		{
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
