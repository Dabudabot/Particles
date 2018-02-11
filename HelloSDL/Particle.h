#pragma once
#include <stdlib.h>
//#include <SDL.h>

struct Particle
{
public:
	double m_x;
	double m_y;
	//Uint32 color;

	double m_acceleration;
	double m_xspeed;
	double m_yspeed;

	Particle();
	~Particle();

	void update();
};

