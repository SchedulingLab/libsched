#ifndef SCHED_MATRIX_H
#define SCHED_MATRIX_H

#include <vector>

#include <sched/common/Api.h>

namespace sched {

  class SCHED_API Matrix {
  public:
    Matrix(std::size_t lines, std::size_t columns, double val = 0.0);

    std::size_t line_count() const
    {
      return m_lines;
    }

    std::size_t column_count() const
    {
      return m_columns;
    }

    double operator()(std::size_t li, std::size_t co) const
    {
      return value_at(li, co);
    }

    double& operator()(std::size_t li, std::size_t co)
    {
      return value_at(li, co);
    }

    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);

    Matrix& operator*=(double val);
    Matrix& operator/=(double val);

    Matrix transpose() const;

  private:
    double value_at(std::size_t li, std::size_t co) const;
    double& value_at(std::size_t li, std::size_t co);
    std::size_t linearize(std::size_t li, std::size_t co) const;

    std::size_t m_lines;
    std::size_t m_columns;
    std::vector<double> m_data;
  };

  SCHED_API Matrix operator+(const Matrix& lhs, const Matrix& rhs);
  SCHED_API Matrix operator-(const Matrix& lhs, const Matrix& rhs);

  SCHED_API Matrix operator*(double lhs, const Matrix& rhs);
  SCHED_API Matrix operator*(const Matrix& lhs, const Matrix& rhs);

}

#endif // SCHED_MATRIX_H
