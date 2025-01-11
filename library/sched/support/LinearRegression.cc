// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/support/LinearRegression.h>

#include <cassert>

#include <sched/support/KahanSum.h>
#include <sched/support/Matrix.h>
#include <sched/support/Statistics.h>

namespace sched {

  namespace {

    Matrix inverse(const Matrix& mat)
    {
      assert(mat.line_count() == 3);
      assert(mat.column_count() == 3);

      Matrix res(3, 3);

      res(0, 0) = (mat(1, 1) * mat(2, 2)) - (mat(1, 2) * mat(2, 1));
      res(0, 1) = -((mat(0, 1) * mat(2, 2)) - (mat(0, 2) * mat(2, 1)));
      res(0, 2) = (mat(0, 1) * mat(1, 2)) - (mat(0, 2) * mat(1, 1));
      res(1, 0) = -((mat(1, 0) * mat(2, 2)) - (mat(1, 2) * mat(2, 0)));
      res(1, 1) = (mat(0, 0) * mat(2, 2)) - (mat(0, 2) * mat(2, 0));
      res(1, 2) = -((mat(0, 0) * mat(1, 2)) - (mat(0, 2) * mat(1, 0)));
      res(2, 0) = (mat(1, 0) * mat(2, 1)) - (mat(1, 1) * mat(2, 0));
      res(2, 1) = -((mat(0, 0) * mat(2, 1)) - (mat(0, 1) * mat(2, 0)));
      res(2, 2) = mat(0, 0) * mat(1, 1) - mat(0, 1) * mat(1, 0);

      const double det = (mat(0, 0) * res(0, 0)) + (mat(1, 0) * res(0, 1)) + (mat(2, 0) * res(0, 2));
      res /= det;

      return res;
    }

    double norm2(const Matrix& mat)
    {
      assert(mat.column_count() == 1);

      KahanSum sum;

      for (std::size_t i = 0; i < mat.line_count(); ++i) {
        sum.add(mat(i, 0) * mat(i, 0));
      }

      return sum.get_result();
    }

  }

  void LinearRegression2D::add(double x0, double x1, double y)
  {
    m_data.push_back({ x0, x1, y });
  }

  auto LinearRegression2D::compute_regression() const -> Result
  {
    constexpr std::size_t P = 2;
    Matrix x(m_data.size(), P + 1);
    Matrix y(m_data.size(), 1);

    for (std::size_t i = 0; i < m_data.size(); ++i) {
      auto point = m_data[i];
      x(i, 0) = 1.0;
      x(i, 1) = point.x0;
      x(i, 2) = point.x1;
      y(i, 0) = point.y;
    }

    const Matrix x_trans = x.transpose();
    const Matrix mat = (inverse(x_trans * x) * x_trans) * y;

    assert(mat.line_count() == 3);
    assert(mat.column_count() == 1);

    // determination of R^2

    Statistics stats;

    for (auto point : m_data) {
      stats.add(point.y);
    }

    const double y_mean = stats.get_mean();
    const Matrix y_hat = x * mat;

    const double ssr = norm2(y_hat) - (static_cast<double>(m_data.size()) * y_mean * y_mean);
    const double sst = norm2(y) - (static_cast<double>(m_data.size()) * y_mean * y_mean);

    Result res;
    res.a0 = mat(0, 0);
    res.a1 = mat(1, 0);
    res.a2 = mat(2, 0);
    res.r_squared = ssr / sst;
    res.adjusted_r_squared = 1.0 - (1.0 - res.r_squared) * (static_cast<double>(m_data.size()) - 1.0) / (static_cast<double>(m_data.size()) - P - 1.0);
    return res;
  }

}
