#ifndef SCHED_NEIGHBORHOOD_CONCEPTS_H
#define SCHED_NEIGHBORHOOD_CONCEPTS_H

#include <string>
#include <vector>

#include <sched/support/Random.h>
#include <sched/types/NeighborhoodTraits.h>

namespace sched::concepts {

  namespace details {

    struct AnySchedule {
    };

  }

  template<typename N, typename Input, typename Schedule = details::AnySchedule>
  concept NeighborhoodFor = requires(N neighborhood, Input input, Schedule schedule, Random& random, std::size_t count) {
    { neighborhood(input, schedule, random) } -> std::same_as<Input>;
    { neighborhood.generate_many(input, schedule, random, count) } -> std::same_as<std::vector<Input>>;
    { NeighborhoodTraits<N>::name() } -> std::same_as<std::string>;
  };

}

#endif // SCHED_NEIGHBORHOOD_CONCEPTS_H
