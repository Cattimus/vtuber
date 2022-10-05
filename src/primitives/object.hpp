#pragma once

#include <SDL.h>
#include "lua_bindings/script.hpp"
#include "primitives/texture.hpp"

class Object
{
protected:
	//translation data
	SDL_Rect origin;
	SDL_Rect offset;
	double rotation;
	SDL_Point rotation_axis;

	//script associated with object
	Script* script;
	
	//determines the order objects are drawn in. higher renders first
	int render_priority; 

	//helper functions
	void init(SDL_Rect& size);
	void init(int w, int h);

public:
	
	//constructors and operators
	explicit Object(SDL_Rect& size);
	Object(int w, int h);
	Object(int w, int h, Script* script);
	Object(SDL_Rect& size, Script* script);
	Object(const Object& to_copy);
	Object& operator=(const Object& to_copy);
	
	//virtual functions
	virtual void draw(SDL_Renderer* renderer);
	virtual void set_texture(Texture* to_change);

	//getters
	double get_width();
	double get_height();
	double get_rotation();
	SDL_Point get_rotation_axis();
	SDL_Rect get_position();
	SDL_Rect get_origin();
	SDL_Rect get_offset();
	int get_priority();
	bool clicked(double x, double y);

	//setters
	void set_rotation(double rot);
	void set_rotation_axis(double x, double y);
	void set_rotation_axis_pixels(double x, double y);
	void set_script(Script* script);
	void set_origin(SDL_Rect& new_origin);
	void set_origin(double x, double y);
	void set_offset(double x, double y);
	void set_priority(int val);

	//mutators
	void inc_priority();
	void dec_priority();
	void reset_position();
	void reset_rotation();
	void reset_size();
	void move_to(double x, double y);
	void relative_move(double x, double y);


	//debug function to print the object's current location
	void print_position()
	{
		SDL_Rect position = get_position();

		std::cout << "x: " << position.x;
		std::cout << " y: " << position.y;
		std::cout << std::endl;
	}
};
