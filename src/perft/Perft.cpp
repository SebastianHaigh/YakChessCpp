#include "Perft.h"

#include <board.h>
#include <memory>
#include <iostream>
#include <types.h>

namespace yak {

ThreadPool::ThreadPool(size_t numThreads)
{
  for (size_t i = 0; i < numThreads; ++i)
  {
    auto threadFn = [this]
    {
      while (true)
      {
        std::function<void()> task;

        {
          std::unique_lock<std::mutex> lock(this->m_mutex);

          this->m_conditionVariable.wait(lock, [this] { return (this->m_stop || not this->m_tasks.empty()); });

          if (this->m_stop && this->m_tasks.empty()) return;

          task = std::move(this->m_tasks.front());
          m_tasks.pop();
        }

        task();
      }
    };
    m_threads.emplace_back(std::move(threadFn));
  }
}

ThreadPool::~ThreadPool()
{
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_stop = true;
  }

  m_conditionVariable.notify_all();

  for (auto& thread : m_threads)
  {
    thread.join();
  }
}

PerftResult perft(Board& board, int depth)
{
  PerftResult result{};

  auto threadPool_p = std::make_unique<ThreadPool>(std::thread::hardware_concurrency());
  return perftHelper(board, depth, std::move(threadPool_p));
}

PerftResult perftHelper(Board& board, int depth, std::unique_ptr<ThreadPool> threadPool_p)
{
  PerftResult result{};

  std::vector<Move> moves = board.generateMoves();

  if (depth == 1)
  {
    result.m_total = moves.size();
    // TODO (haigh) use ranges?
    for (const auto& move : moves)
    {
      if (isCapture(move)) ++result.m_captures;
    }
    return result;
  }

  std::vector<std::future<PerftResult>> futures;

  if (threadPool_p)
  {
    auto currentFen = board.toFen();

    for (const auto& move : moves)
    {
      auto task = [move, depth, currentFen]
      {
        Board localBoard{ currentFen };
        localBoard.makeMove(move);
        auto result = perftHelper(localBoard, depth - 1, nullptr);
        localBoard.undoMove();
        return result;
      };

      futures.push_back(threadPool_p->enqueue(std::move(task)));
    }
  }
  else
  {
    for (const auto& move : moves)
    {
      Board::MoveResult moveResult = board.makeMove(move);

      PerftResult newResult = perftHelper(board, depth - 1, nullptr);

      result.m_total += newResult.m_total;
      result.m_captures += newResult.m_captures;

      (void) board.undoMove();
    }
  }

  if (not threadPool_p) return result;

  for (auto& future : futures)
  {
    auto newResult = future.get();
    result.m_total += newResult.m_total;
    result.m_captures += newResult.m_captures;
  }

  return result;
}

} // namespace yak
