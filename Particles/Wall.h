#pragma once

#include "Screen.h"

#include <memory>
#include <vector>

namespace particle
{
  class Wall
  {
    double x1_;
    double y1_;
    double x2_;
    double y2_;

    double a_;
    double b_;
    double c_;

    int fade_counter_;

    void calc_abc();

  public:

    Wall(double x, double y);

    void move_end(double x, double y);
    void draw_wall(const std::shared_ptr<Screen>& screen, bool fade);
    bool is_sign(double x, double y) const;
    bool is_collide(double old_x, double old_y, double new_x, double new_y) const;
  };

  class WallHost
  {
    std::vector<std::shared_ptr<Wall>> walls_;
    std::shared_ptr<Wall> active_wall_;

  public:

    WallHost() = default;

    void start_wall(double x, double y);
    void move_wall(const std::shared_ptr<Screen>& screen, double x, double y) const;
    void end_wall(double x, double y);
    bool is_collide(double old_x, double old_y, double new_x, double new_y);
    void draw_walls(const std::shared_ptr<Screen>& screen, bool fade);
  };
}


