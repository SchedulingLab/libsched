#ifndef SCHED_SHOP_TIME_LIST_NEIGHBORHOOD_H
#define SCHED_SHOP_TIME_LIST_NEIGHBORHOOD_H

#include <algorithm>
#include <random>

#include <sched/common/Api.h>
#include <sched/common/Random.h>

#include "BasicNeighborhood.h"
#include "TimeListInput.h"

namespace sched::shop {

  struct SCHED_API TimeListNeighborhood : BasicNeighborhood {

    template<typename Schedule>
    TimeListInput operator()(const TimeListInput& input, [[maybe_unused]] const Schedule& schedule, Random& random) {
      TimeListInput neighbor = input;
      std::bernoulli_distribution dist_change(0.15);

      do {
        for (auto & lag : neighbor) {
          if (dist_change(random)) {
            std::negative_binomial_distribution<Time> dist_time(lag == 0 ? 4 : 4 * lag, 0.8);
            lag = std::max(dist_time(random), Time(0));
          }
        }
      } while (input == neighbor);

      return neighbor;
    }

    template<typename Input, typename Schedule>
    std::vector<Input> generate_many(const Input& input, const Schedule& schedule, Random& random, std::size_t count) {
      return BasicNeighborhood::generate_many(*this, input, schedule, random, count);
    }

    bool are_neighbors([[maybe_unused]] const TimeListInput& input0, [[maybe_unused]] const TimeListInput& input1) {
      return true;
    }

  };

}

#endif // SCHED_SHOP_TIME_LIST_NEIGHBORHOOD_H
