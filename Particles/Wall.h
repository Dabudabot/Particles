#pragma once

#include "Screen.h"

#include <memory>
#include <vector>
#include <fstream>

namespace particle
{
  template <class T>
  struct Point
  {
    T x;
    T y;

    Point() = default;
    Point(T x, T y) : x(x), y(y) {}
  };

  template <class T>
  struct Coordinates
  {
    Point<T> start;
    Point<T> end;
  };

  using D_Point = Point<double>;
  using I_Point = Point<int>;
  using D_Coordinates = Coordinates<double>;

  class Wall
  {
    struct Coordinates<double> coordinates_{};
    
    double a_;
    double b_;
    double c_;

    int fade_counter_;

    void calc_abc();
    bool on_segment(D_Point p, D_Point q, D_Point r) const;
    static int orientation(D_Point p, D_Point q, D_Point r);

  public:

    Wall(int x, int y);
    explicit Wall(Coordinates<double> coordinates);

    void move_end(int x, int y);
    void draw_wall(const std::shared_ptr<Screen>& screen, bool fade);
    bool is_collide(D_Point p1, D_Point q1, D_Point p2, D_Point q2) const;
    Coordinates<double>* get_coordinates() { return &coordinates_; }
  };

  class WallHost
  {
    std::vector<std::shared_ptr<Wall>> walls_;
    std::shared_ptr<Wall> active_wall_;

  public:

    WallHost() : active_wall_(nullptr) {}

    void start_wall(int x, int y);
    void move_wall(const std::shared_ptr<Screen>& screen, int x, int y) const;
    void end_wall(int x, int y);
    bool is_collide(double old_x, double old_y, double new_x, double new_y);
    void draw_walls(const std::shared_ptr<Screen>& screen, bool fade);
    bool is_overflow() const;

    void serialize_walls(std::ostream& file);
    void deserialize_walls(std::istream& file);
  };
}


