#ifndef SCHED_BASIC_NEIGHBORHOOD_H
#define SCHED_BASIC_NEIGHBORHOOD_H

#include <vector>

#include <sched/common/Api.h>
#include <sched/common/Random.h>

namespace sched::shop {

  struct SCHED_API BasicNeighborhood {

    template<typename Neighborhood, typename Input, typename Schedule>
    static std::vector<Input> generate_many(Neighborhood& neighborhood, const Input& input, const Schedule& schedule, Random& random, std::size_t count) {
      std::vector<Input> result;

      for (std::size_t i = 0; i < count; ++i) {
        result.push_back(neighborhood(input, schedule, random));
      }

      return result;
    }

  };

}

#endif // SCHED_BASIC_NEIGHBORHOOD_H
