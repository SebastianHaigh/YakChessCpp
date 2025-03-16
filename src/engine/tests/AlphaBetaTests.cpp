#include <catch2/catch_test_macros.hpp>

#include <board.h>
#include <AlphaBeta.h>
#include <iostream>

namespace yak::engine {

static constexpr std::string_view STANDARD_STARTING_FEN{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };

TEST_CASE("Alpha beta test 1")
{
  Board board{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };

  std::cout << board.to_string() << std::endl << std::endl;

  int numMoves{ 0 };

  while (!board.isCheckmate() && numMoves < 10)
  {
    auto [scoreForWhite, bestMoveForWhite] = alphaBeta(board, 6, PieceColour::WHITE);
    std::cout << "WHITE: " << toAlgebraic(bestMoveForWhite) << " => " << scoreForWhite << std::endl;
    board.makeMove(bestMoveForWhite);
    std::cout << board.to_string() << std::endl << std::endl;

    if (board.isCheckmate()) break;

    auto [scoreForBlack, bestMoveForBlack] = alphaBeta(board, 6, PieceColour::BLACK);
    std::cout << "BLACK: " << toAlgebraic(bestMoveForBlack) << " => " << scoreForBlack << std::endl;
    board.makeMove(bestMoveForBlack);
    std::cout << board.to_string() << std::endl << std::endl;

    ++numMoves;
  }
}

} // namespace yak

