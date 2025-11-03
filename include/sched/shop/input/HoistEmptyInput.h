// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_HOIST_EMPTY_INPUT_H
#define SCHED_SHOP_HOIST_EMPTY_INPUT_H

#include <cassert>

#include <algorithm>
#include <string>
#include <vector>

#include <sched/Ids.h>
#include <sched/meta/Instance.h>
#include <sched/shop/helper/Partition.h>
#include <sched/support/Random.h>
#include <sched/types/InputTraits.h>

namespace sched {

  namespace shop {

    struct HoistEmptyInput {
      std::vector<MachineId> machines;
      std::size_t count = 0;
      double float_index = 0.0;

      bool empty() const
      {
        return machines.empty();
      }

      std::size_t size() const
      {
        return machines.size();
      }

      const MachineId& operator[](std::size_t index) const
      {
        assert(index < machines.size());
        return machines[index];
      }

      MachineId& operator[](std::size_t index)
      {
        assert(index < machines.size());
        return machines[index];
      }

      bool operator==(const HoistEmptyInput& other) const
      {
        return other.machines == machines && other.count == count && reference_same_partition(other.float_index, float_index, count);
      }

    };

  }

  template<>
  struct InputTraits<shop::HoistEmptyInput> {
    static std::string name()
    {
      return "mpt";
    }

    template<typename Instance>
    static shop::HoistEmptyInput generate_input(const Instance& instance)
    {
      shop::HoistEmptyInput input;

      for (const MachineId machine : machines(instance)) {
        input.machines.push_back(machine);
      }

      return input;
    }

    template<typename Instance>
    static shop::HoistEmptyInput generate_random(const Instance& instance, Random& random)
    {
      shop::HoistEmptyInput input = generate_input(instance);
      std::ranges::shuffle(input.machines, random);
      input.count = random.compute_uniform_integer<std::size_t>(3, input.machines.size() - 1);
      return input;
    }

    template<typename Instance>
    static shop::HoistEmptyInput generate_feasible(const Instance& instance, Random& random)
    {
      return generate_random(instance, random);
    }
  };

}

#endif // SCHED_SHOP_HOIST_EMPTY_INPUT_H
