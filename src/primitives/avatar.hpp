#pragma once

#include "project_includes.hpp"
#include "object.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "entity.hpp"

//TODO - tie an entity to another entity (accessories)
//TODO - implement flipping image horizontally and vertically

class Avatar : public Object
{
private:
	//copies of origins for both values
	glm::vec3 top_origin;
	glm::vec3 bottom_origin;

	Entity* avatar_top;
	Entity* avatar_bottom;
	bool is_split;

	double talk_height = 0.25;

	//move both sub-objects by their origins. This means they can move freely
	void update_position()
	{
		if(!avatar_bottom)
		{
			avatar_top->set_origin(top_origin + get_position());
		}
		else
		{
			avatar_top->set_origin(top_origin + get_position());
			avatar_bottom->set_origin(bottom_origin + get_position());
		}
	}

public:

	//create new (unsplit) avatar
	Avatar(glm::vec3 size, Texture* texture) : Object(size)
	{
		this->is_split = false;
		this->top_origin = glm::vec3(0,0,0);
		this->bottom_origin = glm::vec3(0,0,0);

		this->avatar_top = new Entity(size, texture);
		avatar_bottom = NULL;
	}

	//copy constructor
	Avatar(const Avatar& to_copy) : Object(to_copy)
	{
		*this = to_copy;
	}

	//assignment operator
	Avatar& operator=(const Avatar& to_copy)
	{
		//self assignment guard
		if(this == &to_copy)
		{
			return *this;
		}

		Object::operator=(to_copy);
		this->avatar_top = NULL;
		this->avatar_bottom = NULL;
		this->is_split = false;
		this->talk_height = to_copy.talk_height;

		if(to_copy.avatar_top)
		{
			this->avatar_top = new Entity(*to_copy.avatar_top);
		}

		if(to_copy.avatar_bottom)
		{
			this->avatar_bottom = new Entity(*to_copy.avatar_bottom);
			this->is_split = true;
		}

		return *this;
	}

	~Avatar()
	{
		if(avatar_top)
		{
			delete avatar_top;
		}

		if(avatar_bottom)
		{
			delete avatar_bottom;
		}
	}

	//AABB collision detection
	bool clicked(double x, double y)
	{
		glm::vec3 current_pos = get_position();
		
		double xmin = current_pos.x - size.x;
		double xmax = current_pos.x + size.x;
		double ymin = current_pos.y - size.y;
		double ymax = current_pos.y + size.y;
		
		if((x >= xmin && x <= xmax) &&
		   (y >= ymin && y <= ymax))
		{
			return true;
		}

		return false;
	}

	void draw()
	{
		update_position();
		if(avatar_top)
		{
			avatar_top->draw();
		}

		if(avatar_bottom)
		{
			avatar_bottom->draw();
		}
	}

	//split the avatar along the selected line
	void split(double y)
	{
		if(!is_split)
		{
			//adjust y in respect to current position
			double yval = y - origin.y - offset.y;
			
			//reset origin so calculations can occur properly
			avatar_top->set_origin(glm::vec3(0,0,0));

			//calculate split
			avatar_bottom = new Entity(*avatar_top);
			avatar_top->split(yval, 1);
			avatar_bottom->split(yval, -1);

			//store position offsets
			top_origin = avatar_top->get_origin();
			bottom_origin = avatar_bottom->get_origin();

			//move top so split is visible
			//talk(1);
			
			is_split = true;
		}
	}

	//move top jaw
	void talk(double height)
	{
		if(!is_split)
		{
			return;
		}

		if(height < 0)
		{
			height = 0;
		}
		if(height > 1)
		{
			height = 1;
		}
		
		double move_val = height * talk_height;
		avatar_top->reset_position();
		avatar_top->relative_move(0, move_val);
	}

	//move all entities back to origin
	void reset_position()
	{	
		offset = glm::vec3(0,0,0);
	}

	void set_talk_height(double height)
	{
		talk_height = height;
	}

	double get_talk_height()
	{
		return talk_height;
	}

};
