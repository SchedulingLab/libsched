#ifndef SCHED_SHOP_BASIC_INSTANCE_H
#define SCHED_SHOP_BASIC_INSTANCE_H

#include <vector>

#include <sched/common/Api.h>
#include <sched/common/Ids.h>
#include <sched/common/Time.h>

namespace sched::shop {

  struct SCHED_API BasicInstance {

    constexpr std::size_t machine_count() const noexcept {
      return 0;
    }

    constexpr std::size_t job_count() const noexcept {
      return 0;
    }

    constexpr std::size_t operation_count([[maybe_unused]] JobId job) const noexcept {
      return 0;
    }

    constexpr Time release_date([[maybe_unused]] JobId job) const noexcept {
      return 0;
    }

    constexpr Time due_date([[maybe_unused]] JobId job) const noexcept {
      return TimeMax;
    }

    std::vector<MachineId> machines_for_operation([[maybe_unused]] OperationId op) const {
      return { };
    }

    constexpr bool has_assignment() const noexcept {
      return false;
    }

    constexpr MachineId assigned_machine_for_operation([[maybe_unused]] OperationId op) const noexcept {
      return NoMachine;
    }

    constexpr Time processing_time([[maybe_unused]] OperationId op, [[maybe_unused]] MachineId machine) const noexcept {
      return 0;
    }

    constexpr std::size_t transport_count() const noexcept {
      return 0;
    }

    constexpr Time transport_time_empty([[maybe_unused]] MachineId from, [[maybe_unused]] MachineId to) const noexcept {
      return 0;
    }

    constexpr Time transport_time_loaded([[maybe_unused]] MachineId from, [[maybe_unused]] MachineId to) const noexcept {
      return 0;
    }

  };

}


#endif // SCHED_SHOP_BASIC_INSTANCE_H
