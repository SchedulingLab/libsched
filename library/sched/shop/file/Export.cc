// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/file/Export.h>

#include <sched/support/Image.h>

namespace sched::shop {
  namespace {

    constexpr Color to_color(JobId id)
    {
      return static_cast<Color>(static_cast<int>(static_cast<std::size_t>(id)));
    }

  }

  void export_schedule(const JobShopSchedule& schedule, std::size_t machines, std::ostream& out)
  {
    static constexpr std::size_t MachineHeight = 20;
    static constexpr std::size_t TimeWidth = 1;

    Image img(machines * MachineHeight);

    for (auto task : schedule.tasks()) {
      for (std::size_t col = task.start * TimeWidth; col < task.completion * TimeWidth; ++col) {
        for (std::size_t row = 0; row < MachineHeight; ++row) {
          img.set_color((to_index(task.machine) * MachineHeight) + row, col, to_color(task.operation.job));
        }
      }
    }

    img.export_to(out);
  }

}
