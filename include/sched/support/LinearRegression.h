#ifndef SCHED_LINEAR_REGRESSION_H
#define SCHED_LINEAR_REGRESSION_H

#include <vector>

#include <sched/Api.h>

namespace sched {

  class SCHED_API LinearRegression2D {
  public:
    struct Result {
      double a0 = 0.0;
      double a1 = 0.0;
      double a2 = 0.0;
      double r_squared = 0.0;
      double adjusted_r_squared = 0.0;
    };

    void add(double x0, double x1, double y);

    Result compute_regression() const;

  private:
    struct Point {
      double x0, x1, y;
    };

    std::vector<Point> m_data;
  };

}

#endif // SCHED_LINEAR_REGRESSION_H
