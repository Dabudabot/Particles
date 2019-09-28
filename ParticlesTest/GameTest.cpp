#include "gtest/gtest.h"
#include "../Particles/Game.h"
#include <thread>
#include <SDL.h>

namespace particle
{
  TEST(GameTestRun, GameTestRun)
  {
    auto run_result = -1;
    Game game;
    SDL_Event event;

    event.type = SDL_QUIT;

    std::thread th {
      [&run_result, &game]()
      {
        run_result = game.run();
      }
    };

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);

    game.process_event(event);
    
    ASSERT_EQ(0, run_result);
  }
  
}
