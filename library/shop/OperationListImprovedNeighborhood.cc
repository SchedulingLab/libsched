#include <sched/shop/OperationListImprovedNeighborhood.h>

#include <sched/common/Extent.h>

#include <sched/shop/InsertNeighborhood.h>
#include <sched/shop/ReverseNeighborhood.h>
#include <sched/shop/SwapNeighborhood.h>

namespace sched::shop {

  namespace {

    Extent<std::size_t> compute_extent(const OperationListInput& input, std::size_t origin) {
      Extent<std::size_t> extent = { origin, origin };

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
    std::size_t n0, n1;

    do {
      n0 = random.compute_uniform_integer(std::size_t{0}, max);
      Extent<std::size_t> extent = compute_extent(input, n0);
      n1 = random.compute_uniform_integer(extent.lo, extent.hi);
    } while (n0 == n1 || !compute_extent(input, n1).contains(n1));

    OperationListInput neighbor = input;
    std::swap(neighbor[n0], neighbor[n1]);
    return neighbor;
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
        Extent<std::size_t> extent = compute_extent(input, orig);
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

  // OperationReverseNeighborhood

  OperationListInput OperationReverseNeighborhood::compute(const OperationListInput& input, Random& random) {
    assert(!input.empty());

    std::size_t max = input.size() - 1;
    OperationListInput neighbor;

    do {
      std::size_t endpoints[2];

      do {
        endpoints[0] = random.compute_uniform_integer(std::size_t{0}, max);
        Extent<std::size_t> extent = compute_extent(input, endpoints[0]);
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

}
