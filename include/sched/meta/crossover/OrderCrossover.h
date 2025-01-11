// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_ORDER_CROSSOVER_H
#define SCHED_ORDER_CROSSOVER_H

#include <cassert>

#include <algorithm>
#include <array>
#include <tuple>
#include <type_traits>

#include <sched/support/Random.h>
#include <sched/support/Log.h>

namespace sched {

  template<std::size_t N>
  struct OrderCrossover {

    template<typename Input>
    std::tuple<Input, Input> operator()(const Input& parent0, const Input& parent1, Random& random)
    {
      const std::size_t size = parent0.size();
      assert(parent1.size() == size);
      assert(size >= N);

      std::array<std::size_t, N> points;

      auto already_present = [&points](std::size_t element, std::size_t size) {
        for (std::size_t i = 0; i < size; ++i) {
          if (points[i] == element) {
            return true;
          }
        }

        return false;
      };

      for (std::size_t i = 0; i < points.size(); ++i) {
        do { // NOLINT(cppcoreguidelines-avoid-do-while)
          points[i] = random.compute_uniform_integer(std::size_t(0), size - 1);
        } while (already_present(points[i], i));
      }

      std::sort(points.begin(), points.end());

      Input child0 = parent0;
      Input child1 = parent1;

      using Element = std::remove_cv_t<std::remove_reference_t<decltype(parent0[0])>>;
      std::vector<Element> missing0;
      std::vector<Element> missing1;

      {
        bool swap = false;
        std::size_t k = 0;

        for (std::size_t i = 0; i < size; ++i) {
          if (swap) {
            std::swap(child0[i], child1[i]);
          } else {
            missing0.push_back(child0[i]);
            missing1.push_back(child1[i]);
          }

          if (k < points.size() && i == points[k]) {
            swap = !swap;
            ++k;
          }
        }
      }

      auto index_of = [](Element element, const Input& input) {
        if (auto iterator = std::find(input.begin(), input.end(), element); iterator != input.end()) {
          return static_cast<std::size_t>(std::distance(input.begin(), iterator));
        }

        assert(false && "are you sure your input is a permutation?");
        return input.size();
      };

      std::sort(missing0.begin(), missing0.end(), [&](Element lhs, Element rhs) {
        return index_of(lhs, parent1) < index_of(rhs, parent1);
      });

      std::sort(missing1.begin(), missing1.end(), [&](Element lhs, Element rhs) {
        return index_of(lhs, parent0) < index_of(rhs, parent0);
      });

      {
        bool swap = false;
        std::size_t j = 0;
        std::size_t k = 0;

        for (std::size_t i = 0; i < size; ++i) {
          if (!swap) {
            child0[i] = missing1[j];
            child1[i] = missing0[j];
            ++j;
          }

          if (k < points.size() && i == points[k]) {
            swap = !swap;
            ++k;
          }
        }
      }

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

#endif // SCHED_ORDER_CROSSOVER_H
