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

  template<class T>
  class IteratorRange {
  public:
    using Iterator = T;

    IteratorRange(Iterator beg, Iterator end)
    : m_beg(beg), m_end(end)
    {
    }

    Iterator begin()
    {
      return m_beg;
    }

    Iterator end()
    {
      return m_end;
    }

  private:
    T m_beg;
    T m_end;
  };

  template<class T>
  inline IteratorRange<typename T::iterator> make_iterator_range(T& cont)
  {
    return IteratorRange<typename T::iterator>(cont.begin(), cont.end());
  }

  template<class T>
  inline IteratorRange<typename T::const_iterator> make_iterator_range(const T& cont)
  {
    return IteratorRange<typename T::const_iterator>(cont.begin(), cont.end());
  }

}

#endif // SCHED_RANGE_H
