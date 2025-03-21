#include "AlphaBeta.h"

#include <board.h>
#include <limits>

namespace yak::engine {

std::pair<int, Move> alphaBeta(Board& board, int depth, PieceColour us)
{
  return alphaBeta(board,
                   std::numeric_limits<int>::min(),
                   std::numeric_limits<int>::max(),
                   depth,
                   (board.sideToMove() == us));
}

std::pair<int, Move> alphaBeta(Board& board, int alpha, int beta, int depth, bool maximise)
{
  if (depth == 0)
  {
    return { evaluate(board), {} };
  }

  Move bestMove;

  if (maximise)
  {
    int maxEvaluation = std::numeric_limits<int>::min();

    for (const auto& move : board.generateMoves())
    {
      board.makeMove(move);

      if (board.isCheckmate())
      {
        board.undoMove();
        return { std::numeric_limits<int>::max(), move };
      }

      auto [evaluation, _] = alphaBeta(board, alpha, beta, depth - 1, false);
      board.undoMove();

      if (evaluation > maxEvaluation)
      {
        maxEvaluation = evaluation;
        bestMove = move;

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

    return { maxEvaluation, bestMove };
  }

  int minEvaluation = std::numeric_limits<int>::max();
  for (const auto& move : board.generateMoves())
  {
    board.makeMove(move);

    if (board.isCheckmate())
    {
      board.undoMove();
      return { std::numeric_limits<int>::min(), move };
    }

    auto [evaluation, _] = alphaBeta(board, alpha, beta, depth - 1, true);
    board.undoMove();

    if (evaluation < minEvaluation)
    {
      minEvaluation = evaluation;
      bestMove = move;

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

  return { minEvaluation, bestMove };

}

int evaluate(Board& board)
{
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

  auto materialDifference = piecesWhite - piecesBlack;

  int pinnedWhitePieces = bitboard::countSetBits(board.getPinned<PieceColour::WHITE>());
  int pinnedBlackPieces = bitboard::countSetBits(board.getPinned<PieceColour::BLACK>());

  int squaresAttackedByWhite = bitboard::countSetBits(board.attacked_by(PieceColour::WHITE));
  int squaresAttackedByBlack = bitboard::countSetBits(board.attacked_by(PieceColour::BLACK));

  auto whiteScore = piecesWhite + pinnedBlackPieces + squaresAttackedByWhite;
  auto blackScore = piecesBlack + pinnedWhitePieces + squaresAttackedByBlack;

  return (whiteScore - blackScore);
}

} // namespace yak::engine
