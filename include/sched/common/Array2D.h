#ifndef SCHED_COMMON_ARRAY_2D_H
#define SCHED_COMMON_ARRAY_2D_H

#include <cassert>

#include <vector>

#include "Range.h"

namespace sched {

  template<typename T>
  class Span {
  public:
    Span(T* ptr, std::size_t size)
    : m_ptr(ptr)
    , m_size(size)
    {
    }

    T& operator[](std::size_t index) {
      assert(index < m_size);
      return m_ptr[index];
    }

    const T& operator[](std::size_t index) const {
      assert(index < m_size);
      return m_ptr[index];
    }

    T* begin() {
      return m_ptr;
    }

    const T* begin() const {
      return m_ptr;
    }

    T* end() {
      return m_ptr + m_size;
    }

    const T* end() const {
      return m_ptr + m_size;
    }

  private:
    T* m_ptr;
    std::size_t m_size;
  };

  template<typename T>
  Span<T> span(T* ptr, std::size_t size) {
    return Span<T>(ptr, size);
  }


  template<typename T>
  class Array2D {
  public:

    Array2D()
    : m_rows(0)
    , m_cols(0)
    {
    }

    Array2D(std::size_t rows, std::size_t cols)
    : m_rows(rows)
    , m_cols(cols)
    , m_data(rows * cols)
    {
    }

    Array2D(std::size_t rows, std::size_t cols, const T& value)
    : m_rows(rows)
    , m_cols(cols)
    , m_data(rows * cols, value)
    {
    }

    std::size_t rows() const noexcept
    {
      return m_rows;
    }

    std::size_t  cols() const noexcept
    {
      return m_cols;
    }

    bool empty() const noexcept
    {
      return m_data.empty();
    }

    bool valid(std::size_t row, std::size_t col) const noexcept
    {
      return row < m_rows && col < m_cols;
    }

    const T& operator()(std::size_t row, std::size_t col) const noexcept
    {
      assert(valid(row, col));
      return m_data[linearize(row, col)];
    }

    T& operator()(std::size_t row, std::size_t col) noexcept
    {
      assert(valid(row, col));
      return m_data[linearize(row, col)];
    }

    auto operator[](std::size_t row) {
      return span(m_data.data() + row * m_cols, m_cols);
    }

    auto operator[](std::size_t row) const {
      return span(m_data.data() + row * m_cols, m_cols);
    }

    const T* raw_data() const noexcept
    {
      return m_data.data();
    }

    std::size_t raw_size() const noexcept
    {
      return m_data.size();
    }

  private:
    std::size_t linearize(std::size_t row, std::size_t col) const
    {
      return row * m_cols + col;
    }

    std::size_t m_rows;
    std::size_t m_cols;
    std::vector<T> m_data;
  };

} // namespace sched

#endif // SCHED_COMMON_ARRAY_2D_H
