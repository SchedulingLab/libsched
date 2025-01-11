// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_INPUT_CONCEPT_H
#define SCHED_INPUT_CONCEPT_H

#include <cstddef>

#include <concepts>
#include <string>
#include <type_traits>

#include <sched/support/Random.h>
#include <sched/types/InputTraits.h>

namespace sched::concepts {

  namespace details {

    template<typename T>
    concept ReferenceToRegular = std::is_reference_v<T> && std::regular<std::remove_reference_t<T>>;

  }

  template<typename I>
  concept Input = std::regular<I> && requires(I input, std::size_t index) {
    { input.empty() } -> std::convertible_to<bool>;
    { input.size() } -> std::convertible_to<std::size_t>;
    { input[index] } -> details::ReferenceToRegular<>;
  };

  namespace details {

    template<typename T>
    concept ReferenceToInput = std::is_reference_v<T> && Input<std::remove_reference_t<T>>;

  }

  template<typename I>
  concept InputArray = std::regular<I> && requires(I input, std::size_t index) {
    { input[index] } -> details::ReferenceToInput<>;
  };

  template<typename I, typename Instance>
  concept InputFor = (Input<I> || InputArray<I>) && requires(I input, Instance instance, Random& random) {
    { InputTraits<I>::name() } -> std::same_as<std::string>;
    { InputTraits<I>::generate_input(instance) } -> std::same_as<I>;
    { InputTraits<I>::generate_random(instance, random) } -> std::same_as<I>;
    { InputTraits<I>::generate_feasible(instance, random) } -> std::same_as<I>;
  };

}

#endif // SCHED_INPUT_CONCEPT_H
