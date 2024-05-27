#ifndef SCHED_REVERSE_NEIHGBORHOOD_H
#define SCHED_REVERSE_NEIHGBORHOOD_H

#include <cassert>

#include <string>

#include <algorithm>

#include <sched/common/Api.h>
#include <sched/common/Random.h>

#include "NeighborhoodHelper.h"
#include "NeighborhoodTraits.h"

namespace sched::shop {

  struct SCHED_API ReverseNeighborhood {

    template<typename Input, typename Schedule>
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

        std::reverse(&neighbor[endpoints[0]], &neighbor[endpoints[1]] + 1);
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
  struct NeighborhoodTraits<ReverseNeighborhood> {
    static std::string name()
    {
      return "rev";
    }
  };

  struct SCHED_API CappedReverseNeighborhood {

    template<typename Input, typename Schedule>
    Input operator()(const Input& input, [[maybe_unused]] const Schedule& schedule, Random& random)
    {
      assert(!input.empty());

      const std::size_t max = input.size() - 1;
      const std::size_t cap = std::max(max / 5, std::size_t(5));
      Input neighbor = input;

      while (neighbor == input) {
        std::size_t endpoints[2] = { 0, 0 };

        while (endpoints[0] == endpoints[1] || (endpoints[1] - endpoints[0] > cap)) {
          endpoints[0] = random.compute_uniform_integer(std::size_t(0), max);
          endpoints[1] = random.compute_uniform_integer(std::size_t(0), max);

          if (endpoints[0] > endpoints[1]) {
            std::swap(endpoints[0], endpoints[1]);
          }
        }

        std::reverse(&neighbor[endpoints[0]], &neighbor[endpoints[1]] + 1);
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
  struct NeighborhoodTraits<CappedReverseNeighborhood> {
    static std::string name()
    {
      return "crv";
    }
  };

}

#endif // SCHED_REVERSE_NEIHGBORHOOD_H
