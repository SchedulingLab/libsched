#ifndef SCHED_SHOP_SOLUTION_H
#define SCHED_SHOP_SOLUTION_H

#include <optional>
#include <utility>

#include <sched/types/EngineConcepts.h>

namespace sched::shop {

  template<typename Engine, typename Criterion, typename Instance>
    requires(concepts::EngineFor<Engine, Instance>)
  struct Solution {
    using Input = typename Engine::Input;
    using Schedule = std::optional<typename Engine::Schedule>;
    using Fitness = decltype(std::declval<Criterion>()(std::declval<Instance>(), *std::declval<Schedule>()));

    Input input;
    Schedule schedule;
    Fitness fitness;
  };

}

#endif // SCHED_SHOP_SOLUTION_H
