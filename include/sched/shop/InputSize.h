#ifndef SCHED_SHOP_INPUT_SIZE_H
#define SCHED_SHOP_INPUT_SIZE_H

#include <cstddef>

#include <sched/common/Instance.h>

namespace sched::shop {

  template<typename Instance>
  std::size_t input_size_for(const Instance& instance)
  {
    std::size_t size = 0;

    for (auto job : sched::jobs(instance)) {
      size += instance.operation_count(job);
    }

    return size;
  }

  template<typename Instance>
  std::size_t input_extended_size_for(const Instance& instance)
  {
    std::size_t size = 0;

    for (auto job : sched::jobs(instance)) {
      size += instance.operation_count(job);
    }

    const std::size_t transportation_count = instance.transportation_count();

    // add r * 10%
    size += transportation_count * std::max(size / 10, std::size_t(1));

    // make it a multiple of transportation_count
    if (size % transportation_count != 0) {
      size += transportation_count - (size % transportation_count);
    }

    return size;
  }

}

#endif // SCHED_SHOP_INPUT_SIZE_H
