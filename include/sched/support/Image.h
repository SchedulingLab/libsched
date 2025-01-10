#ifndef SCHED_IMAGE_H
#define SCHED_IMAGE_H

#include <iosfwd>
#include <vector>

#include <sched/Api.h>

namespace sched {

  // NOLINTNEXTLINE(performance-enum-size)
  enum class Color : int {
    Black = -2,
    White = -1,
  };

  namespace literals {

    constexpr Color operator""_color(unsigned long long int value)
    {
      return Color{ static_cast<int>(value) };
    }

  }

  class SCHED_API Image {
  public:
    Image(std::size_t height);
    Image(std::size_t width, std::size_t height);

    void set_color(std::size_t row, std::size_t col, Color color);
    Color get_color(std::size_t row, std::size_t col) const;

    void export_to(std::ostream& out) const;

  private:
    Color& get(std::size_t row, std::size_t col);
    const Color& get(std::size_t row, std::size_t col) const;

    std::size_t m_width;
    std::size_t m_height;
    std::vector<Color> m_data;
  };

}

#endif // SCHED_IMAGE_H
