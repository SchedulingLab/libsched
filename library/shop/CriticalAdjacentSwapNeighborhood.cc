#include <sched/shop/CriticalAdjacentSwapNeighborhood.h>

#include <algorithm>
#include <iterator>

#include <sched/shop/JobShopCriticalPath.h>
#include <sched/shop/OperationListImprovedNeighborhood.h>

namespace sched::shop {

  namespace {

    template<typename Iterator>
    bool fill_the_gap(Iterator it0, Iterator it1) {
      if (it1 < it0) {
        std::swap(it0, it1);
      }

      auto val0 = *it0;

      while (it0 < it1 && std::next(it0)->job != it0->job) {
        std::iter_swap(it0, std::next(it0));
        ++it0;
        assert(*it0 == val0);
      }

      auto val1 = *it1;

      while (it0 < it1 && std::prev(it1)->job != it1->job) {
        std::iter_swap(it1, std::prev(it1));
        --it1;
        assert(*it1 == val1);
      }

      return it0 == it1;
    }

  }

  OperationListInput OperationCriticalAdjacentSwapNeighborhood::operator()(const OperationListInput& input, const JobShopSchedule& schedule, Random& random) {
    auto critical_path = sched::shop::critical_path(schedule);

    if (critical_path.size() > 1) {
      auto max = critical_path.size() - 1;

      std::size_t n0, n1;
      std::size_t iterations = 0;
      const std::size_t max_iterations = input.size() / 10 + 1;

      do {
        n0 = random.compute_uniform_integer(std::size_t{0}, max);

        if (n0 < max && critical_path[n0].machine == critical_path[n0 + 1].machine) {
          n1 = n0 + 1;
        } else if (n0 > 0 && critical_path[n0].machine == critical_path[n0 - 1].machine) {
          n1 = n0 - 1;
        } else {
          n1 = n0;
        }

        ++iterations;
      } while (n0 == n1 && iterations <= max_iterations);

      if (iterations <= max_iterations) {
        assert(critical_path[n0].machine == critical_path[n1].machine);
        OperationListInput neighbor = input;

        auto it0 = std::find(neighbor.begin(), neighbor.end(), critical_path[n0].operation);
        assert(it0 != neighbor.end());
        auto it1 = std::find(neighbor.begin(), neighbor.end(), critical_path[n1].operation);
        assert(it1 != neighbor.end());

        if (fill_the_gap(it0, it1)) {
          return neighbor;
        }
      }
    }

    // fallback to swap
    OperationSwapNeighborhood fallback;
    return fallback(input, schedule, random);
  }

  std::vector<OperationListInput> OperationCriticalAdjacentSwapNeighborhood::generate_many(const OperationListInput& input, const JobShopSchedule& schedule, Random& random, std::size_t count) {
    auto critical_path = sched::shop::critical_path(schedule);

    if (critical_path.size() > 1) {
      std::vector<OperationListInput> result;

      // get all the possible swaps

      for (std::size_t i = 0; i < critical_path.size() - 1; ++i) {
        if (critical_path[i].machine == critical_path[i + 1].machine) {
          OperationListInput neighbor = input;

          auto it0 = std::find(neighbor.begin(), neighbor.end(), critical_path[i].operation);
          assert(it0 != neighbor.end());
          auto it1 = std::find(neighbor.begin(), neighbor.end(), critical_path[i + 1].operation);
          assert(it1 != neighbor.end());

          if (fill_the_gap(it0, it1)) {
            result.push_back(neighbor);
          }
        }
      }

      if (result.size() < count) {
        // if there are not enough, add some random swaps
        OperationSwapNeighborhood fallback;
        auto more = fallback.generate_many(input, schedule, random, count - result.size());
        result.insert(result.end(), more.begin(), more.end());
      } else {
        std::shuffle(result.begin(), result.end(), random);
        result.resize(count);
      }

      return result;
    }

    // fallback to swap
    OperationSwapNeighborhood fallback;
    return fallback.generate_many(input, schedule, random, count);
  }

  bool OperationCriticalAdjacentSwapNeighborhood::are_neighbors([[maybe_unused]] const OperationListInput& input0, [[maybe_unused]] const OperationListInput& input1) {
    // TODO
    // - identify the swap
    // - check that it belongs to the same machine or it belongs to the same job
    // WARNING: may need instance to identify the machine...
    return true;
  }

}
