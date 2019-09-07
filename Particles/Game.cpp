#include "Game.h"

particle::Game::Game()
{
  screen_ = std::make_shared<Screen>();
  swarm_host_ = std::make_unique<SwarmHost>();
  wall_host_ = std::make_unique<WallHost>();
  running_ = true;
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
  //TODO

  if (SDL_QUIT == event.type)
  {
    return false;
  }

  return true;
}


