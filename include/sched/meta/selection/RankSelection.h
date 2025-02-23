// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SCHOP_RANK_SELECTION_H
#define SCHED_SCHOP_RANK_SELECTION_H

#include <cassert>

#include <sched/support/Fixed.h>
#include <sched/support/Random.h>

namespace sched {

  template<int64_t Pressure>
  struct RankSelection {

    template<typename Solution>
    std::vector<Solution> operator()(const std::vector<Solution>& solutions, std::size_t selected_count, Random& random)
    {
      using Fixed = BasicFixed<1000>;

      const std::size_t count = solutions.size();
      const Fixed selection_pressure(1.0 + (Pressure / 100.0));
      std::vector<int64_t> weights;
      weights.reserve(count);

      for (std::size_t i = 0; i < count; ++i) {
        weights.push_back(((count - 1) * selection_pressure + (2 * selection_pressure - 1) * i).as_raw());
      }

      std::discrete_distribution<std::size_t> distribution(weights.begin(), weights.end());
      std::vector<Solution> selected;

      for (std::size_t i = 0; i < selected_count; ++i) {
        const std::size_t index = distribution(random);
        assert(index < solutions.size());
        selected.push_back(solutions[index]);
      }

      return selected;
    }
  };

}

#endif // SCHED_SCHOP_RANK_SELECTION_H
