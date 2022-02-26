#include <sched/tools/Image.h>

#include <cassert>
#include <cstdint>
#include <cinttypes>
#include <iostream>
#include <map>

namespace sched {
  namespace {

    struct RGB {
      uint8_t r;
      uint8_t g;
      uint8_t b;
    };

    class Palette {
    public:
      Palette()
      : m_state(42)
      {
        m_colors[Color::White] = { 255, 255, 255 };
        m_colors[Color::Black] = { 0, 0, 0 };
      }

      RGB get_rgb(Color color) {
        auto it = m_colors.find(color);

        if (it != m_colors.end()) {
          return it->second;
        }

        m_state = (UINT32_C(1664525) * m_state) + UINT32_C(1013904223);

        RGB rgb;
        rgb.r = static_cast<uint8_t>(m_state >> 24);
        rgb.g = static_cast<uint8_t>(m_state >> 16);
        rgb.b = static_cast<uint8_t>(m_state >>  8);

        m_colors.insert({ color, rgb });

        return rgb;
      }

    private:
      uint32_t m_state;
      std::map<Color, RGB> m_colors;
    };

  }

  Image::Image(std::size_t height)
  : m_width(0)
  , m_height(height)
  {
  }

  Image::Image(std::size_t width, std::size_t height)
  : m_width(width)
  , m_height(height)
  , m_data(m_width * m_height, Color::White)
  {
  }

  void Image::set_color(std::size_t row, std::size_t col, Color color) {
    get(row, col) = color;
  }

  Color Image::get_color(std::size_t row, std::size_t col) const {
    return get(row, col);
  }

  void Image::export_to(std::ostream& out) {
    out << "P3\n";
    out << m_width << ' ' << m_height << ' ' << 255 << '\n';

    Palette pal;

    for (std::size_t row = 0; row < m_height; ++row) {
      for (std::size_t col = 0; col < m_width; ++col) {
        RGB rgb = pal.get_rgb(get_color(row, col));
        out << static_cast<int>(rgb.r) << ' ' << static_cast<int>(rgb.g) << ' ' << static_cast<int>(rgb.b) << ' ';
      }

      out << '\n';
    }
  }

  Color& Image::get(std::size_t row, std::size_t col) {
    assert(row < m_height);

    if (col >= m_width) {
      m_width = col + 1;
      m_data.resize(m_width * m_height, Color::White);
    }

    assert(col < m_width);
    return m_data[col * m_height + row];
  }

  const Color& Image::get(std::size_t row, std::size_t col) const {
    assert(row < m_height);
    assert(col < m_width);
    return m_data[col * m_height + row];
  }

}

