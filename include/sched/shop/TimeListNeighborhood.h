#ifndef SCHED_SHOP_TIME_LIST_NEIGHBORHOOD_H
#define SCHED_SHOP_TIME_LIST_NEIGHBORHOOD_H

#include <algorithm>
#include <random>
#include <string>

#include <sched/common/Api.h>
#include <sched/common/Random.h>

#include "NeighborhoodHelper.h"
#include "NeighborhoodTraits.h"
#include "TimeListInput.h"

namespace sched::shop {

  struct SCHED_API TimeListNeighborhood {

    template<typename Schedule>
    TimeListInput operator()(const TimeListInput& input, [[maybe_unused]] const Schedule& schedule, Random& random) {
      TimeListInput neighbor;
      std::bernoulli_distribution dist_change(0.15);

      do {
        neighbor = input;

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
      return NeighborhoodHelper::generate_many(*this, input, schedule, random, count);
    }

    bool are_neighbors([[maybe_unused]] const TimeListInput& input0, [[maybe_unused]] const TimeListInput& input1) {
      return true;
    }

  };

  template<>
  struct NeighborhoodTraits<TimeListNeighborhood> {
    static std::string name() {
      return "tim";
    }
  };

}

#endif // SCHED_SHOP_TIME_LIST_NEIGHBORHOOD_H
