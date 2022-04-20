#include <sched/shop/OperationListImprovedNeighborhood.h>

#include <sched/shop/InsertNeighborhood.h>
#include <sched/shop/ReverseNeighborhood.h>
#include <sched/shop/SwapNeighborhood.h>

namespace sched::shop {

  namespace {

    struct Extent {
      std::size_t lo; // inclusive
      std::size_t hi; // inclusive

      bool contains(std::size_t index) const noexcept {
        return lo <= index && index <= hi;
      }
    };

    Extent compute_extent(const OperationListInput& input, std::size_t origin) {
      Extent extent = { origin, origin };

      while (extent.lo > 0 && input[extent.lo - 1].job != input[origin].job) {
        --extent.lo;
      }

      while (extent.hi < input.size() - 1 && input[extent.hi + 1].job != input[origin].job) {
        ++extent.hi;
      }

      return extent;
    }

  }

  // OperationSwapNeighborhood

  OperationListInput OperationSwapNeighborhood::compute(const OperationListInput& input, Random& random) {
    assert(!input.empty());

    std::size_t max = input.size() - 1;
    std::size_t n1, n2;

    do {
      n1 = random.compute_uniform_integer(std::size_t{0}, max);
      Extent extent = compute_extent(input, n1);
      n2 = random.compute_uniform_integer(extent.lo, extent.hi);
    } while (n1 == n2 || !compute_extent(input, n2).contains(n1));

    OperationListInput neighbor = input;
    std::swap(neighbor[n1], neighbor[n2]);
    return neighbor;
  }

  bool OperationSwapNeighborhood::are_neighbors(const OperationListInput& input0, const OperationListInput& input1) {
    SwapNeighborhood neighborhood;
    return neighborhood.are_neighbors(input0, input1);
  }

  // OperationInsertNeighborhood

  OperationListInput OperationInsertNeighborhood::compute(const OperationListInput& input, Random& random) {
    assert(!input.empty());

    std::size_t max = input.size() - 1;
    OperationListInput neighbor;

    do {
      std::size_t orig, dest;

      do {
        orig = random.compute_uniform_integer(std::size_t{0}, max);
        Extent extent = compute_extent(input, orig);
        dest = random.compute_uniform_integer(extent.lo, extent.hi);
      } while (orig == dest);

      neighbor = input;
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
    } while (neighbor == input);


    return neighbor;
  }

  bool OperationInsertNeighborhood::are_neighbors(const OperationListInput& input0, const OperationListInput& input1) {
    InsertNeighborhood neighborhood;
    return neighborhood.are_neighbors(input0, input1);
  }

  // OperationReverseNeighborhood

  OperationListInput OperationReverseNeighborhood::compute(const OperationListInput& input, Random& random) {
    assert(!input.empty());

    std::size_t max = input.size() - 1;
    OperationListInput neighbor;

    do {
      std::size_t endpoints[2];

      do {
        endpoints[0] = random.compute_uniform_integer(std::size_t{0}, max);
        Extent extent = compute_extent(input, endpoints[0]);
        endpoints[1] = random.compute_uniform_integer(extent.lo, extent.hi);
      } while (endpoints[0] == endpoints[1] || !compute_extent(input, endpoints[1]).contains(endpoints[0]));

      if (endpoints[0] > endpoints[1]) {
        std::swap(endpoints[0], endpoints[1]);
      }

      neighbor = input;
      std::reverse(neighbor.begin() + endpoints[0], neighbor.begin() + endpoints[1] + 1);
    } while (neighbor == input);

    return neighbor;
  }

  bool OperationReverseNeighborhood::are_neighbors(const OperationListInput& input0, const OperationListInput& input1) {
    ReverseNeighborhood neighborhood;
    return neighborhood.are_neighbors(input0, input1);
  }

}
