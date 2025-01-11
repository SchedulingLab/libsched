// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_RANDOM_H
#define SCHED_RANDOM_H

#include <cstdint>

#include <limits>
#include <random>

#include <sched/Api.h>

namespace sched {

  // xoshiro256++ 1.0
  class SCHED_API Random {
  public:
    using result_type = uint64_t;

    Random(result_type seed);

    static constexpr result_type min()
    {
      return std::numeric_limits<result_type>::min();
    }

    static constexpr result_type max()
    {
      return std::numeric_limits<result_type>::max();
    }

    result_type operator()();

    void jump();
    void long_jump();

    template<typename Integer>
    Integer compute_uniform_integer(Integer a, Integer b)
    {
      std::uniform_int_distribution<Integer> dist(a, b);
      return dist(*this);
    }

  private:
    uint64_t next();

    uint64_t m_state[4];
  };

}

#endif // SCHED_RANDOM_H
