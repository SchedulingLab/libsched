// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_FLOAT_LIST_INPUT_H
#define SCHED_FLOAT_LIST_INPUT_H

#include <string>
#include <vector>

#include <sched/meta/Instance.h>
#include <sched/meta/input/InputSize.h>
#include <sched/support/Random.h>
#include <sched/types/InputTraits.h>

namespace sched {

  using FloatListInput = std::vector<double>;

  template<>
  struct InputTraits<FloatListInput> {
    static std::string name()
    {
      return "flt";
    }

    template<typename Instance>
    static FloatListInput generate_input(const Instance& instance)
    {
      return { input_size_for(instance), 0.0 };
    }

    template<typename Instance>
    static FloatListInput generate_random(const Instance& instance, Random& random)
    {
      FloatListInput input = generate_input(instance);

      std::uniform_real_distribution<double> dist(0.0, 1.0);

      for (auto& item : input) {
        item = dist(random);
      }

      return input;
    }

    template<typename Instance>
    static FloatListInput generate_feasible(const Instance& instance, Random& random)
    {
      return generate_random(instance, random);
    }
  };
}

#endif // SCHED_FLOAT_LIST_INPUT_H
