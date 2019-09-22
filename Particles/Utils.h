/**
 * \author Dabudabot
 * \brief contains common functions
 */

#pragma once

/**
* \brief main namespace of the project
*/
namespace particle
{
  /**
   * \brief generate color from rgb
   * \tparam T8 size of single color element
   * \tparam T32 size of color = T8*4
   * \param red red element
   * \param green green element
   * \param blue blue element
   * \return color
   */
  template<class T8, class T32>
  T32 generate_color(T8 red, T8 green, T8 blue)
  {
    T32 color = 0;

    if (green > 255 ||
      blue > 255 ||
      red > 255 ||
      green < 0 ||
      blue < 0 ||
      red < 0)
    {
      return color;
    }

    // alfa
    color += 0xff;
    color <<= 8;
    color += red;
    color <<= 8;
    color += green;
    color <<= 8;
    color += blue;

    return color;
  }

  /**
   * \brief generate color from rgba
   * \tparam T8 size of single color element
   * \tparam T32 size of color = T8*4
   * \param red red element
   * \param green green element
   * \param blue blue element
   * \param alfa alfa element
   * \return color
   */
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

  /**
   * \brief extract color elements from single color
   * \tparam T8 size of single color element
   * \tparam T32 size of color = T8*4
   * \param color sum of the rgb
   * \param red red element
   * \param green green element
   * \param blue blue element
   */
  template<class T8, class T32>
  void extract_color(T32 color, T8* red, T8* green, T8* blue)
  {
    *red = color >> 16;
    *green = color >> 8;
    *blue = color;
  }

  /**
   * \brief generate gray color
   * \tparam T8 size of single color element
   * \tparam T32 size of color = T8*4
   * \param gray tone of the gray
   * \return color
   */
  template<class T8, class T32>
  T32 generate_color(T8 gray)
  {
    return generate_color<T8, T32>(gray, gray, gray, gray);
  }
}
