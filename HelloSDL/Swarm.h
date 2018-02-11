#pragma once
#include "Particle.h"

class Swarm
{
private:
	Particle *m_pParticles;
	
public:
	constexpr static int SWARM_SIZE = 500;

	Swarm();
	~Swarm();

	const Particle* const getParticles();
	void update();
};

