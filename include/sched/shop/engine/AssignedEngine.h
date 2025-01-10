#ifndef SCHED_SHOP_ASSIGNED_ENGINE_H
#define SCHED_SHOP_ASSIGNED_ENGINE_H

#include <optional>

#include <sched/shop/instance/AssignedInstance.h>
#include <sched/meta/input/SplitInput.h>

namespace sched::shop {

  template<typename Assignment, typename Engine>
  struct AssignedEngine {
    using Input = SplitInput<typename Assignment::Input, typename Engine::Input>;
    using Schedule = typename Engine::Schedule;

    template<typename Instance>
    std::optional<Schedule> operator()(const Instance& instance, const Input& input)
    {
      AssignedInstance assigned_instance(instance);
      assigned_instance.set_assignment(assignment(instance, input.assignment));
      return engine(assigned_instance, input.schedule);
    }

    Assignment assignment;
    Engine engine;
  };

}

#endif // SCHED_SHOP_ASSIGNED_ENGINE_H
