#pragma once
#include <SDL.h>

namespace particle
{
  class Screen final
  {
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    SDL_Texture* texture_;

    Uint32* buffer_;// TODO why? buffer2_ front_buffer_ back_buffer_

    const static short red_shift = 24;
    const static short green_shift = 16;
    const static short blue_shift = 8;

  public:
    const static int screen_width = 800;
    const static int screen_height = 600;

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
    void update() const;
    void clear() const;

    static double to_relative(int abs_coord, int max);
    static int to_abs(double rel_coord, int max);
  };
}


