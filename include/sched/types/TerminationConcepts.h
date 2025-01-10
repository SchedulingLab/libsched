#ifndef SCHED_TERMINTATION_CONCEPTS_H
#define SCHED_TERMINTATION_CONCEPTS_H

#include <concepts>

namespace sched::concepts {

  template<typename T>
  concept Termination = std::semiregular<T> && requires(T termination) {
    { termination.start() } -> std::same_as<void>;
    { termination.step() } -> std::same_as<void>;
    { termination.satisfied() } -> std::convertible_to<bool>;
  };

}

#endif // SCHED_TERMINTATION_CONCEPTS_H
