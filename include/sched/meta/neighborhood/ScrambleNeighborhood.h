#ifndef SCHED_SCRAMBLE_NEIHGBORHOOD_H
#define SCHED_SCRAMBLE_NEIHGBORHOOD_H

#include <cassert>

#include <algorithm>
#include <string>

#include <sched/Api.h>
#include <sched/support/Random.h>
#include <sched/meta/neighborhood/NeighborhoodHelper.h>
#include <sched/types/InputConcepts.h>
#include <sched/types/NeighborhoodTraits.h>

namespace sched {

  struct SCHED_API ScrambleNeighborhood {

    template<concepts::Input Input, typename Schedule>
    Input operator()(const Input& input, [[maybe_unused]] const Schedule& schedule, Random& random)
    {
      assert(!input.empty());

      const std::size_t max = input.size() - 1;
      Input neighbor = input;

      while (neighbor == input) {
        std::size_t endpoints[2] = { 0, 0 };

        while (endpoints[0] == endpoints[1]) {
          endpoints[0] = random.compute_uniform_integer(std::size_t(0), max);
          endpoints[1] = random.compute_uniform_integer(std::size_t(0), max);
        }

        if (endpoints[0] > endpoints[1]) {
          std::swap(endpoints[0], endpoints[1]);
        }

        std::shuffle(&neighbor[endpoints[0]], &neighbor[endpoints[1]] + 1, random);
      }

      return neighbor;
    }

    template<concepts::Input Input, typename Schedule>
    std::vector<Input> generate_many(const Input& input, const Schedule& schedule, Random& random, std::size_t count)
    {
      return generate_many_neighbors(*this, input, schedule, random, count);
    }

  };

  template<>
  struct NeighborhoodTraits<ScrambleNeighborhood> {
    static std::string name()
    {
      return "scr";
    }
  };

  using ScrambleMutation = ScrambleNeighborhood;

}

#endif // SCHED_SCRAMBLE_NEIHGBORHOOD_H
