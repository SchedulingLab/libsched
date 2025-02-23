// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_VEHICLE_LIST_INPUT_H
#define SCHED_SHOP_VEHICLE_LIST_INPUT_H

#include <algorithm>
#include <vector>

#include <sched/Ids.h>
#include <sched/meta/Instance.h>
#include <sched/meta/input/InputSize.h>
#include <sched/support/Random.h>
#include <sched/types/InputTraits.h>

namespace sched {

  namespace shop {

    using VehicleListInput = std::vector<VehicleId>;

  }

  template<>
  struct InputTraits<shop::VehicleListInput> {
    static std::string name()
    {
      return "trp";
    }

    template<typename Instance>
    static shop::VehicleListInput generate_input(const Instance& instance)
    {
      const std::size_t input_size = input_extended_size_for(instance);
      const std::size_t vehicle_count = instance.vehicle_count();

      shop::VehicleListInput input;
      input.reserve(input_size);

      for (std::size_t i = 0; i < input_size; ++i) {
        input.push_back(VehicleId{ i % vehicle_count });
      }

      return input;
    }

    template<typename Instance>
    static shop::VehicleListInput generate_random(const Instance& instance, Random& random)
    {
      shop::VehicleListInput input = generate_input(instance);
      std::shuffle(input.begin(), input.end(), random);
      return input;
    }

    template<typename Instance>
    static shop::VehicleListInput generate_feasible(const Instance& instance, Random& random)
    {
      // all inputs are feasible
      return generate_random(instance, random);
    }
  };

}

#endif // SCHED_SHOP_VEHICLE_LIST_INPUT_H
