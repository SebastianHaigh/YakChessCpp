#include "Perft.h"
#include "bitboard.h"
#include "board.h"
#include <stdexcept>
#include <string>

namespace yak {

PerftResult perft(int depth)
{
  PerftResult result{};

  const std::string startingFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

  Board board{startingFen};

  return perftHelper(board, depth);
}

PerftResult perftHelper(Board& board, int depth)
{
  PerftResult result{};

  std::vector<Move> moves = board.generateMoves();

  if (depth == 1)
  {
    result.m_total = moves.size();
    return result;
  }

  for (const auto& move : moves)
  {
    Board::MoveResult mresult = board.makeMove(move);

    if (mresult != Board::MoveResult::SUCCESS)
    {
      throw std::runtime_error{"Make move failed with: " + std::to_string(static_cast<int>(mresult))};
    }

    PerftResult newResult = perftHelper(board, depth - 1);

    result.m_total += newResult.m_total;

    Board::MoveResult uresult = board.undoMove();
    if (uresult != Board::MoveResult::SUCCESS)
    {
      throw std::runtime_error{"undo move failed with: " + std::to_string(static_cast<int>(uresult))};
    }
  }

  return result;
}

} // namespace yak
