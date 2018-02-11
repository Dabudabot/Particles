#include "Screen.h"

Screen::Screen() :
	m_window(nullptr),
	m_renderer(nullptr),
	m_texture(nullptr),
	m_buffer1(nullptr)
{
	srand(time(NULL));
}

Screen::~Screen()
{
	delete[] m_buffer1;
	SDL_DestroyTexture(m_texture);
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

Uint32 Screen::generateColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alfa)
{
	Uint32 color = 0;

	if (green > 255 || 
		blue > 255 || 
		red > 255 || 
		alfa > 255 || 
		green < 0 ||
		blue < 0 ||
		red < 0 ||
		alfa < 0)
	{
		return color;
	}

	color += alfa;
	color <<= 8;
	color += red;
	color <<= 8;
	color += green;
	color <<= 8;
	color += blue;

	return color;
}

void Screen::setPixel(int x, int y, Uint32 color)
{
	if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
	{
		return;
	}
	m_buffer1[(y * SCREEN_WIDTH) + x] = color;
}

void Screen::setBackgroud(Uint32 color)
{
	SDL_memset4(m_buffer1, color, SCREEN_WIDTH*SCREEN_HEIGHT);
}

void Screen::clear() 
{
	setBackgroud(0x0);
}

void Screen::update()
{
	SDL_UpdateTexture(m_texture, nullptr, m_buffer1, SCREEN_WIDTH * sizeof(Uint32));
	SDL_RenderClear(m_renderer);
	SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
	SDL_RenderPresent(m_renderer);
}

bool Screen::processEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			return false;
		}
	}
	return true;
}

bool Screen::init()
{
	//check init
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		return false;
	}

	//create window
	m_window = SDL_CreateWindow(
		"Valintine",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);

	//check window
	if (m_window == nullptr)
	{
		SDL_Quit();
		return false;
	}

	//create renderer
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);

	//check renderer
	if (m_renderer == nullptr)
	{
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}

	//create texture
	m_texture = SDL_CreateTexture(
		m_renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STATIC,
		SCREEN_WIDTH,
		SCREEN_HEIGHT);

	//check texture
	if (m_texture == nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}

	//create buffer
	m_buffer1 = new Uint32[SCREEN_WIDTH*SCREEN_HEIGHT];
	clear();

	return true;
}
