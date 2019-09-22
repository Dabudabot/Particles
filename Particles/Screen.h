/**
 * \author Dabudabot
 * \brief contain screen drawing functionality
 */

#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

/**
* \brief main namespace of the project
*/
namespace particle
{
  /**
   * \brief class to draw elements on screen
   */
  class Screen final
  {
    /**
     * \brief windows of the game
     */
    SDL_Window* window_;
    /**
     * \brief rendering engine
     */
    SDL_Renderer* renderer_;
    /**
     * \brief main texture of the game
     */
    SDL_Texture* texture_;

    /**
     * \brief texture buffer
     */
    Uint32* buffer_;
    /**
     * \brief back buffer for motion blur
     */
    Uint32* blur_buffer_;

  public:
    /**
     * \brief game window width
     */
    const static int screen_width = 1200;
    /**
     * \brief game window height
     */
    const static int screen_height = 700;

    // delete unused ctors
    Screen(const Screen&) = delete;
    Screen(Screen&&) = delete;
    Screen& operator= (const Screen&) = delete;
    Screen& operator= (Screen&&) = delete;

    /**
     * \brief set pointers to null and init random
     */
    Screen();
    /**
     * \brief de init SDL lib
     */
    ~Screen();

    /**
     * \brief init SDL lib
     * \return true if init success, false otherwise
     */
    bool init();

    /**
     * \brief set color pixel to the texture buffer
     * \param x width coordinate
     * \param y height coordinate
     * \param color color of the pixel
     */
    void set_pixel(int x, int y, Uint32 color) const;

    /**
     * \brief set all texture buffer to one color
     * \param color color of all pixels
     */
    void set_background(Uint32 color) const;

    /**
     * \brief draw text over texture buffer
     * \param text string to write
     * \param color color of the text
     * \param font size and font of the text
     * \param x width coordinate
     * \param y height coordinate
     */
    void set_text(const char* text, SDL_Color color, TTF_Font* font, int x, int y) const;
    /**
     * \brief print help message over the screen
     * \param fade count of fade to black
     */
    void print_help(Uint8 fade) const;

    /**
     * \brief show current texture
     */
    void present() const;
    /**
     * \brief update pixels
     */
    void update() const;
    /**
     * \brief remove all pixels
     */
    void clear() const;

    /**
     * \brief motion blur of texture buffer
     */
    void motion_blur() const;

    /**
     * \brief cast absolute coordinate to relative
     * \param abs_coord absolute coordinate
     * \param max maximum available coordinate in this dimension
     * \return relative coordinate
     */
    static double to_relative(int abs_coord, int max);
    /**
     * \brief cast relative coordinate to absolute
     * \param rel_coord relative coordinate
     * \param max maximum available coordinate in this dimension
     * \return absolute coordinate
     */
    static int to_abs(double rel_coord, int max);
  };
}


