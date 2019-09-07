#pragma once

#include <SDL.h>
#include <memory>
#include <vector>

namespace particle
{
  class Particle
  {
    double x_;
    double y_;
    Uint32 color_;

    double acceleration_;
    double speed_;
    double dir_;

  public:

    // delete unused ctors
    Particle(const Particle&) = delete;
    Particle(Particle&&) = delete;
    Particle& operator= (const Particle&) = delete;
    Particle& operator= (Particle&&) = delete;

    Particle(double x, double y);
    ~Particle() = default;

    double get_x() const { return x_; }
    double get_y() const { return y_; }
    Uint32 get_color() const { return color_; }

    void update(int interval, Uint32& color);
  };

  using Particles = std::vector<std::shared_ptr<Particle>>;

  class Swarm
  {
    int last_time_;
    int swarm_size_;
    Particles particles_;

  public:

    // delete unused ctors
    Swarm(const Swarm&) = delete;
    Swarm(Swarm&&) = delete;
    Swarm& operator= (const Swarm&) = delete;
    Swarm& operator= (Swarm&&) = delete;

    Swarm(double origin_x, double origin_y);
    ~Swarm() = default;

    Particles get_particles() const { return particles_; };
    void update(int elapsed);
  };

  using Swarms = std::vector<std::shared_ptr<Swarm>>;

  class SwarmHost
  {
    Swarms swarms_;

  public:

    // delete unused ctors
    SwarmHost(const SwarmHost&) = delete;
    SwarmHost(SwarmHost&&) = delete;
    SwarmHost& operator= (const SwarmHost&) = delete;
    SwarmHost& operator= (SwarmHost&&) = delete;

    SwarmHost() = default;
    ~SwarmHost() = default;

    void generate_swarm(double x, double y);
    void update(int elapsed);
    Particles get_particles();
  };
}

