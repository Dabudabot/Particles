/**
 * \author Dabudabot
 * \brief contain main game class
 */

#pragma once
#include "Screen.h"
#include "Swarm.h"
#include "Wall.h"

#include <memory>

/**
 * \brief main namespace of the project 
 */
namespace particle {
  /**
   * \brief controlling instance of the game
   */
  class Game
  {
    /**
     * \brief display of the game elements
     */
    std::shared_ptr<Screen> screen_;
    /**
     * \brief particles control
     */
    std::shared_ptr<SwarmHost> swarm_host_;
    /**
     * \brief walls control
     */
    std::shared_ptr<WallHost> wall_host_;
    /**
     * \brief is game running or end
     */
    bool running_;

    /**
     * \brief is we are in wall building mode (after click)
     */
    bool wall_building_;
    /**
     * \brief is we showing help elements (tab hold)
     */
    bool show_help_;

    /**
     * \brief fade counter of help elements
     */
    Uint8 help_fade_;

    /**
     * \brief name of the quick save file
     */
    const char* filename_ = "save.save";

  public:

    bool is_running() const { return running_; }
    void set_running(const bool is_running) { running_ = is_running; }
    bool is_wall_building() const { return wall_building_; }
    void set_wall_building(const bool is_building) { wall_building_ = is_building; }
    bool is_help_shown() const { return show_help_; }
    void set_help(const bool is_shown) { show_help_ = is_shown; }
    void set_help_fade(Uint8& help_fade) { help_fade_ = help_fade; }
    Uint8 get_help_fade() const { return help_fade_; }
    std::shared_ptr<Screen> get_screen() const { return screen_; }
    std::shared_ptr<SwarmHost> get_swarm_host() const { return swarm_host_; }
    std::shared_ptr<WallHost> get_wall_host() const { return wall_host_; }

    // delete unused ctors
    Game(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator= (const Game&) = delete;
    Game& operator= (Game&&) = delete;
    
    /**
     * \brief default ctor to init pointers
     */
    Game();
    Game(const std::shared_ptr<Screen>& screen, 
      const std::shared_ptr<SwarmHost>& swarm_host,
      const std::shared_ptr<WallHost>& wall_host);
    virtual ~Game() = default;

    /**
     * \brief add help elements to the screen
     */
    virtual void draw_help();

    /**
     * \brief start the game
     * \return 0 in case of success or error code
     */
    virtual int run();

    /**
     * \brief processing mouse and keyboard events
     * \param event event of the keyboard and mouse
     * \return true is game continues, false otherwise
     */
    virtual bool process_event(SDL_Event& event);
    /**
     * \brief jump to initial state
     */
    virtual void restore_defaults();
    /**
     * \brief open file for write and save walls position
     * \param filename name of the save file
     */
    virtual void save(const char* filename) const;
    /**
     * \brief open file for read and get walls position,
     *        current walls are destroyed
     * \param filename name of the file to read
     */
    virtual void load(const char* filename);
  };
}
