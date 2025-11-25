// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/input/HoistEmptyInput.h>

#include <algorithm>

#include <sched/types/InputConcepts.h>

static_assert(sched::concepts::Input<sched::shop::HoistEmptyInput>);

namespace sched::shop {

  std::vector<Move> generate_valid_empty_moves(std::size_t machine_count, Random& random)
  {
    enum class TankStatus : uint8_t {
      Available,
      AlreadyUnloaded,
    };

    std::vector<TankStatus> tanks(machine_count, TankStatus::Available);

    // initialize

    tanks[0] = TankStatus::AlreadyUnloaded;
    MachineId current = machine(1);

    std::vector<Move> empty_moves;

    for (;;) {
      // choose the next tank among available tanks

      std::vector<MachineId> available;

      for (std::size_t i = 0; i < machine_count; ++i) {
        if (tanks[i] == TankStatus::Available) {
          available.push_back(machine(i));
        }
      }

      if (available.empty()) {
        using namespace sched::literals;

        if (current != 0_m) {
          empty_moves.push_back({ .orig = current, .dest = 0_m });
        }

        break;
      }

      const auto index_next = random.compute_uniform_integer<std::size_t>(0, available.size() - 1);
      const MachineId next = available[index_next];
      tanks[to_index(next)] = TankStatus::AlreadyUnloaded;

      // add an empty move if not trivial

      if (next != current) {
        empty_moves.push_back({ .orig = current, .dest = next });
      }

      // select the new tank

      current = machine((to_index(next) + 1) % machine_count);
    }

    return empty_moves;
  }

  HoistEmptyInput convert_empty_moves_to_empty_input(std::vector<Move> empty_moves, std::size_t machine_count)
  {
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

    for (const MachineId machine : MachineRange{ machine_count }) {
      if (!std::ranges::contains(empty_input.machines, machine)) {
        empty_input.machines.push_back(machine);
      }
    }

    assert(empty_input.machines.size() == machine_count);

    // find the float index corresponding to partition

    Partition partition(empty_input.length, indices);
    const PartitionGroup partition_group(empty_input.length);
    const std::size_t index = partition_group.find_partition(partition);
    assert(index < partition_group.size());

    empty_input.float_index = (static_cast<double>(index) + 0.5) / static_cast<double>(partition_group.size());

    return empty_input;
  }



}
