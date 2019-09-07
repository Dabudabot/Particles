#include "Screen.h"
#include <ctime>
#include <cstdlib>

particle::Screen::Screen() :
	window_(nullptr),
	renderer_(nullptr),
	texture_(nullptr),
	buffer_(nullptr)
{
	srand(static_cast<unsigned int>(std::time(nullptr)));
}

particle::Screen::~Screen()
{
	delete[] buffer_;
	SDL_DestroyTexture(texture_);
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
	SDL_Quit();
}

void particle::Screen::set_pixel(const double x, const double y, const Uint32 color) const 
{
  const auto fix_x = static_cast<int>(x) * screen_width / 2;
  const auto fix_y = static_cast<int>(y) * screen_height / 2;

  set_pixel(fix_x, fix_y, color);
}

void particle::Screen::set_pixel(const int x, const int y, const Uint32 color) const
{
  if (x < 0 || x >= screen_width || y < 0 || y >= screen_height)
  {
    return;
  }
  buffer_[(y * screen_width) + x] = color;
}

void particle::Screen::set_background(const Uint32 color) const
{
	SDL_memset4(buffer_, color, screen_width*screen_height);
}

void particle::Screen::clear() const
{
  set_background(0x0);
}

void particle::Screen::update() const
{
	SDL_UpdateTexture(texture_, nullptr, buffer_, screen_width * sizeof(Uint32));
	SDL_RenderClear(renderer_);
	SDL_RenderCopy(renderer_, texture_, nullptr, nullptr);
	SDL_RenderPresent(renderer_);
}

bool particle::Screen::init()
{
	//check init
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		return false;
	}

	//create window
	window_ = SDL_CreateWindow(
		"Your Particles",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		screen_width,
		screen_height,
		SDL_WINDOW_SHOWN);

	//check window
	if (window_ == nullptr)
	{
		SDL_Quit();
		return false;
	}

	//create renderer
	renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_PRESENTVSYNC);

	//check renderer
	if (renderer_ == nullptr)
	{
		SDL_DestroyWindow(window_);
		SDL_Quit();
		return false;
	}

	//create texture
	texture_ = SDL_CreateTexture(
		renderer_,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STATIC,
		screen_width,
		screen_height);

	//check texture
	if (texture_ == nullptr)
	{
		SDL_DestroyRenderer(renderer_);
		SDL_DestroyWindow(window_);
		SDL_Quit();
		return false;
	}

	//create buffer
	buffer_ = new Uint32[screen_width*screen_height];
	clear();

	return true;
}
