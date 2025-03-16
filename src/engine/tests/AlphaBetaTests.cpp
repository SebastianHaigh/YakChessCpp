#include "engine/AlphaBeta.h"
#include "move.hpp"
#include <catch2/catch_test_macros.hpp>

#include <board.h>
#include <AlphaBeta.h>
#include <iostream>
#include <limits>

namespace yak::engine {

static constexpr std::string_view STANDARD_STARTING_FEN{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };

TEST_CASE("Alpha beta test 1")
{
  Board board{ "rnbqkbnr/ppp2ppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };

  std::cout << "init eval: " << evaluate(board) << std::endl;

  std::cout << board.to_string() << std::endl << std::endl;
  Move bestmove;

  {
    auto bestScore{ std::numeric_limits<int>::min() };
    auto moves = board.generateMoves();

    std::cout << "while move one" << std::endl;
    for (const auto& move : moves)
    {
      board.makeMove(move);
      auto score = alphaBeta(board, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), 6, false);
      std::cout << toAlgebraic(move) << " => " << score << std::endl;
      board.undoMove();
      std::cout << board.to_string() << std::endl << std::endl;

      if (score > bestScore)
      {
        bestmove = move;
        bestScore = score;
      }
    }
  }

  std::cout << "Before making the move" << std::endl;
  std::cout << board.to_string() << std::endl << std::endl;

  board.makeMove(bestmove);
  std::cout << "After making the move" << std::endl;
  std::cout << board.to_string() << std::endl << std::endl;

  board.makeMove(board.generateMoves().front());
  std::cout << board.to_string() << std::endl << std::endl;

  {
    auto bestScore{ std::numeric_limits<int>::min() };
    auto moves = board.generateMoves();

    std::cout << "while move two" << std::endl;
    for (const auto& move : moves)
    {
      board.makeMove(move);
      auto score = alphaBeta(board, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), 6, false);
      std::cout << toAlgebraic(move) << " => " << score << std::endl;
      board.undoMove();

      if (score > bestScore)
      {
        bestmove = move;
        bestScore = score;
      }
    }
  }

  board.makeMove(bestmove);
  std::cout << board.to_string() << std::endl << std::endl;

  board.makeMove(board.generateMoves().front());
  std::cout << board.to_string() << std::endl << std::endl;

}

TEST_CASE("Alpha beta test 2")
{
  /* Board board{ "rnb1kbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" }; */
  /* CHECK(board.toFen() == STANDARD_STARTING_FEN); */


  /* std::cout << "eval: " << alphaBeta(board, 1, -8, 4, false) << std::endl; */


}


} // namespace yak

