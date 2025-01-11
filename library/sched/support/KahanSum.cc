// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/support/KahanSum.h>

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
