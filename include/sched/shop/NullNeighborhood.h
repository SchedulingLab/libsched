#ifndef SCHED_NULL_NEIGHBORHOOD_H
#define SCHED_NULL_NEIGHBORHOOD_H

#include <sched/common/Api.h>

namespace sched::shop {

  struct SCHED_API NullNeighborhood {

    template<typename Input>
    Input operator()(const Input& input) {
      return input;
    }

    template<typename Input>
    bool are_neighbors(const Input& input0, const Input& input1) {
      return false;
    }

  };

}

#endif // SCHED_NULL_NEIGHBORHOOD_H
