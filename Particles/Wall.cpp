#include "Wall.h"
#include "Utils.h"

particle::Wall::Wall(const int x, const int y)
  : a_(0), b_(0), c_(0), fade_counter_(0)
{
  x1_ = Screen::to_relative(x, Screen::screen_width);
  y1_ = Screen::to_relative(y, Screen::screen_height);
  x2_ = x1_;
  y2_ = y1_;
}

void particle::Wall::calc_abc()
{
  a_ = y1_ - y2_;
  b_ = x2_ - x1_;
  c_ = x1_ * y2_ - x2_ * y1_;
}

void particle::Wall::move_end(const int x, const int y)
{
  x2_ = Screen::to_relative(x, Screen::screen_width);
  y2_ = Screen::to_relative(y, Screen::screen_height);

  calc_abc();
}

void particle::Wall::draw_wall(const std::shared_ptr<Screen>& screen, const bool fade)
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
  auto x1 = Screen::to_abs(x1_, Screen::screen_width);
  auto y1 = Screen::to_abs(y1_, Screen::screen_height);
  auto x2 = Screen::to_abs(x2_, Screen::screen_width);
  auto y2 = Screen::to_abs(y2_, Screen::screen_height);

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

  const auto max_x = x2;

  for (auto x = x1; x < max_x; x++)
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

bool particle::Wall::is_sign(const double x, const double y) const
{
  const auto z = a_ * x + b_ * y + c_;
  return z > 0;
}

bool particle::Wall::is_collide(
  const double old_x,
  const double old_y,
  const double new_x,
  const double new_y) const
{
  const auto old_sign = is_sign(old_x, old_y);
  const auto new_sign = is_sign(new_x, new_y);

  if (new_sign == old_sign) return false;

  auto min_x = x1_;
  auto max_x = x2_;
  auto min_y = y1_;
  auto max_y = y2_;

  if (max_x < min_x) std::swap(min_x, max_x);
  if (max_y < min_y) std::swap(min_y, max_y);

  return (new_x >= min_x && new_x <= max_x &&
    new_y >= min_y && new_y <= max_y);
}

void particle::WallHost::start_wall(const int x, const int y)
{
  active_wall_ = std::make_shared<Wall>(x , y);
}

void particle::WallHost::move_wall(const std::shared_ptr<Screen>& screen, const int x, const int y) const
{
  active_wall_->move_end(x, y);
  active_wall_->draw_wall(screen, false);
}

void particle::WallHost::end_wall(const int x, const int y)
{
  active_wall_->move_end(x, y);
  walls_.push_back(active_wall_);
  active_wall_ = nullptr;
}

bool particle::WallHost::is_collide(
  const double old_x,
  const double old_y,
  const double new_x,
  const double new_y)
{
  for (auto& wall : walls_)
  {
    if (wall->is_collide(old_x, old_y, new_x, new_y))
    {
      return true;
    }
  }

  return false;
}

void particle::WallHost::draw_walls(const std::shared_ptr<Screen>& screen, const bool fade)
{
  if (nullptr != active_wall_)
  {
    active_wall_->draw_wall(screen, fade);
  }
  
  for (const auto& wall : walls_)
  {
    wall->draw_wall(screen, fade);
  }
}

bool particle::WallHost::is_overflow() const
{
  return walls_.size() > 50;
}
