#ifndef SCHED_RANDOM_NEW_NEIGHBORHOOD_H
#define SCHED_RANDOM_NEW_NEIGHBORHOOD_H

#include <cassert>

#include <random>
#include <string>

#include <sched/Api.h>
#include <sched/support/Random.h>
#include <sched/meta/input/FloatListInput.h>
#include <sched/meta/neighborhood/NeighborhoodHelper.h>
#include <sched/types/NeighborhoodTraits.h>

namespace sched {

  struct SCHED_API RandomNewNeighborhood {

    template<typename Schedule>
    FloatListInput operator()(const FloatListInput& input, [[maybe_unused]] const Schedule& schedule, Random& random)
    {
      assert(!input.empty());
      std::uniform_real_distribution<double> dist_value(0.0, 1.0);
      std::bernoulli_distribution dist_change(0.15);
      FloatListInput neighbor = input;

      while (neighbor == input) {
        for (auto& value : neighbor) {
          if (dist_change(random)) {
            value = dist_value(random);
          }
        }
      }

      return neighbor;
    }

    template<typename Schedule>
    std::vector<FloatListInput> generate_many(const FloatListInput& input, const Schedule& schedule, Random& random, std::size_t count)
    {
      return generate_many_neighbors(*this, input, schedule, random, count);
    }
  };

  template<>
  struct NeighborhoodTraits<RandomNewNeighborhood> {
    static std::string name()
    {
      return "rnd";
    }
  };

  using RandomNewMutation = RandomNewNeighborhood;

}

#endif // SCHED_RANDOM_NEW_NEIGHBORHOOD_H
