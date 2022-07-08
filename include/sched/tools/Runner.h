#ifndef SCHED_TOOLS_RUNNER_H
#define SCHED_TOOLS_RUNNER_H

#include <atomic>
#include <functional>
#include <thread>
#include <vector>

#include "ConcurrentQueue.h"
#include "Log.h"

namespace sched {

  template<typename Work>
  class Runner {
  public:
    using Executor = std::function<void(const Work&)>;

    Runner(std::vector<Work> works)
    : m_works(std::move(works))
    {
    }

    void run(Executor executor) const {
      int concurrency = std::thread::hardware_concurrency() - 1;

      if (concurrency > 1) {
        Log::println("Running in parallel using {} threads...", concurrency);

        ConcurrentQueue<Work> input;

        for (auto & work : m_works) {
          input.push(work);
        }

        std::vector<std::thread> threads;
        std::atomic_size_t index = 1;

        for (int i = 0; i < concurrency; ++i) {
          threads.emplace_back([&]() {
            for (;;) {
              auto maybe_work = input.pop();

              if (!maybe_work) {
                return;
              }

              auto work = *maybe_work;

              Log::println("[{}/{}] {}", index++, m_works.size(), work);
              executor(work);
            }
          });
        }

        input.close();

        for (auto & thread : threads) {
          thread.join();
        }
      } else {
        sched::Log::println("Running sequentially...");
        int index = 1;

        for (auto & work : m_works) {
          sched::Log::println("[{}/{}] {}", index++, m_works.size(), work);
          executor(work);
        }
      }
    }

  private:
    std::vector<Work> m_works;
  };

}

#endif // SCHED_TOOLS_RUNNER_H