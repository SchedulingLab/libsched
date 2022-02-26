#ifndef SCHED_SHOP_OPERATION_LISY_IMPROVED_NEIGHBORHOOD_H
#define SCHED_SHOP_OPERATION_LISY_IMPROVED_NEIGHBORHOOD_H

#include "OperationListInput.h"

#include <sched/common/Api.h>

namespace sched::shop {

  struct SCHED_API OperationSwapNeighborhood {

    OperationListInput operator()(const OperationListInput& input, Random& random);
    bool are_neighbors(const OperationListInput& input0, const OperationListInput& input1);

  };

  struct SCHED_API OperationInsertNeighborhood {

    OperationListInput operator()(const OperationListInput& input, Random& random);
    bool are_neighbors(const OperationListInput& input0, const OperationListInput& input1);

  };

  struct SCHED_API OperationReverseNeighborhood {

    OperationListInput operator()(const OperationListInput& input, Random& random);
    bool are_neighbors(const OperationListInput& input0, const OperationListInput& input1);

  };

}


#endif // SCHED_SHOP_OPERATION_LISY_IMPROVED_NEIGHBORHOOD_H
