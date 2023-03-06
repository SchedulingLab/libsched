#ifndef SCHED_COMMON_FIXED_H
#define SCHED_COMMON_FIXED_H

#include <cstdint>

namespace sched {

  template<int64_t Factor>
  class BasicFixed {
  public:
    constexpr BasicFixed()
    : m_data(0)
    {
    }

    template<typename T>
    constexpr BasicFixed(T val)
    : m_data(static_cast<int64_t>(val * Factor))
    {
    }

    constexpr int64_t as_int() const {
      return m_data / Factor;
    }

    constexpr double as_float() const {
      return static_cast<double>(m_data) / static_cast<double>(Factor);
    }

    constexpr int64_t as_raw() const {
      return m_data;
    }

    constexpr BasicFixed& operator+=(BasicFixed other) {
      m_data += other.m_data;
      return *this;
    }

    constexpr BasicFixed& operator-=(BasicFixed other) {
      m_data -= other.m_data;
      return *this;
    }

    constexpr BasicFixed& operator*=(BasicFixed other) {
      m_data *= other.m_data;
      m_data /= Factor;
      return *this;
    }

    constexpr BasicFixed& operator/=(BasicFixed other) {
      m_data *= Factor;
      m_data /= other.m_data;
      return *this;
    }

    constexpr bool operator==(BasicFixed other) const {
      return m_data == other.m_data;
    }

    constexpr bool operator!=(BasicFixed other) const {
      return m_data != other.m_data;
    }

    constexpr bool operator<(BasicFixed other) const {
      return m_data < other.m_data;
    }

    constexpr bool operator<=(BasicFixed other) const {
      return m_data <= other.m_data;
    }

    constexpr bool operator>(BasicFixed other) const {
      return m_data > other.m_data;
    }

    constexpr bool operator>=(BasicFixed other) const {
      return m_data >= other.m_data;
    }

  private:
    int64_t m_data;
  };

  template<int64_t Factor>
  constexpr BasicFixed<Factor> operator+(BasicFixed<Factor> lhs, BasicFixed<Factor> rhs) { return lhs += rhs; }

  template<int64_t Factor, typename T>
  constexpr BasicFixed<Factor> operator+(T lhs, BasicFixed<Factor> rhs) { return BasicFixed<Factor>(lhs) + rhs; }

  template<int64_t Factor, typename T>
  constexpr BasicFixed<Factor> operator+(BasicFixed<Factor> lhs, T rhs) { return lhs + BasicFixed<Factor>(rhs); }

  template<int64_t Factor>
  constexpr BasicFixed<Factor> operator-(BasicFixed<Factor> lhs, BasicFixed<Factor> rhs) { return lhs -= rhs; }

  template<int64_t Factor, typename T>
  constexpr BasicFixed<Factor> operator-(T lhs, BasicFixed<Factor> rhs) { return BasicFixed<Factor>(lhs) - rhs; }

  template<int64_t Factor, typename T>
  constexpr BasicFixed<Factor> operator-(BasicFixed<Factor> lhs, T rhs) { return lhs - BasicFixed<Factor>(rhs); }

  template<int64_t Factor>
  constexpr BasicFixed<Factor> operator*(BasicFixed<Factor> lhs, BasicFixed<Factor> rhs) { return lhs *= rhs; }

  template<int64_t Factor, typename T>
  constexpr BasicFixed<Factor> operator*(T lhs, BasicFixed<Factor> rhs) { return BasicFixed<Factor>(lhs) * rhs; }

  template<int64_t Factor, typename T>
  constexpr BasicFixed<Factor> operator*(BasicFixed<Factor> lhs, T rhs) { return lhs * BasicFixed<Factor>(rhs); }

  template<int64_t Factor>
  constexpr BasicFixed<Factor> operator/(BasicFixed<Factor> lhs, BasicFixed<Factor> rhs) { return lhs /= rhs; }

  template<int64_t Factor, typename T>
  constexpr BasicFixed<Factor> operator/(T lhs, BasicFixed<Factor> rhs) { return BasicFixed<Factor>(lhs) / rhs; }

  template<int64_t Factor, typename T>
  constexpr BasicFixed<Factor> operator/(BasicFixed<Factor> lhs, T rhs) { return lhs / BasicFixed<Factor>(rhs); }

  using Fixed = BasicFixed<1000000>;

}

#endif // SCHED_COMMON_FIXED_H
