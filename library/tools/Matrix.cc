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
    assert(get_lines() == other.get_lines());
    assert(get_columns() == other.get_columns());

    for (std::size_t li = 0; li < get_lines(); ++li) {
      for (std::size_t co = 0; co < get_columns(); ++co) {
        value_at(li, co) += other(li, co);
      }
    }

    return *this;
  }

  Matrix& Matrix::operator-=(const Matrix& other) {
    assert(get_lines() == other.get_lines());
    assert(get_columns() == other.get_columns());

    for (std::size_t li = 0; li < get_lines(); ++li) {
      for (std::size_t co = 0; co < get_columns(); ++co) {
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

    for (std::size_t li = 0; li < res.get_lines(); ++li) {
      for (std::size_t co = 0; co < res.get_columns(); ++co) {
        res(li, co) *= lhs;
      }
    }

    return res;
  }

  Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
    Matrix res(lhs.get_lines(), rhs.get_columns());

    if (lhs.get_columns() != rhs.get_lines()) {
      throw std::runtime_error("matrix size mismatch.");
    }

    std::size_t n = lhs.get_columns();

    for (std::size_t li = 0; li < res.get_lines(); ++li) {
      for (std::size_t co = 0; co < res.get_columns(); ++co) {
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
