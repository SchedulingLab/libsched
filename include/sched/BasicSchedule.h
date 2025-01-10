#ifndef SCHED_BASIC_SCHEDULE_H
#define SCHED_BASIC_SCHEDULE_H

#include <ranges>
#include <vector>

namespace sched {

  template<typename Task>
  class BasicSchedule {
  public:
    void append(const Task& task)
    {
      m_tasks.push_back(task);
    }

    using TaskRange = std::ranges::ref_view<std::vector<Task>>;

    TaskRange tasks()
    {
      return std::views::all(m_tasks);
    }

    using ConstTaskRange = std::ranges::ref_view<const std::vector<Task>>;

    ConstTaskRange tasks() const
    {
      return std::views::all(m_tasks);
    }

    std::size_t task_count() const
    {
      return m_tasks.size();
    }

  private:
    std::vector<Task> m_tasks;
  };

}

#endif // SCHED_BASIC_SCHEDULE_H
