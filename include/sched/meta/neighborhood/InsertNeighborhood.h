#ifndef SCHED_INSERT_NEIHGBORHOOD_H
#define SCHED_INSERT_NEIHGBORHOOD_H

#include <cassert>

#include <string>

#include <sched/Api.h>
#include <sched/support/Random.h>
#include <sched/meta/neighborhood/NeighborhoodHelper.h>
#include <sched/types/InputConcepts.h>
#include <sched/types/NeighborhoodTraits.h>
#include <sched/meta/neighborhood/RowNeighborhood.h>

namespace sched {

  struct SCHED_API InsertNeighborhood {

    template<concepts::Input Input, typename Schedule>
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

    template<concepts::Input Input, typename Schedule>
    std::vector<Input> generate_many(const Input& input, const Schedule& schedule, Random& random, std::size_t count)
    {
      return generate_many_neighbors(*this, input, schedule, random, count);
    }

  };

  template<>
  struct NeighborhoodTraits<InsertNeighborhood> {
    static std::string name()
    {
      return "ins";
    }
  };

  using InsertMutation = InsertNeighborhood;

  using InsertInRowNeighborhood = RowNeighborhood<InsertNeighborhood>;

}

#endif // SCHED_INSERT_NEIHGBORHOOD_H
