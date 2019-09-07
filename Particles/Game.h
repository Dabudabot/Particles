#pragma once
#include "Screen.h"
#include "SwarmHost.h"
#include "Wall.h"

#include <memory>

namespace particle {
  class Game
  {
    std::shared_ptr<Screen> screen_;
    std::unique_ptr<SwarmHost> swarm_host_;
    std::unique_ptr<WallHost> wall_host_;
    bool running_;

    bool process_event(SDL_Event& event);

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
