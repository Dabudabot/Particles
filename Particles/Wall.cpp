/**
 * \author Dabudabot
 * \brief
 */

#include "Wall.h"
#include "Utils.h"
#include <algorithm>

particle::Wall::Wall(const int x, const int y, const int max_x, const int max_y)
  : a_(0), b_(0), c_(0), fade_counter_(0)
{
  coordinates_.start.x = Screen::to_relative(x, max_x);
  coordinates_.start.y = Screen::to_relative(y, max_y);
  coordinates_.end.x = coordinates_.start.x;
  coordinates_.end.y = coordinates_.start.y;
}

particle::Wall::Wall(const Coordinates<double> coordinates)
{
  coordinates_.start.x = coordinates.start.x;
  coordinates_.start.y = coordinates.start.y;
  coordinates_.end.x = coordinates.end.x;
  coordinates_.end.y = coordinates.end.y;
  fade_counter_ = 0;
  a_ = 0;
  b_ = 0;
  c_ = 0;
  calc_abc();
}

void particle::Wall::calc_abc()
{
  a_ = coordinates_.start.y - coordinates_.end.y;
  b_ = coordinates_.end.x - coordinates_.start.x;
  c_ = coordinates_.start.x * coordinates_.end.y - coordinates_.end.x * coordinates_.start.y;
}

void particle::Wall::move_end(
  const int start_x, const int start_y,
  int x, int y, 
  const int max_x, const int max_y)
{
  

  coordinates_.end.x = Screen::to_relative(x, max_x);
  coordinates_.end.y = Screen::to_relative(y, max_y);

  calc_abc();
}

void particle::Wall::draw_wall(
  const std::shared_ptr<Screen>& screen, 
  const bool fade, 
  const int max_x, 
  const int max_y
)
{
  if (fade_counter_ == 0 && fade)
  {
    return;
  }

  if (fade)
  {
    fade_counter_ -= 0x5;
  }
  else
  {
    fade_counter_ = 0xff;
  }

  const auto color = generate_color<Uint8, Uint32>(fade_counter_);
  auto x1 = Screen::to_abs(coordinates_.start.x, max_x);
  auto y1 = Screen::to_abs(coordinates_.start.y, max_y);
  auto x2 = Screen::to_abs(coordinates_.end.x, max_x);
  auto y2 = Screen::to_abs(coordinates_.end.y, max_y);

  // Bresenham's line algorithm
  const auto steep = (std::abs(y2 - y1) > std::abs(x2 - x1));
  if (steep)
  {
    std::swap(x1, y1);
    std::swap(x2, y2);
  }

  if (x1 > x2)
  {
    std::swap(x1, x2);
    std::swap(y1, y2);
  }

  const auto dx = x2 - x1;
  const auto dy = std::abs(y2 - y1);

  auto error = dx / 2.0f;
  const auto y_step = (y1 < y2) ? 1 : -1;
  auto y = y1;

  const auto max_x1 = x2;

  for (auto x = x1; x < max_x1; x++)
  {
    if (steep)
    {
      screen->set_pixel(y, x, color);
    }
    else
    {
      screen->set_pixel(x, y, color);
    }

    error -= dy;
    if (error < 0)
    {
      y += y_step;
      error += dx;
    }
  }
}

bool particle::Wall::on_segment(const D_Point p, const D_Point q, const D_Point r) const
{
  return q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
    q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y);
}

int particle::Wall::orientation(const D_Point p, const D_Point q, const D_Point r)
{
  // See https://www.geeksforgeeks.org/orientation-3-ordered-points/ 
  // for details of below formula. 
  const auto val = (q.y - p.y) * (r.x - q.x) -
    (q.x - p.x) * (r.y - q.y);

  if (val == 0) return 0;  // colinear 

  return (val > 0) ? 1 : 2; // clock or counter clock wise 
}

// The main function that returns true if line segment 'p1q1' 
// and 'p2q2' intersect. 
bool particle::Wall::is_collide(const D_Point p1, const D_Point q1, const D_Point p2, const D_Point q2) const
{
  // Find the four orientations needed for general and 
  // special cases 
  const auto o1 = orientation(p1, q1, p2);
  const auto o2 = orientation(p1, q1, q2);
  const auto o3 = orientation(p2, q2, p1);
  const auto o4 = orientation(p2, q2, q1);

  // General case 
  if (o1 != o2 && o3 != o4)
    return true;

  // Special Cases 
  // p1, q1 and p2 are colinear and p2 lies on segment p1q1 
  if (o1 == 0 && on_segment(p1, p2, q1)) return true;

  // p1, q1 and q2 are colinear and q2 lies on segment p1q1 
  if (o2 == 0 && on_segment(p1, q2, q1)) return true;

  // p2, q2 and p1 are colinear and p1 lies on segment p2q2 
  if (o3 == 0 && on_segment(p2, p1, q2)) return true;

  // p2, q2 and q1 are colinear and q1 lies on segment p2q2 
  if (o4 == 0 && on_segment(p2, q1, q2)) return true;

  return false; // Doesn't fall in any of the above cases 
}

void particle::WallHost::start_wall(const int x, const int y, const int max_x, const int max_y)
{
  active_wall_ = std::make_shared<Wall>(x , y, max_x, max_y);
}

void particle::WallHost::move_wall(
  const std::shared_ptr<Screen>& screen, 
  const int x, const int y, 
  const int max_x, const int max_y) const
{
  active_wall_->move_end(
    active_wall_->get_coordinates()->start.x, 
    active_wall_->get_coordinates()->start.y, 
    x, y, max_x, max_y);
  active_wall_->draw_wall(screen, false, max_x, max_y);
}

void particle::WallHost::end_wall(
  const int x, const int y,
  const int max_x, const int max_y)
{
  active_wall_->move_end(
    active_wall_->get_coordinates()->start.x,
    active_wall_->get_coordinates()->start.y, 
    x, y, max_x, max_y);
  walls_.push_back(active_wall_);
  active_wall_ = nullptr;
}

bool particle::WallHost::is_collide(
  const double old_x,
  const double old_y,
  const double new_x,
  const double new_y) const
{
  for (const auto& wall : walls_)
  {
    if (wall->is_collide(
      D_Point(old_x, old_y), 
      D_Point(new_x, new_y), 
      wall->get_coordinates()->start, 
      wall->get_coordinates()->end
    ))
    {
      return true;
    }
  }

  return false;
}

void particle::WallHost::draw_walls(
  const std::shared_ptr<Screen>& screen, 
  const bool fade,
  const int max_x, 
  const int max_y)
{
  if (nullptr != active_wall_)
  {
    active_wall_->draw_wall(screen, fade, max_x, max_y);
  }
  
  for (const auto& wall : walls_)
  {
    wall->draw_wall(screen, fade, max_x, max_y);
  }
}

bool particle::WallHost::is_overflow() const
{
  return walls_.size() > 70;
}

void particle::WallHost::serialize_walls(std::ostream& file)
{
  for (const auto& wall : walls_)
  {
    file.write(
      reinterpret_cast<char*>(wall->get_coordinates()), 
      sizeof(D_Coordinates)
    );
  }
}

void particle::WallHost::deserialize_walls(std::istream& file)
{
  while (!file.eof())
  {
    D_Coordinates coordinates{};
    file.read(reinterpret_cast<char*>(&coordinates), sizeof(D_Coordinates));
    walls_.push_back(std::make_shared<Wall>(coordinates));
  }
}
