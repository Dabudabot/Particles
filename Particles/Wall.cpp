#include "Wall.h"
#include "Utils.h"

particle::Wall::Wall(const double x, const double y)
  : x1_(x), y1_(y), x2_(x), y2_(y), a_(0), b_(0), c_(0), fade_counter_(0)
{}

void particle::Wall::calc_abc()
{
  a_ = y1_ - y2_;
  b_ = x2_ - x1_;
  c_ = x1_ * y2_ - x2_ * y1_;
}

void particle::Wall::move_end(const double x, const double y)
{
  x2_ = x;
  y2_ = y;

  calc_abc();
}

void particle::Wall::draw_wall(const std::shared_ptr<Screen>& screen, const bool fade)
{
  if (fade)
  {
    fade_counter_ -= 0x11;
  }
  else
  {
    fade_counter_ = 0xff;
  }

  if (fade_counter_ == 0)
  {
    return;
  }

  const auto color = generate_color<Uint8, Uint32>(
    0xff, 0xff, 0xff, fade_counter_);
  auto x1 = static_cast<int>(x1_) * Screen::screen_width / 2;
  auto y1 = static_cast<int>(y1_) * Screen::screen_height / 2;
  auto x2 = static_cast<int>(x2_) * Screen::screen_width / 2;
  auto y2 = static_cast<int>(y2_) * Screen::screen_height / 2;

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
  return new_sign != old_sign;
}

void particle::WallHost::start_wall(double x, double y)
{
  active_wall_ = std::make_shared<Wall>(x, y);
}

void particle::WallHost::move_wall(const std::shared_ptr<Screen>& screen, const double x, const double y) const
{
  active_wall_->move_end(x, y);
  active_wall_->draw_wall(screen, false);
}

void particle::WallHost::end_wall(const double x, const double y)
{
  active_wall_->move_end(x, y);
  walls_.push_back(active_wall_);
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
  for (const auto& wall : walls_)
  {
    wall->draw_wall(screen, fade);
  }
}
