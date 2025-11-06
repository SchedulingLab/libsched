// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/input/HoistInputConversion.h>

#include <cassert>

#include <algorithm>
#include <ranges>

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
    std::vector<Move> empty_moves = compute_empty_moves(loaded_input);

    std::vector<std::size_t> indices;

    auto start_iterator = empty_moves.begin();

    while (start_iterator != empty_moves.end()) {
      // put the minimum at the beginning

      auto min_iterator = std::ranges::min_element(std::ranges::subrange(start_iterator, empty_moves.end()), {}, &Move::orig);
      std::iter_swap(start_iterator, min_iterator);

      // construct the cycle

      sched::MachineId next_machine = start_iterator->dest;
      auto current_iterator = std::next(start_iterator);

      while (next_machine != start_iterator->orig) {
        auto next_iterator = std::ranges::find(std::ranges::subrange(current_iterator, empty_moves.end()), next_machine, &Move::orig);
        assert(next_iterator != empty_moves.end());
        next_machine = next_iterator->dest;
        std::iter_swap(current_iterator, next_iterator);
        std::advance(current_iterator, 1);
      }

      // construct the partition

      indices.push_back(std::distance(empty_moves.begin(), current_iterator));
      start_iterator = current_iterator;
    }

    if (!indices.empty()) {
      assert(indices.back() == empty_moves.size());
      indices.pop_back();
    }

    // reconstruct the empty input

    HoistEmptyInput empty_input;
    empty_input.length = empty_moves.size();

    for (const sched::shop::Move& move : empty_moves) {
      empty_input.machines.push_back(move.orig);
    }

    // put missing machines at the end

    for (const MachineId machine : machines(instance)) {
      if (!std::ranges::contains(empty_input.machines, machine)) {
        empty_input.machines.push_back(machine);
      }
    }

    assert(empty_input.machines.size() == instance.machine_count());

    // find the float index corresponding to partition

    Partition partition(empty_input.length, indices);
    const PartitionGroup partition_group(empty_input.length);
    const std::size_t index = partition_group.find_partition(partition);
    assert(index < partition_group.size());

    empty_input.float_index = (static_cast<double>(index) + 0.5) / static_cast<double>(partition_group.size());

    return empty_input;
  }

  HoistLoadedInput to_loaded_input(const HoistEmptyInput& empty_input, const HoistSchedulingInstance& instance)
  {
    HoistLoadedInput loaded_input = {};



    return loaded_input;
  }

}
