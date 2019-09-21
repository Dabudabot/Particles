#pragma once
#include "Screen.h"
#include "Swarm.h"
#include "Wall.h"

#include <memory>

namespace particle {
  class Game
  {
    std::shared_ptr<Screen> screen_;
    std::unique_ptr<SwarmHost> swarm_host_;
    std::shared_ptr<WallHost> wall_host_;
    bool running_;

    bool wall_building_;
    bool show_help_;

    Uint8 help_fade_;

    const char* filename_ = "save.dat";

    bool process_event(SDL_Event& event);
    void draw_help();
    void restore_defaults();

    void save(const char* filename) const;
    void load(const char* filename);

  public:

    // delete unused ctors
    Game(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator= (const Game&) = delete;
    Game& operator= (Game&&) = delete;
    
    Game();
    ~Game() = default;

    // start the game
    int run();
  };
}
