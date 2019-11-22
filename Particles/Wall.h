/**
 * \author Dabudabot
 * \brief container of the wall control
 */

#pragma once

#include "Screen.h"

#include <memory>
#include <vector>
#include <fstream>

/**
* \brief main namespace of the project
*/
namespace particle
{
  /**
   * \brief simple x and y coordinates
   * \tparam T size of coordinates
   */
  template <class T>
  struct Point
  {
    /**
     * \brief width coordinate
     */
    T x;
    /**
     * \brief height coordinate
     */
    T y;

    Point() = default;
    /**
     * \brief ctor to construct by coordinates
     * \param x width coordinate
     * \param y height coordinate
     */
    Point(T x, T y) : x(x), y(y) {}
  };

  /**
   * \brief coordinates of the line
   * \tparam T size of coordinates
   */
  template <class T>
  struct Coordinates
  {
    /**
     * \brief starting point of the line
     */
    Point<T> start;
    /**
     * \brief end point of the line
     */
    Point<T> end;
  };

  /**
   * \brief typedef of point of size double
   */
  using D_Point = Point<double>;
  /**
   * \brief typedef of point of size int
   */
  using I_Point = Point<int>;
  /**
   * \brief typedef of line of size double
   */
  using D_Coordinates = Coordinates<double>;

  /**
   * \brief wall control class
   */
  class Wall
  {
    /**
     * \brief start and end point coordinates of the wall
     */
    struct Coordinates<double> coordinates_{};
    
    /**
     * \brief y1 - y2
     */
    double a_;
    /**
     * \brief x2 - x1
     */
    double b_;
    /**
     * \brief x1*y2 - x2*y1
     */
    double c_;

    /**
     * \brief counter of fade to black of the wall
     */
    int fade_counter_;

  public:

    /**
     * \brief calculate a, b and c by coordinates
     */
    void calc_abc();
    /**
     * \brief Given three colinear points p, q, r,
     *        the function checks if point q lies
     *        on line segment 'pr'
     * \param p point
     * \param q point
     * \param r point
     * \return true if lies
     */
    [[nodiscard]] bool on_segment(D_Point p, D_Point q, D_Point r) const;
    /**
     * \brief To find orientation of ordered triplet (p, q, r)
     * \param p point
     * \param q point
     * \param r point
     * \return  0 --> p, q and r are colinear
     *          1 --> Clockwise
     *          2 --> Counterclockwise
     */
    static int orientation(D_Point p, D_Point q, D_Point r);

    /**
     * \brief ctor to start wall
     * \param x width coordinates of the start of the wall
     * \param y height coordinates of the start of the wall
     * \param max_x max width coordinates of the start of the wall
     * \param max_y max height coordinates of the start of the wall
     */
    Wall(int x, int y, int max_x, int max_y);
    /**
     * \brief ctor to build wall from coordinates
     * \param coordinates coordinates of the wall
     */
    explicit Wall(Coordinates<double> coordinates);

    /**
     * \brief move end point of the wall
     * \param start_x center of the wall x
     * \param start_y center of the wall y
     * \param x width coordinates of the end of the wall
     * \param y height coordinates of the end of the wall
     * \param max_x max width coordinates of the end of the wall
     * \param max_y max height coordinates of the end of the wall
     */
    void move_end(int start_x, int start_y, int x, int y, int max_x, int max_y);
    /**
     * \brief draw wall on the screen with fade
     * \param screen screen to draw
     * \param fade should fade it or not
     * \param max_x max width coordinates of the end of the wall
     * \param max_y max height coordinates of the end of the wall
     */
    void draw_wall(const std::shared_ptr<Screen>& screen, bool fade, int max_x, int max_y);
    /**
     * \brief does p1-q1 line intersect p2-q2
     * \param p1 start coordinates of first line
     * \param q1 end coordinates of first line
     * \param p2 start coordinates of second line
     * \param q2 end coordinates of second line
     * \return true if intersect, false otherwise
     */
    [[nodiscard]] bool is_collide(D_Point p1, D_Point q1, D_Point p2, D_Point q2) const;
    /**
     * \brief getter of the address of the coordinates of the wall
     * \return address of the coordinates of the wall
     */
    Coordinates<double>* get_coordinates() { return &coordinates_; }
  };

  /**
   * \brief control and contain the wall
   */
  class WallHost
  {
    /**
     * \brief container of the wall
     */
    std::vector<std::shared_ptr<Wall>> walls_;
    /**
     * \brief current building wall
     */
    std::shared_ptr<Wall> active_wall_;

  public:
    [[nodiscard]] std::vector<std::shared_ptr<Wall>> get_walls() const { return walls_; }
    [[nodiscard]] std::shared_ptr<Wall> get_active_wall() const { return active_wall_; }

    // delete unused ctors
    WallHost(const WallHost&) = delete;
    WallHost(WallHost&&) = delete;
    WallHost& operator= (const WallHost&) = delete;
    WallHost& operator= (WallHost&&) = delete;

    ~WallHost() = default;

    /**
     * \brief default ctor defining active wall with null
     */
    WallHost() : active_wall_(nullptr) {}

    /**
     * \brief start active wall
     * \param x absolute start width coordinate of the wall
     * \param y absolute start height coordinate of the wall
     * \param max_x absolute start width coordinate of the wall
     * \param max_y absolute start height coordinate of the wall
     */
    void start_wall(int x, int y, int max_x, int max_y);
    /**
     * \brief moving end of active wall by coordinates
     * \param screen drawer of the wall
     * \param x absolute end width coordinate of the wall
     * \param y absolute end height coordinate of the wall
     * \param max_x max absolute end width coordinate of the wall
     * \param max_y max absolute end height coordinate of the wall
     */
    void move_wall(
      const std::shared_ptr<Screen>& screen, 
      int x, int y, 
      int max_x, int max_y) const;
    /**
     * \brief finish wall
     * \param x absolute end width coordinate of the wall
     * \param y absolute end height coordinate of the wall
     * \param max_x max absolute end width coordinate of the wall
     * \param max_y max absolute end height coordinate of the wall
     */
    void end_wall(
      int x, int y, 
      int max_x, int max_y);
    /**
     * \brief is line intersect with any wall in container
     * \param old_x relative width coordinate start of the line
     * \param old_y relative height coordinate start of the line
     * \param new_x relative width coordinate end of the line
     * \param new_y relative height coordinate end of the line
     * \return true if any wall is intersected, false otherwise
     */
    [[nodiscard]] bool is_collide(double old_x, double old_y, double new_x, double new_y) const;
    /**
     * \brief draw all walls
     * \param screen drawer
     * \param fade should fade walls or not
     * \param max_x max absolute end width coordinate of the wall
     * \param max_y max absolute end height coordinate of the wall
     */
    void draw_walls(
      const std::shared_ptr<Screen>& screen, 
      bool fade,
      int max_x,
      int max_y);
    /**
     * \brief too many walls? (max 70)
     * \return true if too many
     */
    [[nodiscard]] bool is_overflow() const;

    /**
     * \brief print all coordinates of walls
     * \param file file to print to
     */
    void serialize_walls(std::ostream& file);
    /**
     * \brief read all coordinates of walls
     * \param file file to read from
     */
    void deserialize_walls(std::istream& file);
  };
}


