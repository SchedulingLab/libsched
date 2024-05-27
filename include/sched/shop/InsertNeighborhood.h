#ifndef SCHED_INSERT_NEIHGBORHOOD_H
#define SCHED_INSERT_NEIHGBORHOOD_H

#include <cassert>

#include <string>

#include <sched/common/Api.h>
#include <sched/common/Random.h>
#include <sched/common/InputConcepts.h>

#include "NeighborhoodHelper.h"
#include "NeighborhoodTraits.h"

namespace sched::shop {

  struct SCHED_API InsertNeighborhood {

    template<typename Input, typename Schedule>
      requires(concepts::Input<Input>)
    Input operator()(const Input& input, [[maybe_unused]] const Schedule& schedule, Random& random)
    {
      assert(!input.empty());

      const std::size_t max = input.size() - 1;
      Input neighbor = input;

      while (neighbor == input) {
        std::size_t orig = 0;
        std::size_t dest = 0;

        while (orig == dest) {
          orig = random.compute_uniform_integer(std::size_t(0), max);
          dest = random.compute_uniform_integer(std::size_t(0), max);
        }

        auto val = neighbor[orig];

        if (orig < dest) {
          for (std::size_t i = orig; i < dest; ++i) {
            neighbor[i] = neighbor[i + 1];
          }
        } else {
          for (std::size_t i = orig; i > dest; --i) {
            neighbor[i] = neighbor[i - 1];
          }
        }

        neighbor[dest] = val;
      }

      return neighbor;
    }

    template<typename Input, typename Schedule>
    std::vector<Input> generate_many(const Input& input, const Schedule& schedule, Random& random, std::size_t count)
    {
      return NeighborhoodHelper::generate_many(*this, input, schedule, random, count);
    }

  };

  template<>
  struct NeighborhoodTraits<InsertNeighborhood> {
    static std::string name()
    {
      return "ins";
    }
  };

}

#endif // SCHED_INSERT_NEIHGBORHOOD_H
