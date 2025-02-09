#include "bitboard.h"

#include <string>
#include <iostream>

namespace yak::bitboard {

std::vector<Square> scanBackward(Bitboard board)
{
  std::vector<Square> serialisedBoard;

  while (board)
  {
    serialisedBoard.push_back(popMS1B(board));
  }

  return serialisedBoard;
}

std::vector<Square> scanForward(Bitboard board)
{
  std::vector<Square> serialisedBoard;
  while (board)
  {
    serialisedBoard.push_back(popLS1B(board));
  }
  return serialisedBoard;
}

void print_board(Bitboard board)
{
  std::vector<std::vector<bool>> boardVector(8, std::vector<bool>(8, false));

  for (Rank rank = 0; rank < 8; rank++)
  {
    for (File file = 0; file < 8; file++)
    {
      boardVector[rank][file] = (board & createBitboard(static_cast<Square>(squareIndex(file, rank)))) > 0;
    }
  }

  for (auto rit = boardVector.rbegin(); rit != boardVector.rend(); ++rit)
  {
    for (auto file : *rit)
    {
      if (file)
      {
        std::cout << " 1";
      }
      else
      {
        std::cout << " 0";
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

std::string to_string(Bitboard board)
{
  std::vector<std::vector<bool>> boardVector(8, std::vector<bool>(8, false));

  for (Rank rank = 0; rank < 8; rank++)
  {
    for (File file = 0; file < 8; file++)
    {
      boardVector[rank][file] = (board & createBitboard(squareIndex(rank, file))) > 0;
    }
  }

  std::string boardStr{};

  for (auto rit = boardVector.rbegin(); rit != boardVector.rend(); ++rit)
  {
    for (auto file : *rit)
    {
      if (file)
      {
        boardStr += " 1";
      }
      else
      {
        boardStr += " 0";
      }
    }
    boardStr += "\n";
  }
  boardStr += "\n";
  return boardStr;
}

} // namespace bitboard::yak

