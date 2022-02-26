#ifndef SCHED_GENERIC_SCHEDULE_H
#define SCHED_GENERIC_SCHEDULE_H

#include <vector>

namespace sched {

  template<typename Task>
  class GenericSchedule {
  public:
    void append(const Task& task) {
      m_tasks.push_back(task);
    }

    auto begin() { return m_tasks.begin(); }
    auto begin() const { return m_tasks.begin(); }
    auto end() { return m_tasks.end(); }
    auto end() const { return m_tasks.end(); }

    bool operator<(const GenericSchedule& other) {
      return m_tasks < other.m_tasks;
    }

  private:
    std::vector<Task> m_tasks;
  };

}


#endif // SCHED_GENERIC_SCHEDULE_H
