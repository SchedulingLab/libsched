#ifndef SCHED_ENGINE_CONCEPTS_H
#define SCHED_ENGINE_CONCEPTS_H

#include <concepts>
#include <optional>
// #include <string>

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
    // { E::name() } -> std::same_as<std::string>;
  };

}

#endif // SCHED_ENGINE_CONCEPTS_H
