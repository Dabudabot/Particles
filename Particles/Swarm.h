/**
 * \author Dabudabot
 * \brief contain particles controls
 */

#pragma once

#include <SDL.h>
#include <memory>
#include <vector>
#include "Wall.h"

/**
* \brief main namespace of the project
*/
namespace particle
{
  /**
   * \brief basic element, simple particle
   */
  class Particle
  {
    /**
     * \brief color of the particle
     */
    Uint32 color_;
    /**
     * \brief relative coordinates of the particle
     */
    D_Point point_{};

    /**
     * \brief original absolute coordinates of the particle, birth place
     */
    I_Point o_point_{};

    /**
     * \brief speed increase ratio
     */
    double acceleration_;
    /**
     * \brief speed of the particle
     */
    double speed_;
    /**
     * \brief direction of the particle
     */
    double dir_;

  public:

    // delete unused ctors
    Particle(const Particle&) = delete;
    Particle(Particle&&) = delete;
    Particle& operator= (const Particle&) = delete;
    Particle& operator= (Particle&&) = delete;

    /**
     * \brief ctor which init particle by absolute coordinates
     * \param x absolute width coordinate
     * \param y absolute height coordinate
     */
    Particle(int x, int y);
    ~Particle() = default;

    /**
     * \brief getter of the absolute width coordinate of the particle 
     * \return absolute width coordinate of the particle 
     */
    int get_x() const;
    /**
     * \brief getter of the absolute height coordinate of the particle 
     * \return absolute height coordinate of the particle 
     */
    int get_y() const;
    /**
     * \brief getter of the color of the particle
     * \return color of the particle
     */
    Uint32 get_color() const { return color_; }

    /**
     * \brief update particle position
     * \param interval time between previous update and current
     * \param color color of the particle
     * \param wall_host keeper of the walls to calculate collision
     */
    void update(int interval, Uint32& color, const std::shared_ptr<WallHost>& wall_host);
  };

  /**
   * \brief typedef of the vector of the pointers to the particle
   */
  using Particles = std::vector<std::shared_ptr<Particle>>;

  /**
   * \brief class to control group of the particles
   */
  class Swarm
  {
    /**
     * \brief last update time
     */
    int last_time_;
    /**
     * \brief size of the group of particles
     */
    int swarm_size_;
    /**
     * \brief container of the particles
     */
    Particles particles_;

  public:

    // delete unused ctors
    Swarm(const Swarm&) = delete;
    Swarm(Swarm&&) = delete;
    Swarm& operator= (const Swarm&) = delete;
    Swarm& operator= (Swarm&&) = delete;

    /**
     * \brief ctor which init particle by absolute coordinates
     * \param origin_x absolute width coordinate of starting point of explosion
     * \param origin_y absolute height coordinate of starting point of explosion
     */
    Swarm(int origin_x, int origin_y);
    ~Swarm() = default;

    /**
     * \brief delete percent of particles in current swarm
     * \param percent percent of particles to delete
     */
    void delete_particles(unsigned int percent);
    /**
     * \brief getter of container of particles
     * \return container of particles
     */
    Particles get_particles() const { return particles_; };
    /**
     * \brief update all particles in current swarm
     * \param elapsed time from start of the game
     * \param wall_host control of the walls
     */
    void update(int elapsed, const std::shared_ptr<WallHost>& wall_host);
  };

  /**
   * \brief typedef of container of container of particles
   */
  using Swarms = std::vector<std::shared_ptr<Swarm>>;

  /**
   * \brief class that controls containers of the particles
   */
  class SwarmHost
  {
    /**
     * \brief container of container of particles
     */
    Swarms swarms_;

  public:

    // delete unused ctors
    SwarmHost(const SwarmHost&) = delete;
    SwarmHost(SwarmHost&&) = delete;
    SwarmHost& operator= (const SwarmHost&) = delete;
    SwarmHost& operator= (SwarmHost&&) = delete;

    SwarmHost() = default;
    ~SwarmHost() = default;

    /**
     * \brief create new swarm
     * \param x original absolute width coordinate
     * \param y original absolute height coordinate
     */
    void generate_swarm(int x, int y);
    /**
     * \brief update all swarms
     * \param elapsed time from start of the game
     * \param wall_host control of the walls
     */
    void update(int elapsed, const std::shared_ptr<WallHost>& wall_host);
    /**
     * \brief sum all particles of all swarms
     * \return container of all particles in world
     */
    Particles get_particles();
  };
}

