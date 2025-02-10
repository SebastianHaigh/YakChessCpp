#include "Perft.h"

#include <board.h>
#include <cctype>
#include <iostream>

int main(int argv, char** argc)
{
  // Check that we have received the correct number of arguments
  if (argv < 3)
  {
    std::cout << "\nPerftExt: A command line perft function\n\n";
    std::cout << "Usage:\n";
    std::cout << "PerftExt <depth> <fen>\n";

    return 0;
  }

  // Set the depth and check that it is valid
  int depth;
  try
  {
    depth = std::stoi(argc[1]);
  }
  catch (std::exception& e)
  {
    std::cerr << "Could not parse depth argument: " << argc[1] << " (" << e.what() << ")\n";
    return 1;
  }

  if (depth < 1)
  {
    std::cerr << "Minimum value for depth is 1" << "\n";
    return 1;
  }

  // Initialise the board and check the provided FEN string was valid
  std::string fen{ argc[2] };
  yak::Board board;
  if (not board.reset( argc[2] ))
  {
    std::cerr << "Could not parse provided FEN string: " << argc[2] << "\n";
    return 1;
  }

  std::cout << "Provided FEN: " << board.toFen() << "\n\n";

  auto moves = board.generateMoves();
  --depth;

  int total{ 0 };
  for (const auto& move : moves)
  {
    if (depth > 0)
    {
      board.makeMove(move);
      auto result = perft(board, depth);
      std::cout << " " << move.toAlgebraic() << " " << result.m_total << "\n";
      total += result.m_total;
      board.undoMove();
      continue;
    }

    std::cout << move.toAlgebraic() << " " << 1 << "\n";
    ++total;
  }

  std::cout << "\n Total moves: " << total << "\n\n";

  return 0;
}

