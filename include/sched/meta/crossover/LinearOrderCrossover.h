// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_LINEAR_ORDER_CROSSOVER_H
#define SCHED_LINEAR_ORDER_CROSSOVER_H

#include <cassert>

#include <algorithm>
#include <array>
#include <tuple>

#include <sched/support/Random.h>
#include <sched/meta/crossover/CrossoverHelpers.h>

namespace sched {

  template<std::size_t N>
  struct LinearOrderCrossover {

    template<typename Input>
    std::tuple<Input, Input> operator()(const Input& parent0, const Input& parent1, Random& random)
    {
      const std::size_t size = parent0.size();
      assert(size == parent1.size());
      assert(size >= N);

      std::array<std::size_t, N> points = choose_points<N>(size, random);
      return compute_crossover(parent0, parent1, points);
    }

    template<typename Input>
    std::tuple<Input, Input> compute_crossover(const Input& parent0, const Input& parent1, const std::array<std::size_t, N>& points)
    {
      const std::size_t size = parent0.size();
      assert(size == parent1.size());
      assert(size >= N);

      Input child0 = parent0;
      Input child1 = parent1;

      swap_elements(child0, child1, points);

      using Element = std::remove_cvref_t<decltype(parent0[0])>;
      std::vector<Element> missing0;
      std::vector<Element> missing1;

      compute_missing_elements(child0, child1, points, missing0, missing1);

      auto index_of = [&](Element element, const Input& input) {
        if (auto iterator = std::ranges::find(input, element); iterator != input.end()) {
          return static_cast<std::size_t>(std::distance(input.begin(), iterator));
        }

        assert(false && "are you sure your input is a permutation?");
        return input.size();
      };

      std::ranges::sort(missing0, [&](Element lhs, Element rhs) {
        return index_of(lhs, parent1) < index_of(rhs, parent1);
      });

      std::ranges::sort(missing1, [&](Element lhs, Element rhs) {
        return index_of(lhs, parent0) < index_of(rhs, parent0);
      });

      assign_missing_elements(child0, child1, points, missing0, missing1); // swapped missing

      assert(child0.size() == size);
      assert(child1.size() == size);

      [[maybe_unused]] auto c0 = &child0[0];
      [[maybe_unused]] auto p0 = &parent0[0];
      [[maybe_unused]] auto c1 = &child1[0];
      [[maybe_unused]] auto p1 = &parent1[0];

      assert(std::is_permutation(c0, c0 + size, p0, p0 + size));
      assert(std::is_permutation(c1, c1 + size, p1, p1 + size));

      return std::make_tuple(child0, child1);
    }
  };

}

#endif // SCHED_LINEAR_ORDER_CROSSOVER_H
