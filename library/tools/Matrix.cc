#include <sched/tools/Matrix.h>

#include <cassert>
#include <stdexcept>

#include <sched/tools/KahanSum.h>

namespace sched {
  Matrix::Matrix(std::size_t lines, std::size_t columns, double val)
  : m_lines(lines)
  , m_columns(columns)
  , m_data(lines * columns, val)
  {
  }

  Matrix& Matrix::operator+=(const Matrix& other) {
    assert(line_count() == other.line_count());
    assert(column_count() == other.column_count());

    for (std::size_t li = 0; li < line_count(); ++li) {
      for (std::size_t co = 0; co < column_count(); ++co) {
        value_at(li, co) += other(li, co);
      }
    }

    return *this;
  }

  Matrix& Matrix::operator-=(const Matrix& other) {
    assert(line_count() == other.line_count());
    assert(column_count() == other.column_count());

    for (std::size_t li = 0; li < line_count(); ++li) {
      for (std::size_t co = 0; co < column_count(); ++co) {
        value_at(li, co) -= other(li, co);
      }
    }

    return *this;
  }

  Matrix& Matrix::operator*=(double val) {
    for (auto & x : m_data) {
      x *= val;
    }

    return *this;
  }

  Matrix& Matrix::operator/=(double val) {
    for (auto & x : m_data) {
      x /= val;
    }

    return *this;
  }

  Matrix Matrix::transpose() const {
    Matrix transposed(m_columns, m_lines);

    for (std::size_t li = 0; li < m_lines; ++li) {
      for (std::size_t co = 0; co < m_columns; ++co) {
        transposed(co, li) = value_at(li, co);
      }
    }

    return transposed;
  }

  double Matrix::value_at(std::size_t li, std::size_t co) const {
    assert(li < m_lines);
    assert(co < m_columns);
    assert(linearize(li, co) < m_data.size());
    return m_data[linearize(li, co)];
  }

  double& Matrix::value_at(std::size_t li, std::size_t co) {
    assert(li < m_lines);
    assert(co < m_columns);
    assert(linearize(li, co) < m_data.size());
    return m_data[linearize(li, co)];
  }

  std::size_t Matrix::linearize(std::size_t li, std::size_t co) const {
    return li * m_columns + co;
  }

  Matrix operator+(const Matrix& lhs, const Matrix& rhs) {
    Matrix res(lhs);
    res += rhs;
    return res;
  }

  Matrix operator-(const Matrix& lhs, const Matrix& rhs) {
    Matrix res(lhs);
    res -= rhs;
    return res;
  }

  Matrix operator*(double lhs, const Matrix& rhs) {
    Matrix res(rhs);

    for (std::size_t li = 0; li < res.line_count(); ++li) {
      for (std::size_t co = 0; co < res.column_count(); ++co) {
        res(li, co) *= lhs;
      }
    }

    return res;
  }

  Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
    Matrix res(lhs.line_count(), rhs.column_count());

    if (lhs.column_count() != rhs.line_count()) {
      throw std::runtime_error("matrix size mismatch.");
    }

    std::size_t n = lhs.column_count();

    for (std::size_t li = 0; li < res.line_count(); ++li) {
      for (std::size_t co = 0; co < res.column_count(); ++co) {
        KahanSum sum;

        for (std::size_t k = 0; k < n; ++k) {
          sum.add(lhs(li, k) * rhs(k, co));
        }

        res(li, co) = sum.get_result();
      }
    }

    return res;
  }

}
