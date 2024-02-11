#include "bitboard.h"
#include <string>

namespace yak {

namespace bitboard {

Square popLS1B(Bitboard &board)
{
  Square idx = LS1B(board);
  board &= board - 1;
  return idx;
}

Square popMS1B(Bitboard &board)
{
  Square idx = MS1B(board);
  board &= ~(Bitboard{1} << idx);
  return idx;
}

void setSquare(Bitboard& board, const Square& square)
{
  Bitboard pieceToSet{1};
  board |= pieceToSet << square;
}

void setSquare(Bitboard &board, const Rank& rank, const File& file)
{
  setSquare(board, squareIndex(file, rank));
}

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

Square firstOccupiedSquare(const Bitboard& board)
{
  for (int square = 0; square < 64; square++)
  {
    if (board & toBitboard(static_cast<Square>(square)))
    {
      return static_cast<Square>(square);
    }
  }

  return Square(64);
}

Bitboard toBitboard(const Square& square)
{
  return Bitboard(1) << square;
}

Bitboard toBitboard(const File& file, const Rank& rank)
{
  return toBitboard(squareIndex(file, rank));
};

Bitboard toBitboard(std::string_view square)
{
  return Bitboard(1) << squareIndex(square);
}

void print_board(Bitboard board)
{
  std::vector<std::vector<bool>> boardVector(8, std::vector<bool>(8, false));

  for (Rank rank = 0; rank < 8; rank++)
  {
    for (File file = 0; file < 8; file++)
    {
      boardVector[rank][file] = (board & toBitboard(file, rank)) > 0;
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
      boardVector[rank][file] = (board & toBitboard(file, rank)) > 0;
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

} // namespace bitboard

} // namespace yak
