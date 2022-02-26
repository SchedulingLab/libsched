#ifndef SCHED_SHOP_ROW_NEIGHBORHOOD_H
#define SCHED_SHOP_ROW_NEIGHBORHOOD_H

#include <sched/common/Random.h>

namespace sched::shop {

  template<typename BaseNeighborhood>
  struct RowNeighborhood {

    template<typename Input>
    Input operator()(const Input& input, Random& random) {
      assert(!input.empty());
      std::size_t rows = input.size() - 1;
      Input neighbor;

      do {
        neighbor = input;
        auto& row = neighbor[random.compute_uniform_integer(std::size_t{0}, rows)];

        if (row.size() <= 1) {
          continue;
        }

        row = base_neighborhood(row, random);
      } while (neighbor == input);

      return neighbor;
    }

    template<typename Input>
    bool are_neighbors(const Input& input0, const Input& input1) {
      assert(input0.size() == input1.size());

      bool neighbors = false;

      for (auto i : sched::over(input0)) {
        assert(input0[i].size() == input1[i].size());

        if (input0[i] == input1[i]) {
          continue;
        }

        if (base_neighborhood.are_neighbors(input0[i], input1[i])) {
          if (neighbors) {
            // there is already a neighbor row
            return false;
          }

          neighbors = true;
        } else {
          // they are not neighbors
          return false;
        }
      }

      return neighbors;
    }

    BaseNeighborhood base_neighborhood;
  };



  struct SwapNeighborhood;
  using SwapInRowNeighborhood = RowNeighborhood<SwapNeighborhood>;

  struct InsertNeighborhood;
  using InsertInRowNeighborhood = RowNeighborhood<InsertNeighborhood>;

  struct ReverseNeighborhood;
  using ReverseInRowNeighborhood = RowNeighborhood<ReverseNeighborhood>;

}

#endif // SCHED_SHOP_ROW_NEIGHBORHOOD_H
