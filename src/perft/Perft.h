#pragma once

#include <condition_variable>
#include <cstddef>
#include <future>
#include <queue>
#include <vector>
#include <type_traits>

namespace yak {

class ThreadPool
{
public:
  explicit ThreadPool(size_t numThreads);
  ~ThreadPool();

  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;

  template<typename Task>
  auto enqueue(Task&& task) -> std::future<typename std::invoke_result_t<Task>>;

private:
  std::vector<std::thread> m_threads;
  std::queue<std::function<void()>> m_tasks;
  std::mutex m_mutex;
  std::condition_variable m_conditionVariable;
  bool m_stop{ false };
  size_t m_numThreads{ 1 };
};

template<typename Task>
auto ThreadPool::enqueue(Task&& task) -> std::future<typename std::invoke_result_t<Task>>
{
  using ReturnType = typename std::invoke_result_t<Task>;

  auto packagedTask = std::make_shared<std::packaged_task<ReturnType()>>(std::forward<Task>(task));

  std::future<ReturnType> future = packagedTask->get_future();

  {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_tasks.emplace([packagedTask] { (*packagedTask)(); });
  }

  m_conditionVariable.notify_one();
  return future;
}

struct PerftResult {
  size_t m_total{0};
  size_t m_captures{0};
  size_t m_ep{0};
  size_t m_castles{0};
  size_t m_promotions{0};
  size_t m_checks{0};
  size_t m_checkmates{0};
};

class Board;
PerftResult perft(Board& board, int depth);
PerftResult perftHelper(Board& board, int depth, std::unique_ptr<ThreadPool> threadPool);

} // namespace yak
