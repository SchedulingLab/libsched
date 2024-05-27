#ifndef SCHED_ENGINE_CONCEPTS_H
#define SCHED_ENGINE_CONCEPTS_H

#include <concepts>
#include <optional>

namespace sched::concepts {

  template<typename E, typename Instance>
  concept EngineFor = requires(E engine, Instance instance, E::Input input) {
    typename E::Input;
    typename E::Schedule;

    { engine(instance, input) } -> std::same_as<std::optional<typename E::Schedule>>;
  };

}

#endif // SCHED_ENGINE_CONCEPTS_H
