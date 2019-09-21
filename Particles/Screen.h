#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

namespace particle
{
  class Screen final
  {
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    SDL_Texture* texture_;

    Uint32* buffer_;
    Uint32* blur_buffer_;

  public:
    const static int screen_width = 1200;
    const static int screen_height = 700;

    // delete unused ctors
    Screen(const Screen&) = delete;
    Screen(Screen&&) = delete;
    Screen& operator= (const Screen&) = delete;
    Screen& operator= (Screen&&) = delete;

    Screen();
    ~Screen();

    bool init();

    void set_pixel(int x, int y, Uint32 color) const;

    void set_background(Uint32 color) const;

    void set_text(const char* text, SDL_Color color, TTF_Font* font, int x, int y) const;
    void print_help(Uint8 fade) const;
    
    void present() const;
    void update() const;
    void clear() const;

    void motion_blur() const;

    static double to_relative(int abs_coord, int max);
    static int to_abs(double rel_coord, int max);
  };
}


