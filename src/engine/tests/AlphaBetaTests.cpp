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

  while (!board.isCheckmate())
  {
    auto [score, bestMove] = alphaBeta(board, 6, PieceColour::WHITE);
    std::cout << toAlgebraic(bestMove) << " => " << score << std::endl;
    board.makeMove(bestMove);
    std::cout << board.to_string() << std::endl << std::endl;
    board.makeMove(board.generateMoves().front());
    std::cout << board.to_string() << std::endl << std::endl;
  }
}

} // namespace yak

