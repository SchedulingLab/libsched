#ifndef SCHED_GENERIC_SCHEDULE_H
#define SCHED_GENERIC_SCHEDULE_H

#include <vector>

#include "Range.h"

namespace sched {

  template<typename Task>
  class GenericSchedule {
  public:
    void append(const Task& task) {
      m_tasks.push_back(task);
    }

    using TaskRange = IteratorRange<typename std::vector<Task>::iterator>;

    TaskRange tasks() {
      return make_iterator_range(m_tasks);
    }

    using ConstTaskRange = IteratorRange<typename std::vector<Task>::const_iterator>;

    ConstTaskRange tasks() const {
      return make_iterator_range(m_tasks);
    }

    bool operator<(const GenericSchedule& other) {
      return m_tasks < other.m_tasks;
    }

  private:
    std::vector<Task> m_tasks;
  };

}


#endif // SCHED_GENERIC_SCHEDULE_H
