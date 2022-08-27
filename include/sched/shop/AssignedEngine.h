#ifndef SCHED_SHOP_ASSIGNED_ENGINE_H
#define SCHED_SHOP_ASSIGNED_ENGINE_H

#include "AssignedInstance.h"
#include "SplitInput.h"

namespace sched::shop {

  template<typename Assignment, typename Engine>
  struct AssignedEngine {
    using Input = FlexibleSplitInput<typename Assignment::Input, typename Engine::Input>;

    template<typename Instance>
    auto operator()(const Instance& instance, const Input& input) {
      AssignedInstance assigned_instance(instance);

      Assignment assignment;
      assigned_instance.set_assignment(assignment(instance, input.assignement));

      Engine engine;
      return engine(assigned_instance, input.schedule);
    }

  };

}

#endif // SCHED_SHOP_ASSIGNED_ENGINE_H
