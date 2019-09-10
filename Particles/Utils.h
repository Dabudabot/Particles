#pragma once

namespace particle
{
  template<class T8, class T32>
  T32 generate_color(T8 red, T8 green, T8 blue, T8 alfa)
  {
    T32 color = 0;

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

  template<class T8, class T32>
  T32 generate_color(T8 gray)
  {
    return generate_color<T8, T32>(gray, gray, gray, gray);
  }
}
