// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_ENGINE_CONCEPTS_H
#define SCHED_ENGINE_CONCEPTS_H

#include <concepts>
#include <optional>
#include <string>

#include <sched/types/EngineTraits.h>

namespace sched::concepts {

  namespace details {

    template<typename E, typename Instance, typename Input, typename Schedule>
    concept BasicEngineFor = requires(E engine, Instance instance, Input input) {
      { engine(instance, input) } -> std::same_as<std::optional<Schedule>>;
    };

  }

  template<typename E, typename Instance>
  concept EngineFor = requires(E engine, Instance instance, E::Input input) {
    typename E::Input;
    typename E::Schedule;

    requires details::BasicEngineFor<E, Instance, typename E::Input, typename E::Schedule>;

    { EngineTraits<E>::name() } -> std::same_as<std::string>;
  };

}

#endif // SCHED_ENGINE_CONCEPTS_H
