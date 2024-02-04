#ifndef SCHED_SHOP_ROW_NEIGHBORHOOD_H
#define SCHED_SHOP_ROW_NEIGHBORHOOD_H

#include <string>

#include <sched/common/Random.h>

#include "AdjacentSwapNeighborhood.h"
#include "InsertNeighborhood.h"
#include "NeighborhoodHelper.h"
#include "NeighborhoodTraits.h"
#include "ReverseNeighborhood.h"
#include "SwapNeighborhood.h"

namespace sched::shop {

  template<typename BaseNeighborhood>
  struct RowNeighborhood : private BaseNeighborhood {

    template<typename Input, typename Schedule>
    Input operator()(const Input& input, const Schedule& schedule, Random& random)
    {
      assert(!input.empty());
      std::size_t rows = input.size() - 1;
      Input neighbor;

      do {
        neighbor = input;
        auto& row = neighbor[random.compute_uniform_integer(std::size_t{ 0 }, rows)];

        if (row.size() <= 1) {
          continue;
        }

        row = BaseNeighborhood::operator()(row, schedule, random);
      } while (neighbor == input);

      return neighbor;
    }

    template<typename Input, typename Schedule>
    std::vector<Input> generate_many(const Input& input, const Schedule& schedule, Random& random, std::size_t count)
    {
      return NeighborhoodHelper::generate_many(*this, input, schedule, random, count);
    }
  };

  template<typename BaseNeighborhood>
  struct NeighborhoodTraits<RowNeighborhood<BaseNeighborhood>> {
    static std::string name()
    {
      return NeighborhoodTraits<BaseNeighborhood>::name();
    }
  };

  using SwapInRowNeighborhood = RowNeighborhood<SwapNeighborhood>;
  using InsertInRowNeighborhood = RowNeighborhood<InsertNeighborhood>;
  using ReverseInRowNeighborhood = RowNeighborhood<ReverseNeighborhood>;
  using AdjacentSwapInRowNeighborhood = RowNeighborhood<AdjacentSwapNeighborhood>;

}

#endif // SCHED_SHOP_ROW_NEIGHBORHOOD_H
