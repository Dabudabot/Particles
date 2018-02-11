#include "Particle.h"

Particle::Particle()
{
	m_x = (2.0 * rand()) / RAND_MAX;
	m_y = (2.0 * rand()) / RAND_MAX;
	//color = 0xFFFFFFFF;
	m_acceleration = 0.001;
	m_xspeed = m_acceleration * (((2.0 * rand()) / RAND_MAX) - 1);
	m_yspeed = m_acceleration * (((2.0 * rand()) / RAND_MAX) - 1);
}

Particle::~Particle()
{
}

void Particle::update() 
{
	m_x += m_xspeed;
	m_y += m_yspeed;

	if (m_x < 0 || m_x > 2) 
	{
		m_xspeed = -m_xspeed;
	}

	if (m_y < 0 || m_y > 2)
	{
		m_yspeed = -m_yspeed;
	}
}
