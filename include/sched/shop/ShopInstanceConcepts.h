#ifndef SCHED_SHOP_INSTANCE_CONCEPTS_H
#define SCHED_SHOP_INSTANCE_CONCEPTS_H

#include <cstddef>

#include <concepts>
#include <vector>

#include <sched/common/Ids.h>
#include <sched/common/InstanceConcepts.h>
#include <sched/common/Time.h>

namespace sched::shop {

  template<typename I>
  concept ShopInstance = Instance<I> && requires(I instance, JobId job, OperationId operation, MachineId machine)  {
    I::Flexible;
    { instance.operation_count(job) } -> std::convertible_to<std::size_t>;

    (I::Flexible && requires(I instance, OperationId operation) {
      { instance.machines_for_operation(operation) } -> std::same_as<std::vector<MachineId>>;
    }) || requires(I instance, OperationId operation, MachineId machine) {
      { instance.assigned_machine_for_operation(operation) } -> std::same_as<MachineId>;
    };

    { instance.processing_time(operation, machine) } -> std::same_as<Time>;

    { instance.release_date(job) } -> std::same_as<Time>;
    { instance.due_date(job) } -> std::same_as<Time>;
  };


  template<typename I>
  concept ShopTransportInstance = ShopInstance<I> && requires(I instance, MachineId origin, MachineId target) {
    { instance.transportation_count() } -> std::convertible_to<std::size_t>;
    { instance.transportation_time_empty(origin, target) } -> std::same_as<Time>;
    { instance.transportation_time_loaded(origin, target) } -> std::same_as<Time>;
  };

}

#endif // SCHED_SHOP_INSTANCE_CONCEPTS_H
