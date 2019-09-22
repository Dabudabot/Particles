#include "Game.h"
#include <fstream>
#include <iostream>

particle::Game::Game()
{
  restore_defaults();
  screen_ = std::make_shared<Screen>();
  running_ = true;
  wall_building_ = false;
  show_help_ = false;
  help_fade_ = 0xff;
}

int particle::Game::run()
{
  if (!screen_->init())
  {
    return 1;
  }

  {
    // H
    wall_host_->start_wall(84, 52);
    wall_host_->end_wall(84, 258);
    wall_host_->start_wall(84, 258);
    wall_host_->end_wall(163, 258);
    wall_host_->start_wall(163, 258);
    wall_host_->end_wall(163, 180);
    wall_host_->start_wall(163, 180);
    wall_host_->end_wall(197, 180);
    wall_host_->start_wall(197, 180);
    wall_host_->end_wall(197, 258);
    wall_host_->start_wall(197, 258);
    wall_host_->end_wall(277, 258);
    wall_host_->start_wall(277, 258);
    wall_host_->end_wall(277, 52);
    wall_host_->start_wall(277, 52);
    wall_host_->end_wall(197, 52);
    wall_host_->start_wall(197, 52);
    wall_host_->end_wall(197, 124);
    wall_host_->start_wall(197, 124);
    wall_host_->end_wall(163, 124);
    wall_host_->start_wall(163, 124);
    wall_host_->end_wall(163, 52);
    wall_host_->start_wall(163, 52);
    wall_host_->end_wall(84, 52);

    // A
    wall_host_->start_wall(349, 52);
    wall_host_->end_wall(298, 258);
    wall_host_->start_wall(298, 258);
    wall_host_->end_wall(374, 258);
    wall_host_->start_wall(374, 258);
    wall_host_->end_wall(380, 225);
    wall_host_->start_wall(380, 225);
    wall_host_->end_wall(424, 225);
    wall_host_->start_wall(424, 225);
    wall_host_->end_wall(431, 258);
    wall_host_->start_wall(431, 258);
    wall_host_->end_wall(512, 258);
    wall_host_->start_wall(512, 258);
    wall_host_->end_wall(458, 52);
    wall_host_->start_wall(458, 52);
    wall_host_->end_wall(349, 52);

    // P
    wall_host_->start_wall(529, 52);
    wall_host_->end_wall(529, 258);
    wall_host_->start_wall(529, 258);
    wall_host_->end_wall(606, 258);
    wall_host_->start_wall(606, 258);
    wall_host_->end_wall(606, 191);
    wall_host_->start_wall(606, 191);
    wall_host_->end_wall(700, 191);
    wall_host_->start_wall(700, 191);
    wall_host_->end_wall(700, 52);
    wall_host_->start_wall(700, 191);
    wall_host_->end_wall(700, 52);
    wall_host_->start_wall(700, 52);
    wall_host_->end_wall(529, 52);

    // P
    wall_host_->start_wall(729, 52);
    wall_host_->end_wall(729, 258);
    wall_host_->start_wall(729, 258);
    wall_host_->end_wall(806, 258);
    wall_host_->start_wall(806, 258);
    wall_host_->end_wall(806, 191);
    wall_host_->start_wall(806, 191);
    wall_host_->end_wall(900, 191);
    wall_host_->start_wall(900, 191);
    wall_host_->end_wall(900, 52);
    wall_host_->start_wall(900, 191);
    wall_host_->end_wall(900, 52);
    wall_host_->start_wall(900, 52);
    wall_host_->end_wall(729, 52);

    // y
    wall_host_->start_wall(925, 52);
    wall_host_->end_wall(993, 184);
    wall_host_->start_wall(993, 184);
    wall_host_->end_wall(993, 258);
    wall_host_->start_wall(993, 258);
    wall_host_->end_wall(1073, 258);
    wall_host_->start_wall(1073, 258);
    wall_host_->end_wall(1073, 184);
    wall_host_->start_wall(1073, 184);
    wall_host_->end_wall(1143, 52);
    wall_host_->start_wall(1143, 52);
    wall_host_->end_wall(1060, 52);
    wall_host_->start_wall(1060, 52);
    wall_host_->end_wall(1036, 129);
    wall_host_->start_wall(1036, 129);
    wall_host_->end_wall(1014, 52);
    wall_host_->start_wall(1014, 52);
    wall_host_->end_wall(925, 52);

    // B
    wall_host_->start_wall(84, 280);
    wall_host_->end_wall(84, 500);
    wall_host_->start_wall(84, 500);
    wall_host_->end_wall(312, 500);
    wall_host_->start_wall(312, 500);
    wall_host_->end_wall(312, 397);
    wall_host_->start_wall(312, 397);
    wall_host_->end_wall(224  , 397);
    wall_host_->start_wall(224, 397);
    wall_host_->end_wall(224, 372);
    wall_host_->start_wall(224, 372);
    wall_host_->end_wall(302, 372);
    wall_host_->start_wall(302, 372);
    wall_host_->end_wall(302, 280);
    wall_host_->start_wall(302, 280);
    wall_host_->end_wall(84, 280);

    // D
    wall_host_->start_wall(363, 280);
    wall_host_->end_wall(363, 500);
    wall_host_->start_wall(363, 500);
    wall_host_->end_wall(594, 500);
    wall_host_->start_wall(594, 500);
    wall_host_->end_wall(594, 280);
    wall_host_->start_wall(594, 280);
    wall_host_->end_wall(363, 280);
    wall_host_->start_wall(484, 336);
    wall_host_->end_wall(484, 443);
    wall_host_->start_wall(484, 443);
    wall_host_->end_wall(537, 443);
    wall_host_->start_wall(537, 443);
    wall_host_->end_wall(537, 336);
    wall_host_->start_wall(537, 336);
    wall_host_->end_wall(484, 336);

    // A
    wall_host_->start_wall(700, 280);
    wall_host_->end_wall(625, 500);
    wall_host_->start_wall(625, 500);
    wall_host_->end_wall(720, 500);
    wall_host_->start_wall(720, 500);
    wall_host_->end_wall(728, 469);
    wall_host_->start_wall(728, 469);
    wall_host_->end_wall(799, 469);
    wall_host_->start_wall(799, 469);
    wall_host_->end_wall(808, 500);
    wall_host_->start_wall(808, 500);
    wall_host_->end_wall(911, 500);
    wall_host_->start_wall(911, 500);
    wall_host_->end_wall(828, 280);
    wall_host_->start_wall(828, 280);
    wall_host_->end_wall(700, 280);

    // Y
    wall_host_->start_wall(866, 280);
    wall_host_->end_wall(953, 421);
    wall_host_->start_wall(953, 421);
    wall_host_->end_wall(953, 500);
    wall_host_->start_wall(953, 500);
    wall_host_->end_wall(1050, 500);
    wall_host_->start_wall(1050, 500);
    wall_host_->end_wall(1050, 421);
    wall_host_->start_wall(1050, 421);
    wall_host_->end_wall(1143, 280);
    wall_host_->start_wall(1143, 280);
    wall_host_->end_wall(1037, 280);
    wall_host_->start_wall(1037, 280);
    wall_host_->end_wall(1007, 356);
    wall_host_->start_wall(1007, 356);
    wall_host_->end_wall(970, 280);
    wall_host_->start_wall(970, 280);
    wall_host_->end_wall(866, 280);
  }

  while (running_)
  {
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

    screen_->motion_blur();

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

    screen_->present();
  }

  return 0;
}

bool particle::Game::process_event(SDL_Event& event)
{
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
      restore_defaults();
      break;
    case SDLK_TAB:
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
    if (event.key.keysym.sym == SDLK_TAB)
    {
      show_help_ = false;
    }
    break;
  default:
    break;
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
  swarm_host_ = std::make_unique<SwarmHost>();
  wall_host_ = std::make_shared<WallHost>();
  wall_building_ = false;
  show_help_ = false;
  help_fade_ = 0x0;
}

void particle::Game::save(const char* filename) const
{
  // open file, create if not exist
  std::ofstream file("save.save", std::ios::out | std::ios::binary);
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
  std::ifstream file("save.save", std::ios::out | std::ios::binary);

  wall_host_->deserialize_walls(file);
  
  file.close();
}
