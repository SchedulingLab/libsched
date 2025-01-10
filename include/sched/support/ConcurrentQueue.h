#ifndef SCHED_CONCURRENT_QUEUE_H
#define SCHED_CONCURRENT_QUEUE_H

#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>

namespace sched {

  template<typename T>
  class ConcurrentQueue {
  public:
    std::optional<T> pop()
    {
      std::unique_lock<std::mutex> lock(m_mutex);

      if (m_closed && m_queue.empty()) {
        return std::nullopt;
      }

      if (m_queue.empty()) {
        m_condition.wait(lock, [this]() { return !m_queue.empty(); });
      }

      T value = m_queue.front();
      m_queue.pop_front();
      return value;
    }

    void push(const T& value)
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      m_queue.push_back(value);
      m_condition.notify_one();
    }

    void push(T&& value)
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      m_queue.push_back(std::move(value));
      m_condition.notify_one();
    }

    void clear()
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      m_queue.clear();
      m_closed = false;
    }

    void close()
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      m_closed = true;
    }

  private:
    std::mutex m_mutex;
    std::condition_variable m_condition;
    std::deque<T> m_queue;
    bool m_closed = false;
  };

}

#endif // SCHED_CONCURRENT_QUEUE_H
