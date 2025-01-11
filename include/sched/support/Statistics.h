// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_STATISTICS_H
#define SCHED_STATISTICS_H

#include <cstddef>

#include <sched/Api.h>
#include <sched/support/KahanSum.h>

namespace sched {

  class SCHED_API Statistics {
  public:

    void reset();
    void add(double x);

    std::size_t get_count() const
    {
      return m_count;
    }

    double get_mean() const
    {
      return m_mean.get_result();
    }

    double get_variance() const
    {
      return m_m2.get_result() / static_cast<double>(m_count);
    }

    double get_sample_variance() const
    {
      return m_m2.get_result() / static_cast<double>(m_count - 1);
    }

    double get_min() const
    {
      return m_min;
    }

    double get_max() const
    {
      return m_max;
    }

  private:
    std::size_t m_count = 0;
    KahanSum m_mean;
    KahanSum m_m2;
    double m_min = 0.0;
    double m_max = 0.0;
  };

}

#endif // SCHED_STATISTICS_H
