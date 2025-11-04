// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/input/HoistLoadedInput.h>

#include <sched/types/InputConcepts.h>

static_assert(sched::concepts::Input<sched::shop::HoistLoadedInput>);


namespace sched::shop {

  std::vector<Move> compute_empty_moves(const HoistLoadedInput& input)
  {
    std::vector<Move> moves;

    for (std::size_t i = 0; i < input.size(); ++i) {
      Move move = {};
      move.orig = machine(((input[i] + 1)) % input.size());
      move.dest = input[(i + 1) % input.size()];
      moves.push_back(move);
    }

    return moves;
  }

}

