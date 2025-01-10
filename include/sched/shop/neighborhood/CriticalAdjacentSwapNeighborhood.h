#ifndef SCHED_SHOP_CRITICAL_ADJACENT_SWAP_NEIGHBORHOOD_H
#define SCHED_SHOP_CRITICAL_ADJACENT_SWAP_NEIGHBORHOOD_H

#include <string>

#include <sched/Api.h>
#include <sched/shop/schedule/JobShopSchedule.h>
#include <sched/shop/input/MachineListInput.h>
#include <sched/shop/input/OperationListInput.h>
#include <sched/types/NeighborhoodTraits.h>

namespace sched {

  namespace shop {

    struct SCHED_API OperationCriticalAdjacentSwapNeighborhood {

      OperationListInput operator()(const OperationListInput& input, const JobShopSchedule& schedule, Random& random);
      std::vector<OperationListInput> generate_many(const OperationListInput& input, const JobShopSchedule& schedule, Random& random, std::size_t count);
    };

  }

  template<>
  struct NeighborhoodTraits<shop::OperationCriticalAdjacentSwapNeighborhood> {
    static std::string name()
    {
      return "cas";
    }
  };

  namespace shop {

    struct SCHED_API MachineCriticalAdjacentSwapNeighborhood {

      MachineListInput operator()(const MachineListInput& input, const JobShopSchedule& schedule, Random& random);
      std::vector<MachineListInput> generate_many(const MachineListInput& input, const JobShopSchedule& schedule, Random& random, std::size_t count);
    };

  }

  template<>
  struct NeighborhoodTraits<shop::MachineCriticalAdjacentSwapNeighborhood> {
    static std::string name()
    {
      return "cas";
    }
  };

}

#endif // SCHED_SHOP_CRITICAL_ADJACENT_SWAP_NEIGHBORHOOD_H
