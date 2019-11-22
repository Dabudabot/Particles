#include <iostream>

using namespace std;

struct Point
{
  int x, y;

  void set(const int x_, const int y_)
  {
    x = x_;
    y = y_;
  }
};

int main()
{
  int n;
  cin >> n;

  const auto points = new Point[n];

  for (auto i = 0; i < n; i++)
  {
    int x, y;
    cin >> x >> y;
    points[i].set(x, y);
  }

  cout << "wall_host_->start_wall(" << points[0].x << ", " << points[0].y << ");\n";

  for (auto i = 1; i < n-1; i++)
  {
    cout << "wall_host_->end_wall(" << points[i].x << ", " << points[i].y << ");\n";
    cout << "wall_host_->start_wall(" << points[i].x << ", " << points[i].y << ");\n";
  }

  cout << "wall_host_->end_wall(" << points[n-1].x << ", " << points[n - 1].y << ");\n";

  delete[] points;

  return 0;
}