#include "Perft.h"

#include <board.h>
#include <iostream>

int main(int argv, char** argc)
{
  if (argv < 3)
  {
    std::cerr << "Not enough arguments provided" << std::endl;
    return 1;
  }

  int depth{ std::stoi(argc[1]) };
  --depth;
  std::string fen{ argc[2] };

  yak::Board board{ argc[2] };
  std::cout << "Provided fen: " << board.toFen() << std::endl;

  auto moves = board.generateMoves();

  int total;
  for (const auto& move : moves)
  {
    board.makeMove(move);
    auto result = perft(board, depth);

    std::cout << move.toAlgebraic() << " " << result.m_total << std::endl;
    total += result.m_total;

    board.undoMove();
  }

  std::cout << std::endl << total << std::endl << std::endl;

  return 0;
}

