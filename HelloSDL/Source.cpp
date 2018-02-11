#include "Screen.h"
#include <math.h>
#include "Swarm.h"

#include <iostream>
using namespace std;

int main(int argc, char * argv[])
{
	Screen screen;
	Swarm swarm;

	if (!screen.init()) 
	{
		return 1;
	}

	while (true) 
	{
		//upd particles
		//draw particles
		//listen
		screen.clear();
		swarm.update();
		
		int elapsed = SDL_GetTicks();
		auto red = (unsigned char)((1 + sin(elapsed * 0.0001)) * 128);
		auto green = (unsigned char)((1 + sin(elapsed * 0.0003)) * 128);
		auto blue = (unsigned char)((1 + sin(elapsed * 0.0005)) * 128);
		auto color = screen.generateColor(red, green, blue, 0xFF);
		
		auto particles = swarm.getParticles();
		for (int i = 0; i < Swarm::SWARM_SIZE; i++)
		{
			Particle particle = particles[i];
			int x = particle.m_x * Screen::SCREEN_WIDTH / 2;
			int y = particle.m_y * Screen::SCREEN_HEIGHT / 2;
			screen.setPixel(x, y, color);
		}

		//screen.setBackgroud(color);
		/*for (int x = 0; x < Screen::SCREEN_WIDTH; x++) 
		{
			for (int y = 0; y < Screen::SCREEN_HEIGHT; y++)
			{
				screen.setPixel(x, y, color);
			}
		}
*/
		screen.update();
		if (!screen.processEvents()) 
		{
			break;
		}
	}

	return 0;
}