#ifndef SCHED_TERMINATION_H
#define SCHED_TERMINATION_H

#include <cstddef>

#include <chrono>

namespace sched {

  /*
   * TimeTermination
   */

  template<typename Duration>
  class TimeTermination {
  public:
    constexpr TimeTermination(Duration duration)
    : m_duration(duration)
    {
    }

    void start()
    {
      m_start = std::chrono::steady_clock::now();
    }

    void step()
    {
      // nothing to do
    }

    bool satisfied() const
    {
      return (std::chrono::steady_clock::now() - m_start) >= m_duration;
    }

  private:
    std::chrono::time_point<std::chrono::steady_clock> m_start;
    Duration m_duration;
  };

  template<typename Duration>
  TimeTermination<Duration> terminate_after(Duration duration)
  {
    return TimeTermination<Duration>(duration);
  }

  /*
   * IterationCountTermination
   */

  class IterationCountTermination {
  public:
    constexpr IterationCountTermination(std::size_t count)
    : m_count(count)
    {
    }

    void start()
    {
      m_iteration = 0;
    }

    void step()
    {
      ++m_iteration;
    }

    bool satisfied() const
    {
      return m_iteration >= m_count;
    }

  private:
    std::size_t m_iteration = 0;
    std::size_t m_count = 0;
  };

  struct IterationCountValue {
    constexpr IterationCountValue(std::size_t count)
    : count(count)
    {
    }

    std::size_t count;
  };

  namespace literals {

    constexpr IterationCountValue operator""_iterations(unsigned long long count)
    {
      return { count };
    }

  }

  constexpr IterationCountTermination terminate_after(IterationCountValue value)
  {
    return { value.count };
  }

}

#endif // SCHED_TERMINATION_H
