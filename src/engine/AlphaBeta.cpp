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
        maxEvaluation = evaluation;

        if (evaluation > alpha)
        {
          alpha = evaluation;
        }
      }

      if (beta <= alpha)
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
    auto evaluation = alphaBeta(board, alpha, beta, depth - 1, true);
    board.undoMove();

    if (evaluation < minEvaluation)
    {
      minEvaluation = evaluation;

      if (evaluation < beta)
      {
        beta = evaluation;
      }
    }

    if (beta <= alpha)
    {
      break;
    }
  }

  return minEvaluation;

}

int evaluate(Board& board)
{
  if (board.isCheckmate())
  {
    if (board.isCheck(PieceColour::WHITE)) return std::numeric_limits<int>::min();
    if (board.isCheck(PieceColour::BLACK)) return std::numeric_limits<int>::max();
  }

  int piecesWhite{ 0 };
  piecesWhite += bitboard::countSetBits(board.getPosition(PieceColour::WHITE, PieceType::PAWN));
  piecesWhite += 3 * bitboard::countSetBits(board.getPosition(PieceColour::WHITE, PieceType::BISHOP));
  piecesWhite += 3 * bitboard::countSetBits(board.getPosition(PieceColour::WHITE, PieceType::KNIGHT));
  piecesWhite += 5 * bitboard::countSetBits(board.getPosition(PieceColour::WHITE, PieceType::ROOK));
  piecesWhite += 8 * bitboard::countSetBits(board.getPosition(PieceColour::WHITE, PieceType::QUEEN));

  int piecesBlack{ 0 };
  piecesBlack += bitboard::countSetBits(board.getPosition(PieceColour::BLACK, PieceType::PAWN));
  piecesBlack += 3 * bitboard::countSetBits(board.getPosition(PieceColour::BLACK, PieceType::BISHOP));
  piecesBlack += 3 * bitboard::countSetBits(board.getPosition(PieceColour::BLACK, PieceType::KNIGHT));
  piecesBlack += 5 * bitboard::countSetBits(board.getPosition(PieceColour::BLACK, PieceType::ROOK));
  piecesBlack += 8 * bitboard::countSetBits(board.getPosition(PieceColour::BLACK, PieceType::QUEEN));

  if (board.isCheck(PieceColour::WHITE)) piecesWhite += 1;
  if (board.isCheck(PieceColour::BLACK)) piecesWhite += 1;

  return (piecesWhite - piecesBlack) * 100;
}

} // namespace yak::engine
