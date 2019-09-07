#include "SwarmHost.h"
#include "Utils.h"

particle::Particle::Particle(const double x, const double y)
: x_(x), y_(y), color_(0xffffffff)
{
  acceleration_ = 0.001;
  speed_ = acceleration_ * (((1.0 * rand()) / RAND_MAX) + 0.1);
  dir_ = (2 * M_PI * rand()) / RAND_MAX;
}

void particle::Particle::update(
  const int interval, 
  Uint32& color
)
{
  auto fixed_speed = speed_ * interval;

  auto x_result = x_ + fixed_speed * cos(dir_);
  auto y_result = y_ + fixed_speed * sin(dir_);

  while (x_result < 0 || x_result > 2 || y_result < 0 || y_result > 2)
  {
    speed_ = acceleration_ * (((1.0 * rand()) / RAND_MAX) + 0.2);
    fixed_speed = speed_ * interval;
    dir_ = (2 * M_PI * rand()) / RAND_MAX;
    x_result = x_ + fixed_speed * cos(dir_);
    y_result = y_ + fixed_speed * sin(dir_);
  }

  x_ = x_result;
  y_ = y_result;
  color_ = color;
}

particle::Swarm::Swarm(const double origin_x, const double origin_y)
: last_time_(0)
{
  swarm_size_ = rand() % 100 + 500;
  particles_ = Particles(swarm_size_, 
    std::make_shared<Particle>(origin_x, origin_y));
}

void particle::Swarm::update(const int elapsed)
{
  const auto interval = elapsed - last_time_;

  const auto red = static_cast<unsigned char>(
    (1 + sin(elapsed * 0.0001)) * 128);
  const auto green = static_cast<unsigned char>(
    (1 + sin(elapsed * 0.0003)) * 128);
  const auto blue = static_cast<unsigned char>(
    (1 + sin(elapsed * 0.0005)) * 128);
  auto color = generate_color<Uint8, Uint32>(
    red, green, blue, 0xFF);

  for (const auto& particle : particles_)
  {
    particle->update(interval, color);
  }

  last_time_ = elapsed;
}

void particle::SwarmHost::generate_swarm(double x, double y)
{
  swarms_.push_back(std::make_shared<Swarm>(x, y));
}

void particle::SwarmHost::update(const int elapsed)
{
  for (const auto& swarm : swarms_)
  {
    swarm->update(elapsed);
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
