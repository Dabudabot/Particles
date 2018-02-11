#pragma once
#include <SDL.h>
#include <iostream>
#include <time.h>

class Screen
{
private:
	SDL_Window *m_window;
	SDL_Renderer *m_renderer;
	SDL_Texture *m_texture;

	Uint32 *m_buffer1;
	Uint32 *m_buffer2;
	Uint32 *m_frontBuffer;
	Uint32 *m_backBuffer;

	const static short RED_SHIFT = 24;
	const static short GREEN_SHIFT = 16;
	const static short BLUE_SHIFT = 8;

public:
	const static int SCREEN_WIDTH = 800;
	const static int SCREEN_HEIGHT = 600;

	Screen();
	virtual ~Screen();

	bool init();
	bool processEvents();
	void setPixel(int x, int y, Uint32 color);
	void setBackgroud(Uint32 color);
	Uint32 generateColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alfa);
	void update();
	void clear();
};
