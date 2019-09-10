#pragma once

#include <SDL.h>
#include <memory>
#include <vector>
#include "Wall.h"

namespace particle
{
  class Particle
  {
    Uint32 color_;
    double x_;
    double y_;

    double acceleration_;
    double speed_;
    double dir_;

  public:

    // delete unused ctors
    Particle(const Particle&) = delete;
    Particle(Particle&&) = delete;
    Particle& operator= (const Particle&) = delete;
    Particle& operator= (Particle&&) = delete;

    Particle(int x, int y);
    ~Particle() = default;

    int get_x() const;
    int get_y() const;
    Uint32 get_color() const { return color_; }

    void update(int interval, Uint32& color, const std::shared_ptr<WallHost>& wall_host);
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

    Swarm(int origin_x, int origin_y);
    ~Swarm() = default;

    void delete_particles(unsigned int percent);
    Particles get_particles() const { return particles_; };
    void update(int elapsed, const std::shared_ptr<WallHost>& wall_host);
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

    void generate_swarm(int x, int y);
    void update(int elapsed, const std::shared_ptr<WallHost>& wall_host);
    Particles get_particles();
  };
}

