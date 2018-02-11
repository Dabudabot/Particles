#include "Swarm.h"

Swarm::Swarm()
{
	m_pParticles = new Particle[SWARM_SIZE];
}

Swarm::~Swarm()
{
	delete[] m_pParticles;
}

const Particle* const Swarm::getParticles()
{
	return m_pParticles;
}

void Swarm::update()
{
	for (int i = 0; i < SWARM_SIZE; i++)
	{
		m_pParticles[i].update();
	}
}
