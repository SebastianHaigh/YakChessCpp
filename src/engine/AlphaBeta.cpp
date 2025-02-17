#include "AlphaBeta.h"

#include <board.h>

#include <limits>

namespace yak::engine {

int alphaBeta(Board& board, int alpha, int beta, int depth, bool maximise)
{
  if (depth == 0)
  {
    return evaluate(board);
  }

  if (maximise)
  {
    int maxEvaluation = std::numeric_limits<int>::min();

    for (const auto& move : board.generateMoves())
    {
      board.makeMove(move);
      auto evaluation = alphaBeta(board, alpha, beta, depth - 1, false);
      board.undoMove();

      if (evaluation > maxEvaluation)
      {
        maxEvaluation = alpha;

        if (evaluation > alpha)
        {
          alpha = evaluation;
        }
      }

      if (evaluation >= beta)
      {
        break;
      }
    }

    return maxEvaluation;
  }

  int minEvaluation = std::numeric_limits<int>::max();
  for (const auto& move : board.generateMoves())
  {
    board.makeMove(move);
    auto evaluation = alphaBeta(board, alpha, beta, true);
    board.undoMove();

    if (evaluation < minEvaluation)
    {
      minEvaluation = evaluation;

      if (evaluation < beta)
      {
        beta = evaluation;
      }
    }

    if (evaluation <= alpha)
    {
      break;
    }
  }

  return minEvaluation;

}

int evaluate(Board& board)
{
  return 0;
}

} // namespace yak::engine
