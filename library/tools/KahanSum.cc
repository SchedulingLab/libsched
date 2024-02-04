// clang-format off: main header
#include <sched/tools/KahanSum.h>
// clang-format on

#include <cmath>

namespace sched {

  void KahanSum::reset()
  {
    m_sum = 0.0;
    m_c = 0.0f;
  }

  void KahanSum::add(double x)
  {
    const double t = m_sum + x;

    if (std::fabs(m_sum) > std::fabs(x)) {
      m_c += (m_sum - t) + x;
    } else {
      m_c += (x - t) + m_sum;
    }

    m_sum = t;
  }

}
