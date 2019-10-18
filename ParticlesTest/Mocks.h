#pragma once

#include "Common.h"
#include "../Particles/Game.h"
#include "../Particles/Game.cpp"
#include "../Particles/Screen.cpp"
#include "../Particles/Wall.cpp"
#include "../Particles/Swarm.cpp"
#include "../Particles/Audio.cpp"

class AudioMock : public particle::Audio
{
public:
  MOCK_METHOD(bool, init, (), (override));
  MOCK_CONST_METHOD0(play_music, bool());
  MOCK_CONST_METHOD2(play_sound, bool(const int x, const int y));
};

class ScreenMock : public particle::Screen
{
public:
  MOCK_METHOD(bool, init, (), (override));
  MOCK_CONST_METHOD5(set_text, void(const char* text, SDL_Color color, TTF_Font* font, int x, int y));
  MOCK_CONST_METHOD1(print_help, void(Uint8 fade));
  MOCK_CONST_METHOD0(present, void());
  MOCK_CONST_METHOD0(update, void());
  MOCK_CONST_METHOD0(clear, void());
  MOCK_CONST_METHOD0(motion_blur, void());
};

class WallHostMock : public particle::WallHost
{
public:
  MOCK_METHOD(void, start_wall, (int x, int y), (override));
  MOCK_CONST_METHOD3(move_wall, void(const std::shared_ptr<particle::Screen>& screen, int x, int y));
  MOCK_METHOD(void, end_wall, (int x, int y), (override));
  MOCK_CONST_METHOD4(is_collide, bool(double old_x, double old_y, double new_x, double new_y));
  MOCK_METHOD(void, draw_walls, (const std::shared_ptr<particle::Screen>& screen, bool fade), (override));
  MOCK_CONST_METHOD0(is_overflow, bool());
  MOCK_METHOD(void, serialize_walls, (std::ostream& file), (override));
  MOCK_METHOD(void, deserialize_walls, (std::istream& file), (override));
};

class SwarmMock : public particle::Swarm
{
public:
  MOCK_METHOD(void, delete_particles, (unsigned int percent), (override));
  MOCK_CONST_METHOD0(get_particles, particle::Particles());
  MOCK_METHOD(void, update, (int elapsed, const std::shared_ptr<particle::WallHost>& wall_host), (override));

  void swarm_delete_particles(const unsigned int percent) { Swarm::delete_particles(percent); }
  particle::Particles swarm_get_particles() const { return Swarm::get_particles(); }
  void swarm_update(const int elapsed, const std::shared_ptr<particle::WallHost>& wall_host) { Swarm::update(elapsed, wall_host); }

};

class SwarmHostMock : public particle::SwarmHost
{
public:
  MOCK_METHOD(void, generate_swarm, (int x, int y), (override));
  MOCK_METHOD(void, update, (int elapsed, const std::shared_ptr<particle::WallHost>& wall_host), (override));
  MOCK_METHOD(particle::Particles, get_particles, (), (override));

  void swarm_host_generate_swarm(const int x, const int y) { SwarmHost::generate_swarm(x, y); }
  void swarm_host_update(const int elapsed, const std::shared_ptr<particle::WallHost>& wall_host) { SwarmHost::update(elapsed, wall_host); }
  particle::Particles swarm_host_get_particles() { return SwarmHost::get_particles(); }
};

class GameMock : public particle::Game
{
public:
  GameMock() : Game() {};

  GameMock(const std::shared_ptr<ScreenMock>& screen,
    const std::shared_ptr<AudioMock>& audio,
    const std::shared_ptr<SwarmHostMock>& swarm_host,
    const std::shared_ptr<WallHostMock>& wall_host) :
    Game(screen, audio, swarm_host, wall_host)
  {};

  MOCK_METHOD(int, run, (), (override));
  MOCK_METHOD(void, draw_help, (), (override));
  MOCK_METHOD(bool, process_event, (SDL_Event& event), (override));
  MOCK_METHOD(void, restore_defaults, (), (override));
  MOCK_CONST_METHOD1(save, void(const char* filename));
  MOCK_METHOD(void, load, (const char* filename), (override));

  int game_run() { return Game::run(); }
  void game_draw_help() { Game::draw_help(); }
  bool game_process_event(SDL_Event& event) { return Game::process_event(event); }
  void game_restore_defaults() { Game::restore_defaults(); }
  void game_save(const char* filename) { Game::save(filename); }
  void game_load(const char* filename) { Game::load(filename); }
};