// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_HOIST_LOADED_INPUT_H
#define SCHED_SHOP_HOIST_LOADED_INPUT_H

#include <algorithm>
#include <string>
#include <vector>

#include <sched/Ids.h>
#include <sched/meta/Instance.h>
#include <sched/shop/Hoist.h>
#include <sched/support/Random.h>
#include <sched/types/InputTraits.h>

namespace sched {

  namespace shop {

    using HoistLoadedInput = std::vector<MachineId>;

    std::vector<Move> compute_empty_moves(const HoistLoadedInput& input, MoveFilter filter = MoveFilter::All);

  }

  template<>
  struct InputTraits<shop::HoistLoadedInput> {
    static std::string name()
    {
      return "ldd";
    }

    template<typename Instance>
    static shop::HoistLoadedInput generate_input(const Instance& instance)
    {
      shop::HoistLoadedInput input;

      for (const MachineId machine : machines(instance)) {
        input.push_back(machine);
      }

      return input;
    }

    template<typename Instance>
    static shop::HoistLoadedInput generate_random(const Instance& instance, Random& random)
    {
      shop::HoistLoadedInput input = generate_input(instance);
      std::ranges::shuffle(input.begin() + 1, input.end(), random); // keep 0_m at the beginning
      return input;
    }

    template<typename Instance>
    static shop::HoistLoadedInput generate_feasible(const Instance& instance, Random& random)
    {
      return generate_random(instance, random);
    }
  };

}

#endif // SCHED_SHOP_HOIST_LOADED_INPUT_H
