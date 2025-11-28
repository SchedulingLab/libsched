// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_CROSSOVER_HELPERS_H
#define SCHED_CROSSOVER_HELPERS_H

#include <cassert>

#include <array>
#include <algorithm>

#include <sched/support/Random.h>

namespace sched {

  template<std::size_t N>
  std::array<std::size_t, N> choose_points(std::size_t size, Random& random)
  {
    assert(size > N);
    std::vector<std::size_t> possible_points(size - 1);
    std::ranges::iota(possible_points, 1);
    std::ranges::shuffle(possible_points, random);

    std::array<std::size_t, N> points;
    std::copy_n(possible_points.begin(), N, points.begin());
    std::ranges::sort(points);

    return points;
  }

  namespace details {

    template<std::size_t N, typename Func0, typename Func1>
    void handle_elements(std::size_t size, const std::array<std::size_t, N>& points, Func0 func0, Func1 func1, bool initial_toggle = false)
    {
      bool toggle = initial_toggle;
      std::size_t k = 0;

      for (std::size_t i = 0; i < size; ++i) {
        if (k < points.size() && i == points[k]) {
          toggle = !toggle;
          ++k;
        }

        if (toggle) {
          func0(i);
        } else {
          func1(i);
        }
      }
    }

  }

  template<typename Input, std::size_t N>
  void swap_elements(Input& child0, Input& child1, const std::array<std::size_t, N>& points, bool initial_toggle = false)
  {
    const std::size_t size = child0.size();
    assert(size == child1.size());

    details::handle_elements(size, points,
        [&](std::size_t i) { std::swap(child0[i], child1[i]); },
        []([[maybe_unused]] std::size_t i) { },
        initial_toggle
    );
  }

  template<typename Input, std::size_t N, typename Element>
  void compute_missing_elements(const Input& child0, const Input& child1, const std::array<std::size_t, N>& points, std::vector<Element>& missing0, std::vector<Element>& missing1, bool initial_toggle = false)
  {
    const std::size_t size = child0.size();
    assert(size == child1.size());

    details::handle_elements(size, points,
        []([[maybe_unused]] std::size_t i) { },
        [&](std::size_t i) {
          missing0.push_back(child0[i]);
          missing1.push_back(child1[i]);
        },
        initial_toggle
    );
  }

  template<typename Input, std::size_t N, typename Element>
  void assign_missing_elements(Input& child0, Input& child1, const std::array<std::size_t, N>& points, const std::vector<Element>& missing0, const std::vector<Element>& missing1, bool initial_toggle = false)
  {
    const std::size_t size = child0.size();
    assert(size == child1.size());

    std::size_t j = 0;

    details::handle_elements(size, points,
        []([[maybe_unused]] std::size_t i) { },
        [&](std::size_t i) {
          assert(j < missing1.size());
          child0[i] = missing1[j];
          assert(j < missing0.size());
          child1[i] = missing0[j];
          ++j;
        },
        initial_toggle
    );
  }

}

#endif // SCHED_CROSSOVER_HELPERS_H
