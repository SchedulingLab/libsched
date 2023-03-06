#ifndef SCHED_SCRAMBLE_NEIHGBORHOOD_H
#define SCHED_SCRAMBLE_NEIHGBORHOOD_H

#include <string>

#include <sched/common/Api.h>
#include <sched/common/Random.h>

#include "MutationNeighborhood.h"
#include "NeighborhoodHelper.h"
#include "NeighborhoodTraits.h"
#include "ScrambleMutation.h"

namespace sched::shop {

  struct SCHED_API ScrambleNeighborhood : MutationNeighborhood<ScrambleMutation> {

    template<typename Input, typename Schedule>
    std::vector<Input> generate_many(const Input& input, const Schedule& schedule, Random& random, std::size_t count) {
      return NeighborhoodHelper::generate_many(*this, input, schedule, random, count);
    }

  };

  template<>
  struct NeighborhoodTraits<ScrambleNeighborhood> {
    static std::string name() {
      return MutationTraits<ScrambleMutation>::name();
    }
  };

}

#endif // SCHED_SCRAMBLE_NEIHGBORHOOD_H
