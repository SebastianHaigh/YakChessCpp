#include "Perft.h"

#include <board.h>
#include <types.h>

namespace yak {

PerftResult perft(Board& board, int depth)
{
  PerftResult result{};

  return perftHelper(board, depth);
}

PerftResult perftHelper(Board& board, int depth)
{
  PerftResult result{};

  std::vector<Move> moves = board.generateMoves();

  if (depth == 1)
  {
    result.m_total = moves.size();
    // TODO (haigh) use ranges?
    for (const auto& move : moves)
    {
      if (move.capture) ++result.m_captures;
    }
    return result;
  }

  for (const auto& move : moves)
  {
    Board::MoveResult moveResult = board.makeMove(move);

    PerftResult newResult = perftHelper(board, depth - 1);

    result.m_total += newResult.m_total;
    result.m_captures += newResult.m_captures;

    (void) board.undoMove();
  }

  return result;
}

} // namespace yak
