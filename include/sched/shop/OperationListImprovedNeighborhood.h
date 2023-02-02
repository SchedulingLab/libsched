#ifndef SCHED_SHOP_OPERATION_LISY_IMPROVED_NEIGHBORHOOD_H
#define SCHED_SHOP_OPERATION_LISY_IMPROVED_NEIGHBORHOOD_H

#include <string>

#include <sched/common/Api.h>

#include "NeighborhoodHelper.h"
#include "NeighborhoodTraits.h"
#include "OperationListInput.h"

namespace sched::shop {

  struct SCHED_API OperationSwapNeighborhood {

    template<typename Schedule>
    OperationListInput operator()(const OperationListInput& input, [[maybe_unused]] const Schedule& schedule, Random& random) {
      return compute(input, random);
    }

    template<typename Schedule>
    std::vector<OperationListInput> generate_many(const OperationListInput& input, const Schedule& schedule, Random& random, std::size_t count) {
      return NeighborhoodHelper::generate_many(*this, input, schedule, random, count);
    }

    bool are_neighbors(const OperationListInput& input0, const OperationListInput& input1);

  private:
    OperationListInput compute(const OperationListInput& input, Random& random);
  };

  template<>
  struct NeighborhoodTraits<OperationSwapNeighborhood> {
    static std::string name() {
      return "swp";
    }
  };

  struct SCHED_API OperationInsertNeighborhood {

    template<typename Schedule>
    OperationListInput operator()(const OperationListInput& input, [[maybe_unused]] const Schedule& schedule, Random& random) {
      return compute(input, random);
    }

    template<typename Schedule>
    std::vector<OperationListInput> generate_many(const OperationListInput& input, const Schedule& schedule, Random& random, std::size_t count) {
      return NeighborhoodHelper::generate_many(*this, input, schedule, random, count);
    }

    bool are_neighbors(const OperationListInput& input0, const OperationListInput& input1);

  private:
    OperationListInput compute(const OperationListInput& input, Random& random);
  };

  template<>
  struct NeighborhoodTraits<OperationInsertNeighborhood> {
    static std::string name() {
      return "ins";
    }
  };

  struct SCHED_API OperationReverseNeighborhood {

    template<typename Schedule>
    OperationListInput operator()(const OperationListInput& input, [[maybe_unused]] const Schedule& schedule, Random& random) {
      return compute(input, random);
    }

    template<typename Schedule>
    std::vector<OperationListInput> generate_many(const OperationListInput& input, const Schedule& schedule, Random& random, std::size_t count) {
      return NeighborhoodHelper::generate_many(*this, input, schedule, random, count);
    }

    bool are_neighbors(const OperationListInput& input0, const OperationListInput& input1);

  private:
    OperationListInput compute(const OperationListInput& input, Random& random);
  };

  template<>
  struct NeighborhoodTraits<OperationReverseNeighborhood> {
    static std::string name() {
      return "rev";
    }
  };

}


#endif // SCHED_SHOP_OPERATION_LISY_IMPROVED_NEIGHBORHOOD_H
