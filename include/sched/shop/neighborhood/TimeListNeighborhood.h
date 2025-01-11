// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_TIME_LIST_NEIGHBORHOOD_H
#define SCHED_SHOP_TIME_LIST_NEIGHBORHOOD_H

#include <algorithm>
#include <random>
#include <string>

#include <sched/Api.h>
#include <sched/support/Random.h>
#include <sched/meta/neighborhood/NeighborhoodHelper.h>
#include <sched/types/InputConcepts.h>
#include <sched/types/NeighborhoodTraits.h>
#include <sched/shop/input/TimeListInput.h>

namespace sched {

  namespace shop {

    struct SCHED_API TimeListNeighborhood {

      template<typename Schedule>
      TimeListInput operator()(const TimeListInput& input, [[maybe_unused]] const Schedule& schedule, Random& random)
      {
        std::bernoulli_distribution dist_change(0.15);
        TimeListInput neighbor = input;

        while (neighbor == input) {
          for (auto& lag : neighbor) {
            if (dist_change(random)) {
              std::negative_binomial_distribution<Time> dist_time(lag == 0 ? 4 : 4 * lag, 0.8);
              lag = std::max(dist_time(random), Time(0));
            }
          }
        }

        return neighbor;
      }

      template<typename Input, typename Schedule>
      std::vector<Input> generate_many(const Input& input, const Schedule& schedule, Random& random, std::size_t count)
      {
        return generate_many_neighbors(*this, input, schedule, random, count);
      }

    };

  }

  template<>
  struct NeighborhoodTraits<shop::TimeListNeighborhood> {
    static std::string name()
    {
      return "tim";
    }
  };

}

#endif // SCHED_SHOP_TIME_LIST_NEIGHBORHOOD_H
