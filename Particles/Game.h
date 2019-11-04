/**
 * \author Dabudabot
 * \brief contain main game class
 */

#pragma once
#include "Screen.h"
#include "Swarm.h"
#include "Wall.h"
#include "Audio.h"

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
     * \brief sound of the game elements
     */
    std::shared_ptr<Audio> audio_;
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
     * \brief is we showing is autoplay enabled
     */
    bool is_autoplay_;
    /**
     * \brief fade counter of help elements
     */
    Uint8 help_fade_;

    Uint32 time_;

    Uint8 current_file_;

    std::vector<std::string> files;

    /**
     * \brief name of the quick save file
     */
    const char* filename_ = "../saves/save.save";
    const char* save_dir_ = "../saves";

    /**
     * \brief add help elements to the screen
     */
    void draw_help();

    /**
     * \brief processing mouse and keyboard events
     * \param event event of the keyboard and mouse
     * \return true is game continues, false otherwise
     */
    bool process_event(SDL_Event& event);

    /**
     * \brief jump to initial state
     */
    void restore_defaults();

    void autoplay(Uint32 elapsed);

    void load_all();

  public:

    // delete unused ctors
    Game(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator= (const Game&) = delete;
    Game& operator= (Game&&) = delete;
    
    /**
     * \brief default ctor to init pointers
     */
    Game();
    ~Game() = default;

    /**
     * \brief start the game
     * \return 0 in case of success or error code
     */
    int run();

    /**
     * \brief open file for write and save walls position
     * \param filename name of the save file
     */
    void save(const char* filename) const;

    /**
     * \brief open file for read and get walls position,
     *        current walls are destroyed
     * \param filename name of the file to read
     */
    void load(const char* filename);
  };
}
