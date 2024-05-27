#ifndef SCHED_INSTANCE_CONCEPT_H
#define SCHED_INSTANCE_CONCEPT_H

#include <cstddef>

#include <concepts>

namespace sched {

  template<typename I>
  concept Instance = std::semiregular<I> && requires(I instance) {
    { instance.machine_count() } -> std::convertible_to<std::size_t>;
    { instance.job_count() } -> std::convertible_to<std::size_t>;
  };

}

#endif // SCHED_INSTANCE_CONCEPT_H
