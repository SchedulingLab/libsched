#include <sched/shop/CriticalAdjacentSwapNeighborhood.h>

#include <algorithm>
#include <iterator>
#include <optional>
#include <tuple>

#include <sched/shop/JobShopCriticalPath.h>
#include <sched/shop/OperationListImprovedNeighborhood.h>
#include <sched/shop/RowNeighborhood.h>
#include <sched/shop/SwapNeighborhood.h>

namespace sched::shop {

  namespace {

    template<typename Iterator>
    bool fill_the_gap(Iterator it0, Iterator it1) {
      if (it1 < it0) {
        std::swap(it0, it1);
      }

      [[maybe_unused]] auto val0 = *it0;

      while (it0 < it1 && std::next(it0)->job != it0->job) {
        std::iter_swap(it0, std::next(it0));
        ++it0;
        assert(*it0 == val0);
      }

      [[maybe_unused]] auto val1 = *it1;

      while (it0 < it1 && std::prev(it1)->job != it1->job) {
        std::iter_swap(it1, std::prev(it1));
        --it1;
        assert(*it1 == val1);
      }

      return it0 == it1;
    }

    std::optional<std::size_t> compute_one_swappable_task(const std::vector<JobShopTask>& tasks, Random& random) {
      if (tasks.size() < 2) {
        return std::nullopt;
      }

      auto max = tasks.size() - 1;

      std::size_t index;
      bool found = false;
      std::size_t iterations = 0;
      const std::size_t max_iterations = tasks.size() / 5 + 1;

      do {
        index = random.compute_uniform_integer(std::size_t{0}, max - 1);

        if (tasks[index].machine == tasks[index + 1].machine) {
          found = true;
        }

        ++iterations;
      } while (!found && iterations < max_iterations);

      if (!found) {
        return std::nullopt;
      }

      return index;
    }

    std::vector<std::size_t> compute_all_swappable_tasks(const std::vector<JobShopTask>& tasks) {
      std::vector<std::size_t> result;

      if (tasks.empty()) {
        return result;
      }

      for (std::size_t i = 0; i < tasks.size() - 1; ++i) {
        if (tasks[i].machine == tasks[i + 1].machine) {
          result.push_back(i);
        }
      }

      return result;
    }


  }

  /*
   * OperationCriticalAdjacentSwapNeighborhood
   */

  OperationListInput OperationCriticalAdjacentSwapNeighborhood::operator()(const OperationListInput& input, const JobShopSchedule& schedule, Random& random) {
    auto critical_path = sched::shop::critical_path(schedule);
    auto maybe_swappable = compute_one_swappable_task(critical_path, random);

    if (maybe_swappable) {
      auto index = *maybe_swappable;

      assert(critical_path[index].machine == critical_path[index + 1].machine);
      OperationListInput neighbor = input;

      auto it0 = std::find(neighbor.begin(), neighbor.end(), critical_path[index].operation);
      assert(it0 != neighbor.end());
      auto it1 = std::find(neighbor.begin(), neighbor.end(), critical_path[index + 1].operation);
      assert(it1 != neighbor.end());

      if (fill_the_gap(it0, it1)) {
        return neighbor;
      }
    }

    // fallback to swap
    OperationSwapNeighborhood fallback;
    return fallback(input, schedule, random);
  }

  std::vector<OperationListInput> OperationCriticalAdjacentSwapNeighborhood::generate_many(const OperationListInput& input, const JobShopSchedule& schedule, Random& random, std::size_t count) {
    auto critical_path = sched::shop::critical_path(schedule);
    auto swappables = compute_all_swappable_tasks(critical_path);

    std::vector<OperationListInput> result;

    for (auto i : swappables) {
      assert(critical_path[i].machine == critical_path[i + 1].machine);

      OperationListInput neighbor = input;

      auto it0 = std::find(neighbor.begin(), neighbor.end(), critical_path[i].operation);
      assert(it0 != neighbor.end());
      auto it1 = std::find(neighbor.begin(), neighbor.end(), critical_path[i + 1].operation);
      assert(it1 != neighbor.end());

      if (fill_the_gap(it0, it1)) {
        result.push_back(neighbor);
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

    assert(result.size() == count);
    return result;
  }

  bool OperationCriticalAdjacentSwapNeighborhood::are_neighbors([[maybe_unused]] const OperationListInput& input0, [[maybe_unused]] const OperationListInput& input1) {
    // TODO
    // - identify the swap
    // - check that it belongs to the same machine or it belongs to the same job
    // WARNING: may need instance to identify the machine...
    return true;
  }

  /*
   * MachineCriticalAdjacentSwapNeighborhood
   */

  MachineListInput MachineCriticalAdjacentSwapNeighborhood::operator()(const MachineListInput& input, const JobShopSchedule& schedule, Random& random) {
    auto critical_path = sched::shop::critical_path(schedule);
    auto maybe_swappable = compute_one_swappable_task(critical_path, random);

    if (maybe_swappable) {
      auto index = *maybe_swappable;

      auto machine = critical_path[index].machine;
      assert(critical_path[index + 1].machine == machine);

      MachineListInput neighbor = input;
      auto & operations = neighbor[to_index(machine)];

      auto it0 = std::find(operations.begin(), operations.end(), critical_path[index].operation);
      assert(it0 != operations.end());
      auto it1 = std::find(operations.begin(), operations.end(), critical_path[index + 1].operation);
      assert(it1 != operations.end());

      assert(std::abs(it0 - it1) == 1);
      std::iter_swap(it0, it1);
      return neighbor;
    }

    // fallback to swap
    SwapNeighborhood fallback;
    return fallback(input, schedule, random);
  }

  std::vector<MachineListInput> MachineCriticalAdjacentSwapNeighborhood::generate_many(const MachineListInput& input, const JobShopSchedule& schedule, Random& random, std::size_t count) {
    auto critical_path = sched::shop::critical_path(schedule);
    auto swappables = compute_all_swappable_tasks(critical_path);

    std::vector<MachineListInput> result;

    for (auto i : swappables) {
      auto machine = critical_path[i].machine;
      assert(critical_path[i + 1].machine == machine);

      MachineListInput neighbor = input;
      auto & operations = neighbor[to_index(machine)];

      auto it0 = std::find(operations.begin(), operations.end(), critical_path[i].operation);
      assert(it0 != operations.end());
      auto it1 = std::find(operations.begin(), operations.end(), critical_path[i + 1].operation);
      assert(it1 != operations.end());

      assert(std::abs(it0 - it1) == 1);
      std::iter_swap(it0, it1);

      result.push_back(neighbor);
    }

    if (result.size() < count) {
      // if there are not enough, add some random swaps
      SwapInRowNeighborhood fallback;
      auto more = fallback.generate_many(input, schedule, random, count - result.size());
      result.insert(result.end(), more.begin(), more.end());
    } else {
      std::shuffle(result.begin(), result.end(), random);
      result.resize(count);
    }

    assert(result.size() == count);
    return result;
  }

  bool MachineCriticalAdjacentSwapNeighborhood::are_neighbors([[maybe_unused]] const MachineListInput& input0, [[maybe_unused]] const MachineListInput& input1) {
    // TODO
    return true;
  }


}
