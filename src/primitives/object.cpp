#include "object.hpp"

void Object::init(SDL_Rect& size)
{
	origin.x = size.x;
	origin.y = size.y;

	offset.x = 0;
	offset.y = 0;

	this->offset.w = size.w;
	this->offset.h = size.h;
	this->origin.w = size.w;
	this->origin.h = size.h;

	this->rotation_axis.x = size.w / 2;
	this->rotation_axis.y = size.h / 2;
	rotation = 0;

	this->script = NULL;

	this->render_priority = 0;
}

void Object::init(int w, int h)
{
	SDL_Rect temp;
	temp.w = w;
	temp.h = h;
	temp.x = 0;
	temp.y = 0;

	init(temp);
}


Object::Object(SDL_Rect& size)
{	
	init(size);
}

//w = width. h = height
Object::Object(int w, int h)
{
	init(w, h);
}

//w = width. h = height. script is a lua script object
Object::Object(int w, int h, Script* script)
{
	init(w, h);

	this->script = script;
}

//size is an SDL_Rect that contains x, y, w, h. script is a lua script object
Object::Object(SDL_Rect& size, Script* script)
{
	init(size);

	this->script = script;
}

Object::Object(const Object& to_copy)
{
	*this = to_copy;
}

Object& Object::operator=(const Object& to_copy)
{
	//self assignment gaurd
	if(this == &to_copy)
	{
		return *this;
	}

	//copy data
	this->origin = to_copy.origin;
	this->offset = to_copy.offset;
	this->rotation = to_copy.rotation;
	this->rotation_axis = to_copy.rotation_axis;
	this->script = to_copy.script;
	this->render_priority = to_copy.render_priority;

	return *this;
}

//draw function that can be overridden by child classes
void Object::draw(SDL_Renderer* renderer)
{
	return;
}

//function that will be defined by child classes to change textures
void Object::set_texture(Texture* to_change)
{
	return;
}

//AABB collision detection
bool Object::clicked(double x, double y)
{
	SDL_Rect current_pos = get_position();
	
	double xmin = current_pos.x;
	double xmax = current_pos.x + offset.w;
	double ymin = current_pos.y;
	double ymax = current_pos.y + offset.h;
	
	if((x >= xmin && x <= xmax) &&
	   (y >= ymin && y <= ymax))
	{
		return true;
	}

	return false;
}

//argument will be between 0 and 1. This will calculate to a point inside the object.
void Object::set_rotation_axis(double x, double y)
{
	rotation_axis.x = offset.w * x;
	rotation_axis.y = offset.h * y;

	if(rotation_axis.x > offset.w)
	{
		rotation_axis.x = offset.w;
	}

	if(rotation_axis.y > offset.h)
	{
		rotation_axis.y = offset.h;
	}
}

//argument will be x and y coordinates in screen space pixels
void Object::set_rotation_axis_pixels(double x, double y)
{
	rotation_axis.x = x;
	rotation_axis.y = y;
}

//return the current location on the screen of the object (origin + position)
SDL_Rect Object::get_position()
{
	SDL_Rect output;
	output.x = origin.x + offset.x;
	output.y = origin.y + offset.y;
	output.w = offset.w;
	output.h = offset.h;
	return output;
}

void Object::inc_priority()
{
	render_priority = render_priority + 1;
}

void Object::dec_priority()
{
	render_priority = render_priority - 1;
}

//return position to origin
void Object::reset_position()
{
	offset.x = 0;
	offset.y = 0;
}

//reset rotation to 0 degrees
void Object::reset_rotation()
{
	rotation = 0;
}

//assign an already loaded script to the object
void Object::set_script(Script* script)
{
	this->script = script;
}

void Object::reset_size()
{
	offset.w = origin.w;
	offset.h = origin.h;
}

//update origin values
void Object::set_origin(SDL_Rect& new_origin)
{
	origin = new_origin;
}

//update origin values (x and y)
void Object::set_origin(double x, double y)
{
	origin.x = x;
	origin.y = y;
}

//update offset values
void Object::set_offset(double x, double y)
{
	offset.x = x;
	offset.y = y;
}

//move to absolute location on the screen
void Object::move_to(double x, double y)
{
	double adjusted_x = x - origin.x;
	double adjusted_y = y - origin.y;
	
	offset.x = adjusted_x;
	offset.y = adjusted_y;
}

//move in respect to the current position
void Object::relative_move(double x, double y)
{
	offset.x += x;
	offset.y += y;
}

void Object::set_priority(int val)
{
	render_priority = val;
}

//misc getters 
double Object::get_width(){return offset.w;}
double Object::get_height(){return offset.y;}
double Object::get_rotation(){return rotation;}
SDL_Point Object::get_rotation_axis(){return rotation_axis;}
SDL_Rect Object::get_origin(){return origin;}
SDL_Rect Object::get_offset(){return offset;}
int Object::get_priority(){return render_priority;}

//misc setters
void Object::set_rotation(double rot){rotation = rot;}

