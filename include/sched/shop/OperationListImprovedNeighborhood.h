#ifndef SCHED_SHOP_OPERATION_LISY_IMPROVED_NEIGHBORHOOD_H
#define SCHED_SHOP_OPERATION_LISY_IMPROVED_NEIGHBORHOOD_H

#include "OperationListInput.h"

#include <sched/common/Api.h>

namespace sched::shop {

  struct SCHED_API OperationSwapNeighborhood {

    template<typename Schedule>
    OperationListInput operator()(const OperationListInput& input, [[maybe_unused]] const Schedule& schedule, Random& random) {
      return compute(input, random);
    }

    bool are_neighbors(const OperationListInput& input0, const OperationListInput& input1);

  private:
    OperationListInput compute(const OperationListInput& input, Random& random);
  };

  struct SCHED_API OperationInsertNeighborhood {

    template<typename Schedule>
    OperationListInput operator()(const OperationListInput& input, [[maybe_unused]] const Schedule& schedule, Random& random) {
      return compute(input, random);
    }

    bool are_neighbors(const OperationListInput& input0, const OperationListInput& input1);

  private:
    OperationListInput compute(const OperationListInput& input, Random& random);
  };

  struct SCHED_API OperationReverseNeighborhood {

    template<typename Schedule>
    OperationListInput operator()(const OperationListInput& input, [[maybe_unused]] const Schedule& schedule, Random& random) {
      return compute(input, random);
    }

    bool are_neighbors(const OperationListInput& input0, const OperationListInput& input1);

  private:
    OperationListInput compute(const OperationListInput& input, Random& random);
  };

}


#endif // SCHED_SHOP_OPERATION_LISY_IMPROVED_NEIGHBORHOOD_H
