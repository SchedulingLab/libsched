#ifndef SCHED_RANGE_H
#define SCHED_RANGE_H

#include <cstddef>

#include <iterator>

namespace sched {

  template<typename Id>
  struct Range {
    std::size_t size;

    struct Iterator {
      using difference_type = std::ptrdiff_t;
      using value_type = Id;
      using pointer = value_type;
      using reference = value_type;
      using iterator_category = std::bidirectional_iterator_tag;

      std::size_t index;

      void swap(Iterator& other) noexcept
      {
        using std::swap;
        swap(index, other.index);
      }

      reference operator*() noexcept
      {
        return Id{ index };
      }

      pointer operator->() noexcept
      {
        return Id{ index };
      }

      Iterator& operator++() noexcept
      {
        ++index;
        return *this;
      }

      Iterator operator++(int) noexcept
      {
        Iterator copy = *this;
        ++index;
        return copy;
      }

      Iterator& operator--() noexcept
      {
        --index;
        return *this;
      }

      constexpr bool operator!=(const Iterator& other) const noexcept
      {
        return index != other.index;
      }

      constexpr bool operator==(const Iterator& other) const noexcept
      {
        return index == other.index;
      }
    };

    constexpr Iterator begin() const noexcept
    {
      return Iterator{ 0 };
    }

    constexpr Iterator end() const noexcept
    {
      return Iterator{ size };
    }
  };

  template<typename T>
  Range<std::size_t> over(const T& container)
  {
    return { container.size() };
  }

}

#endif // SCHED_RANGE_H
