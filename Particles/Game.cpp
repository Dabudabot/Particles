#include "Game.h"

particle::Game::Game()
{
  screen_ = std::make_shared<Screen>();
  swarm_host_ = std::make_unique<SwarmHost>();
  wall_host_ = std::make_unique<WallHost>();
  running_ = true;
  wall_building_ = false;
}

int particle::Game::run()
{
  if (!screen_->init())
  {
    return 1;
  }

  while (running_)
  {
    screen_->clear();

    // get time from the beginning
    const auto elapsed = SDL_GetTicks();

    // update all available swarms
    swarm_host_->update(elapsed);

    // all particles on the screen
    auto particles = swarm_host_->get_particles();

    // draw each particle
    for (const auto& particle : particles)
    {
      screen_->set_pixel(
        particle->get_x(),
        particle->get_y(),
        particle->get_color()
      );
    }

    wall_host_->draw_walls(screen_, true);

    // process clicks
    SDL_Event event;
    if (SDL_PollEvent(&event)) 
    {
      running_ = process_event(event);
    }

    // update screen buffer
    screen_->update();
  }

  return 0;
}

bool particle::Game::process_event(SDL_Event& event)
{
  if (SDL_QUIT == event.type)
  {
    return false;
  }

  if (wall_building_)
  {
    if (SDL_MOUSEBUTTONDOWN == event.type)
    {
      switch(event.button.button)
      {
      case SDL_BUTTON_LEFT:
        wall_host_->end_wall(event.button.x, event.button.y);
        wall_building_ = false;
        break;
      case SDL_BUTTON_RIGHT:
        wall_building_ = false;
        break;
      default:
        break;
      }
    } else if(SDL_MOUSEMOTION == event.type)
    {
      wall_host_->move_wall(screen_, event.button.x, event.button.y);
    }
  }
  else
  {
    if (SDL_MOUSEBUTTONDOWN == event.type)
    {
      switch (event.button.button)
      {
      case SDL_BUTTON_LEFT:
        wall_host_->start_wall(event.button.x, event.button.y);
        wall_building_ = true;
        break;
      case SDL_BUTTON_RIGHT:
        swarm_host_->generate_swarm(event.button.x, event.button.y);
        break;
      default:
        break;
      }
    }
  }

  return true;
}


