// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/input/HoistInputConversion.h>

#include <cassert>

#include <algorithm>

#include <sched/shop/helper/Partition.h>

namespace sched::shop {

  namespace {

    MachineId next_machine(MachineId machine, std::size_t machine_count)
    {
      return static_cast<MachineId>((static_cast<std::size_t>(machine) + 1) % machine_count);
    }

  }

  HoistEmptyInput to_empty_input(const HoistLoadedInput& loaded_input, std::size_t machine_count)
  {
    assert(loaded_input.size() == machine_count);
    std::vector<Move> empty_moves = compute_empty_moves(loaded_input, MoveFilter::RemoveTrivial);
    return convert_empty_moves_to_empty_input(std::move(empty_moves), machine_count);
  }

  std::optional<HoistLoadedInput> to_loaded_input(const HoistEmptyInput& empty_input, std::size_t machine_count)
  {
    const PartitionGroup group(empty_input.length);
    const Partition& partition = group.partition(empty_input.float_index);
    const auto end = std::next(empty_input.machines.begin(), static_cast<std::ptrdiff_t>(empty_input.length));

    HoistLoadedInput loaded_input = {};
    MachineId current = machine(0);

    for (;;) {
      loaded_input.push_back(current);
      current = next_machine(current, machine_count); // target of the loaded move

      auto iterator = std::ranges::find(empty_input.machines.begin(), end, current);

      if (iterator != end) {
        const std::size_t index = std::distance(empty_input.machines.begin(), iterator);
        const std::size_t next_index = partition.next_index(index);
        assert(next_index < empty_input.length);
        current = empty_input.machines[next_index];
      }

      if (current == machine(0)) {
        break;
      }
    }

    if (loaded_input.size() != machine_count) {
      // the empty input was not a valid sequence of empty moves
      return std::nullopt;
    }

    return loaded_input;
  }

}
