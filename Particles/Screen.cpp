/**
 * \author Dabudabot
 */

#include "Screen.h"
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include "Utils.h"

#include <Windows.h>

particle::Screen::Screen() :
  window_(nullptr),
  renderer_(nullptr),
  texture_(nullptr),
  buffer_(nullptr),
  blur_buffer_(nullptr),
  screen_width(0),
  screen_height(0)
{
	srand(static_cast<unsigned int>(std::time(nullptr)));
}

particle::Screen::~Screen()
{
  delete[] buffer_;
  delete[] blur_buffer_;
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
  rect.y = y; // controls the rect's y coordinate
  rect.w = surface->w; // controls the width of the rect
  rect.h = surface->h; // controls the height of the rect

  SDL_FreeSurface(surface);

  SDL_RenderCopy(renderer_, texture, nullptr, &rect);
  SDL_DestroyTexture(texture);
}

void particle::Screen::print_help(const Uint8 fade) const
{
  const SDL_Color color = { fade, fade, fade, fade };
  const auto font = TTF_OpenFont(R"(..\font\impact.ttf)", 24);

  if (!font) return;

  set_text("space", color, font, 50, 50);
  set_text("lmb", color, font, 50, 75);
  set_text("rmb", color, font, 50, 100);
  set_text("tab", color, font, 50, 125);
  set_text("F8", color, font, 50, 150);
  set_text("F9 ", color, font, 50, 175);
  set_text("F10", color, font, 50, 200);
  set_text("esc", color, font, 50, 225);

  set_text("- reset", color, font, 120, 50);
  set_text("- explode", color, font, 100, 75);
  set_text("- start/end wall", color, font, 100, 100);
  set_text("- show walls & help", color, font, 100, 125);
  set_text("- autoplay", color, font, 100, 150);
  set_text("- quick save", color, font, 100, 175);
  set_text("- quick load", color, font, 100, 200);
  set_text("- exit", color, font, 100, 225);

  SDL_RenderPresent(renderer_);
}

void particle::Screen::set_background(const Uint32 color) const
{
	SDL_memset4(buffer_, color, size_t(screen_width)*screen_height);
}

void particle::Screen::present() const
{
  SDL_RenderPresent(renderer_);
}

void particle::Screen::clear() const
{
  set_background(0x0);
}

void particle::Screen::motion_blur() const
{
  for (auto y = 0; y < screen_height; y++)
  {
    for (auto x = 0; x < screen_width; x++)
    {
      Uint8 red = 0;
      Uint8 green = 0;
      Uint8 blue = 0;

      Uint8 red2 = 0;
      Uint8 green2 = 0;
      Uint8 blue2 = 0;

      const Uint8 k = 0xf;

      // get pixel from blur buffer
      extract_color<Uint8, Uint32>(blur_buffer_[(y * screen_width) + x], &red, &green, &blue);

      // fade it
      red < k ? red = 0 : red -= k;
      green < k ? green = 0 : green -= k;
      blue  < k ? blue = 0 : blue -= k;

      // get pixel from current buffer
      extract_color<Uint8, Uint32>(buffer_[(y * screen_width) + x], &red2, &green2, &blue2);

      // set color
      if (red2 == 0 && green2 == 0 && blue2 == 0 && !(red == 0 && green == 0 && blue == 0))
      {
        buffer_[(y * screen_width) + x] = generate_color<Uint8, Uint32>(red, green, blue);
      }
    }
  }

  // copy to blur buffer
  SDL_memcpy4(blur_buffer_, buffer_, size_t(screen_width) * screen_height);
}

void particle::Screen::update() const
{
	SDL_UpdateTexture(texture_, nullptr, buffer_, unsigned int(screen_width) * sizeof(Uint32));
  SDL_RenderClear(renderer_);
	SDL_RenderCopy(renderer_, texture_, nullptr, nullptr);
}

bool particle::Screen::init()
{
	//check init
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
    MessageBox(
      nullptr,
      LPCSTR("SDL_INIT_EVERYTHING failed"),
      LPCSTR("Screen init"), 
      MB_OK
    );

		return false;
	}

  if (TTF_Init() < 0) 
  {
    MessageBox(
      nullptr,
      LPCSTR("TTF_Init failed"),
      LPCSTR("Screen init"),
      MB_OK
    );

    return false;
  }

#if 1
	//create window
	window_ = SDL_CreateWindow(
		"Particles",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		screen_width,
		screen_height,
    SDL_WINDOW_FULLSCREEN_DESKTOP);
#else//create window
  window_ = SDL_CreateWindow(
    "Particles",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    1000,
    600,
    SDL_WINDOW_SHOWN);
#endif

  SDL_GetWindowSize(window_, &screen_width, &screen_height);

	//check window
	if (window_ == nullptr)
	{
    MessageBox(
      nullptr,
      LPCSTR("Window not created"),
      LPCSTR("Screen init"),
      MB_OK
    );

		SDL_Quit();
		return false;
	}

	//create renderer
	renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_PRESENTVSYNC);

	//check renderer
	if (renderer_ == nullptr)
	{
    MessageBox(
      nullptr,
      LPCSTR("Renderer error"),
      LPCSTR("Screen init"),
      MB_OK
    );

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
    MessageBox(
      nullptr,
      LPCSTR("Texture error"),
      LPCSTR("Screen init"),
      MB_OK
    );

		SDL_DestroyRenderer(renderer_);
		SDL_DestroyWindow(window_);
		SDL_Quit();
		return false;
	}

	//create buffer
	buffer_ = new Uint32[size_t(screen_width)*screen_height];
  blur_buffer_ = new Uint32[size_t(screen_width) * screen_height];
  SDL_memset4(buffer_, 0, size_t(screen_width) * screen_height);
  SDL_memset4(blur_buffer_, 0, size_t(screen_width) * screen_height);

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