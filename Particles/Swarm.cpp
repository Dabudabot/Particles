/**
 * \author Dabudabot
 * \brief
 */

#include "Swarm.h"
#include "Utils.h"
#include "Screen.h"

particle::Particle::Particle(const int x, const int y, const int max_x, const int max_y)
: color_(0xffffffff)
{
  point_.x = Screen::to_relative(x, max_x);
  point_.y = Screen::to_relative(y, max_y);
  o_point_.x = x;
  o_point_.y = y;

  acceleration_ = 0.0001;
  speed_ = acceleration_ * (((1.0 * rand()) / RAND_MAX) + 0.1);
  dir_ = (2 * M_PI * rand()) / RAND_MAX;
  is_dead_ = false;
}

int particle::Particle::get_x(const int max_x) const
{
  return Screen::to_abs(point_.x, max_x);
}

int particle::Particle::get_y(const int max_y) const
{
  return Screen::to_abs(point_.y, max_y);
}

void particle::Particle::update(
  const int interval, 
  Uint32& color,
  const std::shared_ptr<WallHost>& wall_host
)
{
  if (is_dead_) return;

  const auto rel_x = point_.x;
  const auto rel_y = point_.y;

  // calc speed according to system performance
  const auto x_fix = 0.6;
  auto fixed_speed = speed_ * static_cast<double>(interval);

  if (fixed_speed > 0.1) {
    fixed_speed = 0.1;
  }
  auto x_result = rel_x + fixed_speed * x_fix * cos(dir_);
  auto y_result = rel_y + fixed_speed * sin(dir_);

  auto ttl = 100;

  // if collision, recalculate direction randomly
  while (x_result < 0 ||
    x_result > 2 ||
    y_result < 0 ||
    y_result > 2 ||
    wall_host->is_collide(rel_x, rel_y, x_result, y_result))
  {
    ttl--;

    if (ttl == 0)
    {
      is_dead_ = true;
      break;
    }

    speed_ = acceleration_ * (((1.0 * rand()) / RAND_MAX) + 0.2);
    fixed_speed = speed_ * interval;
    dir_ = (2 * M_PI * rand()) / RAND_MAX;
    x_result = rel_x + fixed_speed * x_fix * cos(dir_);
    y_result = rel_y + fixed_speed * sin(dir_);
  }

  point_.x = x_result;
  point_.y = y_result;
  color_ = color;
}

particle::Swarm::Swarm(const int origin_x, const int origin_y, const int max_x, const int max_y)
: last_time_(0)
{
  swarm_size_ = rand() % 300 + 300;
  particles_ = Particles(swarm_size_);

  for (auto& particle : particles_)
  {
    particle = std::make_shared<Particle>(origin_x, origin_y, max_x, max_y);
  }
}

void particle::Swarm::delete_particles(const unsigned int percent)
{
  const auto amount = static_cast<unsigned int>(particles_.size()) * percent / 100;
  if (amount == 0) return;
  particles_.resize(particles_.size() - amount);
}

void particle::Swarm::update(const int elapsed, const std::shared_ptr<WallHost>& wall_host)
{
  auto interval = elapsed - last_time_;

  if (last_time_ == 0) interval = 1;

  const auto red = static_cast<unsigned char>(
    (1 + sin((double(elapsed) - 0x4000) * 0.0001)) * 128);
  const auto green = static_cast<unsigned char>(
    (1 + sin(elapsed * 0.0001)) * 128);
  const auto blue = static_cast<unsigned char>(
    (1 + sin((double(elapsed) + 0x4000) * 0.0001)) * 128);
  auto color = generate_color<Uint8, Uint32>(
    red, green, blue, 0xFF);

  for (const auto& particle : particles_)
  {
    particle->update(interval, color, wall_host);
  }

  last_time_ = elapsed;
}

void particle::SwarmHost::generate_swarm(int x, int y, int max_x, int max_y)
{
  swarms_.push_back(std::make_shared<Swarm>(x, y, max_x, max_y));

  for (unsigned int i = static_cast<int>(swarms_.size()) - 1; i > 0; i--)
  {
    auto percent = (static_cast<int>(swarms_.size()) - i) * 2;

    if (percent >= 100) percent = 100;

    swarms_[i]->delete_particles(percent);
  }

  if (swarms_.size() >= 50 )
  {
    swarms_.erase(swarms_.begin(), swarms_.begin() + 1);
  }
}

void particle::SwarmHost::update(const int elapsed, const std::shared_ptr<WallHost>& wall_host)
{
  for (const auto& swarm : swarms_)
  {
    swarm->update(elapsed, wall_host);
  }
}

particle::Particles particle::SwarmHost::get_particles()
{
  Particles result;

  for (const auto& swarm : swarms_)
  {
    auto particles = swarm->get_particles();

    result.insert(
      result.end(), 
      particles.begin(), 
      particles.end()
    );
  }

  return result;
}
