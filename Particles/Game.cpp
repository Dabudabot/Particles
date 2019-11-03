/**
 * \author Dabudabot
 */

#include "Game.h"
#include <fstream>

particle::Game::Game()
{
  Game::restore_defaults();
  screen_ = std::make_shared<Screen>();
  audio_ = std::make_shared<Audio>();
  running_ = true;
  wall_building_ = false;
  show_help_ = false;
  help_fade_ = 0xff;
}

int particle::Game::run()
{
  // if failed to init exit with error
  if (!screen_->init())
  {
    return 1;
  }

  if (!audio_->init())
  {
    return 2;
  }

  if (!audio_->play_music())
  {
    return 3;
  }

  // until game run do loop
  while (running_)
  {
    // clean screen
    screen_->clear();

    // get time from the beginning
    const auto elapsed = SDL_GetTicks();

    // update all available swarms
    swarm_host_->update(elapsed, wall_host_);

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

    // blur particles
    screen_->motion_blur();

    // draw walls if required
    wall_host_->draw_walls(screen_, !show_help_);

    // process clicks
    SDL_Event event;
    if (SDL_PollEvent(&event)) 
    {
      running_ = process_event(event);
    }

    // update screen buffer
    screen_->update();

    // draw overlay
    draw_help();

    // show updated texture
    screen_->present();
  }

  return 0;
}

bool particle::Game::process_event(SDL_Event& event)
{
  // end work
  if (SDL_QUIT == event.type)
  {
    return false;
  }

  switch(event.type)
  {
  case SDL_KEYDOWN:
    switch (event.key.keysym.sym)
    {
    case SDLK_ESCAPE:
      // esc resets the game
      this->restore_defaults();
      break;
    case SDLK_TAB:
      // tab shows the game
      show_help_ = true;
      break;
    case SDLK_F9:
      // quick save
      save(filename_);
      break;
    case SDLK_F10:
      // quick load
      load(filename_);
      break;
    default:
      break;
    }
    break;
  case SDL_KEYUP:
    // stop showing help
    if (event.key.keysym.sym == SDLK_TAB)
    {
      show_help_ = false;
    }
    break;
  default:
    break;
  }

  // after left click we are in wall building mode
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
    }
    else if(SDL_MOUSEMOTION == event.type)
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
        
        if (!wall_host_->is_overflow())
        {
          wall_host_->start_wall(event.button.x, event.button.y);
          wall_building_ = true;
        }
        break;
      case SDL_BUTTON_RIGHT:
        swarm_host_->generate_swarm(event.button.x, event.button.y);
        audio_->play_sound(event.button.x, event.button.y);
        break;
      default:
        break;
      }
    }
  }

  return true;
}

void particle::Game::draw_help()
{
  if (help_fade_ == 0 && !show_help_)
  {
    return;
  }

  if (show_help_)
  {
    help_fade_ = 0xff;
  }
  else
  {
    help_fade_ -= 0x3;
  }

  screen_->print_help(help_fade_);
}

void particle::Game::restore_defaults()
{
  swarm_host_ = std::make_shared<SwarmHost>();
  wall_host_ = std::make_shared<WallHost>();
  wall_building_ = false;
  show_help_ = false;
  help_fade_ = 0x0;
}

void particle::Game::save(const char* filename) const
{
  // open file, create if not exist
  std::ofstream file(filename, std::ios::out | std::ios::binary);
  if (!file) {
    return;
  }

  wall_host_->serialize_walls(file);
  
  file.close();
}

void particle::Game::load(const char* filename)
{
  restore_defaults();

  // open file, create if not exist
  std::ifstream file(filename, std::ios::out | std::ios::binary);

  wall_host_->deserialize_walls(file);
  
  file.close();
}
