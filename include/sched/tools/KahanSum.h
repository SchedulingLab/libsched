#ifndef SCHED_KAHAN_SUM_H
#define SCHED_KAHAN_SUM_H

#include <sched/common/Api.h>

namespace sched {

  // improved Kahan–Babuška algorithm
  class SCHED_API KahanSum {
  public:

    void reset();
    void add(double x);

    double get_result() const
    {
      return m_sum + m_c;
    }

  private:
    double m_sum = 0;
    double m_c = 0;
  };

}

#endif // SCHED_KAHAN_SUM_H
