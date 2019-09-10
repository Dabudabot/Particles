#include "Screen.h"
#include <ctime>
#include <cstdlib>
#include <cstdio>

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
  TTF_Quit();
	SDL_DestroyTexture(texture_);
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
	SDL_Quit();
}

void particle::Screen::set_pixel(const int x, const int y, const Uint32 color) const
{
  if (x < 0 || x >= screen_width || y < 0 || y >= screen_height)
  {
    return;
  }
  buffer_[(y * screen_width) + x] = color;
}

void particle::Screen::set_text(
  const char* text,
  const SDL_Color color,
  TTF_Font* font,
  const int x,
  const int y) const
{
  const auto surface = TTF_RenderText_Solid(font, text, color);

  const auto texture = SDL_CreateTextureFromSurface(renderer_, surface);

  SDL_Rect rect; //create a rect
  rect.x = x;  //controls the rect's x coordinate 
  rect.y = y; // controls the rect's y coordinte
  rect.w = surface->w; // controls the width of the rect
  rect.h = surface->h; // controls the height of the rect

  SDL_FreeSurface(surface);

  SDL_RenderCopy(renderer_, texture, nullptr, &rect);
  SDL_DestroyTexture(texture);
}

void particle::Screen::print_help(const Uint8 fade) const
{
  const SDL_Color color = { fade, fade, fade, fade };
  const auto font = TTF_OpenFont("impact.ttf", 24);

  if (!font) return;

  set_text("esc", color, font, 50, 50);
  set_text("lbm", color, font, 50, 75);
  set_text("rbm", color, font, 50, 100);
  set_text("tab", color, font, 50, 125);

  set_text("- reset", color, font, 100, 50);
  set_text("- explode", color, font, 100, 75);
  set_text("- start/end wall", color, font, 100, 100);
  set_text("- show walls & help", color, font, 100, 125);

  SDL_RenderPresent(renderer_);
}

void particle::Screen::set_background(const Uint32 color) const
{
	SDL_memset4(buffer_, color, screen_width*screen_height);
}

void particle::Screen::present() const
{
  SDL_RenderPresent(renderer_);
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
}

bool particle::Screen::init()
{
	//check init
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		return false;
	}

  if (TTF_Init() < 0) {
    
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

double particle::Screen::to_relative(const int abs_coord, const int max)
{
  return static_cast<double>((2.0 * abs_coord) / max);
}

int particle::Screen::to_abs(const double rel_coord, const int max)
{
  return static_cast<int>(rel_coord * max / 2.0);
}