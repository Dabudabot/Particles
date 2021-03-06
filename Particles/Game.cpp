/**
 * \author Dabudabot
 */

#include "Game.h"
#include <fstream>
#include <filesystem>
#include <string>

particle::Game::Game()
{
  screen_ = std::make_shared<Screen>();
  audio_ = std::make_shared<Audio>();

  swarm_host_ = std::make_shared<SwarmHost>();
  wall_host_ = std::make_shared<WallHost>();
  wall_building_ = false;
  show_help_ = false;
  is_autoplay_ = false;
  help_fade_ = 0x0;
  time_ = 0x0;
  current_file_ = 0x0;
  filename_ = "../saves/save.save";
  side_ = 0x0;
  running_ = true;
  load_all();
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

  // ReSharper disable once CppExpressionWithoutSideEffects
  audio_->play_music();

  SDL_ShowCursor(0);

  // until game run do loop
  while (running_)
  {
    // clean screen
    screen_->clear();

    // get time from the beginning
    const auto elapsed = SDL_GetTicks();

    if (is_autoplay_) autoplay(elapsed);
    
    // update all available swarms
    swarm_host_->update(elapsed, wall_host_);

    // all particles on the screen
    auto particles = swarm_host_->get_particles();

    // draw each particle
    for (const auto& particle : particles)
    {
      if (particle->dead()) continue;

      screen_->set_pixel(
        particle->get_x(screen_->screen_width),
        particle->get_y(screen_->screen_height),
        particle->get_color()
      );
    }

    // blur particles
    screen_->motion_blur();

    // draw walls if required
    wall_host_->draw_walls(
      screen_, 
      !show_help_, 
      screen_->screen_width, 
      screen_->screen_height
    );

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
      return false;
    case SDLK_SPACE:
      // esc resets the game
      this->restore_defaults();
      break;
    case SDLK_TAB:
      // tab shows the game
      show_help_ = true;
      break;
    case SDLK_F9:
      // quick save
      save();
      break;
    case SDLK_F10:
      // quick load
      load(filename_);
      break;
    case SDLK_F8:
      if (!is_autoplay_) is_autoplay_ = true;
      else is_autoplay_ = false;
      break;
    default:
      break;
    }
    break;
  case SDL_KEYUP:
    // stop showing help
    switch (event.key.keysym.sym)
    {
    case SDLK_TAB:
      show_help_ = false;
      break;
    default:
      break;
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
        wall_host_->end_wall(
          event.button.x, event.button.y, 
          screen_->screen_width, screen_->screen_height);
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
      wall_host_->move_wall(screen_, 
        event.button.x, event.button.y, 
        screen_->screen_width, screen_->screen_height);
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
          wall_host_->start_wall(
            event.button.x, 
            event.button.y, 
            screen_->screen_width,
            screen_->screen_height
          );
          wall_building_ = true;
        }
        break;
      case SDL_BUTTON_RIGHT:
        swarm_host_->generate_swarm(
          event.button.x, 
          event.button.y, 
          screen_->screen_width, 
          screen_->screen_height
        );
        audio_->play_sound(
          event.button.x, 
          event.button.y, 
          screen_->screen_width, 
          screen_->screen_height
        );
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
    SDL_ShowCursor(0);
    return;
  }

  if (show_help_)
  {
    SDL_ShowCursor(1);
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
  current_file_ = 0x0;
}

void particle::Game::save()
{
  // open file, create if not exist
  if (current_file_ >= 10) current_file_ = 0x0;
  std::string filename;
  filename += save_dir_;
  filename.append("\\");
  filename.append(std::to_string(current_file_));
  filename.append(".save");

  filename_ = filename;

  current_file_++;

  std::ofstream file(filename, std::ios::out | std::ios::binary);
  if (!file) {
    return;
  }

  wall_host_->serialize_walls(file);
  
  file.close();
}

void particle::Game::load(std::string& filename)
{
  restore_defaults();

  // open file, create if not exist
  std::ifstream file(filename, std::ios::out | std::ios::binary);

  wall_host_->deserialize_walls(file);
  
  file.close();
}

void particle::Game::autoplay(const Uint32 elapsed)
{
  if (time_ == 0 || elapsed - time_ > 60000)
  {
    time_ = elapsed;
    load(files[current_file_]);
    current_file_++;
    if (current_file_ >= files.size()) current_file_ = 0;
    if (side_ >= 5) side_ = 0x0;
    else side_++;
  }

  srand(elapsed);
  const auto r = (rand() % 300) + 1;

  if (1 == r)
  {
    int x;
    int y;

    switch(side_)
    {
    case 0:
      x = (rand() % (screen_->screen_width / 2)) + 1;
      y = (rand() % (screen_->screen_height / 2)) + 1;
      break;
    case 1:
      x = (rand() % (screen_->screen_width / 2)) + (screen_->screen_width / 2);
      y = (rand() % (screen_->screen_height / 2)) + 1;
      break;
    case 2:
      x = (rand() % (screen_->screen_width / 2)) + (screen_->screen_width / 2);
      y = (rand() % (screen_->screen_height / 2)) + (screen_->screen_height / 2);
      break;
    case 3:
      x = (rand() % (screen_->screen_width / 2)) + 1;
      y = (rand() % (screen_->screen_height / 2)) + (screen_->screen_height / 2);
      break;
    default:
      x = (rand() % screen_->screen_width) + 1;
      y = (rand() % screen_->screen_height) + 1;
      break;
    }

    swarm_host_->generate_swarm(
      x, 
      y,
      screen_->screen_width,
      screen_->screen_height
    );
    audio_->play_sound(
      x, 
      y, 
      screen_->screen_width, 
      screen_->screen_height
    );
  }
}

void particle::Game::load_all()
{
  for (const auto& entry : std::filesystem::directory_iterator(save_dir_))
  {
    files.push_back(entry.path().string());
  }
}
