// clang-format off: main header
#include <sched/tools/Statistics.h>
// clang-format on

#include <algorithm>

namespace sched {

  void Statistics::reset()
  {
    m_count = 0;
    m_mean.reset();
    m_m2.reset();
    m_min = 0.0;
    m_max = 0.0;
  }

  // https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Welford's_online_algorithm

  void Statistics::add(double x)
  {
    ++m_count;
    const double delta = x - m_mean.get_result();
    m_mean.add(delta / static_cast<double>(m_count));
    const double delta2 = x - m_mean.get_result();
    m_m2.add(delta * delta2);

    if (m_count == 1) {
      m_min = x;
      m_max = x;
    } else {
      m_min = std::min(m_min, x);
      m_max = std::max(m_max, x);
    }
  }

}
