// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/input/HoistInputConversion.h>

#include <algorithm>
#include <set>

namespace sched::shop {

  namespace {

    MachineId next_machine(MachineId machine, const HoistSchedulingInstance& instance)
    {
      return static_cast<MachineId>((static_cast<std::size_t>(machine) + 1) % instance.machine_count());
    }

  }

  HoistEmptyInput to_empty_input(const HoistLoadedInput& loaded_input, const HoistSchedulingInstance& instance)
  {
    assert(loaded_input.size() == instance.machine_count());
    const std::vector<Move> empty_moves = compute_empty_moves(loaded_input);

    /*
     * reconstitutes the cycles
     */

    std::set<Move> empty_moves_set(empty_moves.begin(), empty_moves.end());

    std::vector<std::vector<MachineId>> cycles;

    for (auto iterator = empty_moves_set.begin(); iterator != empty_moves_set.end(); ) {
      std::vector<MachineId> cycle;

      const MachineId first_machine = iterator->orig;
      cycle.push_back(first_machine);

      MachineId current_machine = iterator->dest;

      for (;;) {
        cycle.push_back(current_machine);
        auto next_iterator = std::ranges::find(empty_moves_set, current_machine, &Move::orig);
        assert(next_iterator != empty_moves_set.end());

        current_machine = next_iterator->dest;
        empty_moves_set.erase(next_iterator);

        if (current_machine == first_machine) {
          break;
        }
      }

      iterator = empty_moves_set.erase(iterator);

      auto minimum_iterator = std::ranges::min_element(cycle);
      std::ranges::rotate(cycle, minimum_iterator); // put minimum first

      cycles.push_back(std::move(cycle));
    }

    std::ranges::sort(cycles, {}, [](const std::vector<MachineId>& cycle) { return cycle.front(); });

    /*
     * Recreate the empty input
     */

    HoistEmptyInput empty_input = {};

    std::vector<std::size_t> indices;
    indices.reserve(cycles.size());

    for (const std::vector<MachineId>& cycle : cycles) {
      indices.push_back(cycle.size());
      empty_input.machines.insert(empty_input.machines.end(), cycle.begin(), cycle.end());
    }

    if (!indices.empty()) {
      std::partial_sum(indices.begin(), indices.end(), indices.begin());
      empty_input.count = indices.back();
      assert(empty_input.machines.size() == empty_input.count);
      indices.pop_back();

      // find the float index corresponding to partition

      Partition partition(empty_input.count, indices);
      const PartitionGroup partition_group(empty_input.count);
      const std::size_t index = partition_group.find_partition(partition);
      assert(index < partition_group.size());

      empty_input.float_index = (static_cast<double>(index) + 0.5) / static_cast<double>(partition_group.size());
    }

    // put missing machines at the end

    for (const MachineId machine : machines(instance)) {
      if (!std::ranges::contains(empty_input.machines, machine)) {
        empty_input.machines.push_back(machine);
      }
    }

    assert(empty_input.machines.size() == instance.machine_count());

    return empty_input;
  }

  HoistLoadedInput to_loaded_input(const HoistEmptyInput& empty_input, const HoistSchedulingInstance& instance)
  {
    HoistLoadedInput loaded_input = {};



    return loaded_input;
  }

}
