#ifndef SCHED_SHOP_ROW_NEIGHBORHOOD_H
#define SCHED_SHOP_ROW_NEIGHBORHOOD_H

#include <string>

#include <sched/meta/neighborhood/NeighborhoodHelper.h>
#include <sched/support/Random.h>
#include <sched/types/InputConcepts.h>
#include <sched/types/NeighborhoodTraits.h>

namespace sched {

  template<typename BaseNeighborhood>
  struct RowNeighborhood : private BaseNeighborhood {

    template<concepts::Input Input, typename Schedule>
    Input operator()(const Input& input, const Schedule& schedule, Random& random)
    {
      assert(!input.empty());
      std::size_t rows = input.size() - 1;
      Input neighbor = input;

      while (neighbor == input) {
        auto& row = neighbor[random.compute_uniform_integer(std::size_t{ 0 }, rows)];

        if (row.size() <= 1) {
          continue;
        }

        row = BaseNeighborhood::operator()(row, schedule, random);
      }

      return neighbor;
    }

    template<concepts::Input Input, typename Schedule>
    std::vector<Input> generate_many(const Input& input, const Schedule& schedule, Random& random, std::size_t count)
    {
      return generate_many_neighbors(*this, input, schedule, random, count);
    }
  };

  template<typename BaseNeighborhood>
  struct NeighborhoodTraits<RowNeighborhood<BaseNeighborhood>> {
    static std::string name()
    {
      return NeighborhoodTraits<BaseNeighborhood>::name();
    }
  };

}

#endif // SCHED_SHOP_ROW_NEIGHBORHOOD_H
