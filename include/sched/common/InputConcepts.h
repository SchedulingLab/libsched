#ifndef SCHED_INPUT_CONCEPT_H
#define SCHED_INPUT_CONCEPT_H

#include <cstddef>

#include <concepts>
#include <type_traits>

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
  concept InputArrayConcept = std::regular<I> && requires(I input, std::size_t index) {
    { input[index] } -> details::ReferenceToInput<>;
  };

}

#endif // SCHED_INPUT_CONCEPT_H
