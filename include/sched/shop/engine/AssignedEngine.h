// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_ASSIGNED_ENGINE_H
#define SCHED_SHOP_ASSIGNED_ENGINE_H

#include <optional>

#include <sched/shop/instance/AssignedInstance.h>
#include <sched/meta/input/SplitInput.h>
#include <sched/types/AssignmentTraits.h>
#include <sched/types/EngineTraits.h>

namespace sched::shop {

  template<typename Assignment, typename Engine>
  struct AssignedEngine {
    using Input = SplitInput<typename Assignment::Input, typename Engine::Input>;
    using Schedule = typename Engine::Schedule;

    template<typename Instance>
    std::optional<Schedule> operator()(const Instance& instance, const Input& input)
    {
      AssignedInstance assigned_instance(instance);
      assigned_instance.set_assignment(assignment(instance, input.input0));
      return engine(assigned_instance, input.input1);
    }

    Assignment assignment;
    Engine engine;
  };

}

namespace sched {

  template<typename Assignment, typename Engine>
  struct EngineTraits<shop::AssignedEngine<Assignment, Engine>> {
    static std::string name()
    {
      return AssignmentTraits<Assignment>::name() + '_' + EngineTraits<Engine>::name();
    }
  };

}

#endif // SCHED_SHOP_ASSIGNED_ENGINE_H
