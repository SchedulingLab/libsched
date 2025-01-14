// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_RANDOM_SAMPLE_H
#define SCHED_RANDOM_SAMPLE_H

#include <cstdint>

#include <vector>

#include <sched/support/Random.h>
#include <sched/types/InputTraits.h>

namespace sched {

  enum class RandomSamplePolicy : uint8_t {
    Uniform,
    Feasible,
  };

  template<typename Engine, RandomSamplePolicy Policy = RandomSamplePolicy::Uniform>
  struct RandomSample {
    using Input = typename Engine::Input;

    template<typename Instance>
    std::vector<Input> operator()(const Instance& instance, Random& random, std::size_t count = 1000)
    {
      std::vector<Input> result;

      while (result.size() < count) {
        Input input;

        if constexpr (Policy == RandomSamplePolicy::Uniform) {
          input = InputTraits<Input>::generate_random(instance, random);
        } else {
          static_assert(Policy == RandomSamplePolicy::Feasible);
          input = InputTraits<Input>::generate_feasible(instance, random);
        }

        if (engine(instance, input)) {
          result.push_back(std::move(input));
        }
      }

      return result;
    }

    Engine engine;
  };

}

#endif // SCHED_RANDOM_SAMPLE_H
