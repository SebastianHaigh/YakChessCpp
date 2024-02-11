#include <types.h>

namespace yak {

File fileIndex(const Square& squareIndex)
{
  if (squareIndex >= 0 && squareIndex <= 63)
  {
    return squareIndex & 7;
  }
  else
  {
    return -1;
  }
}

File fileIndex(char algebraicFile)
{
  return toupper(algebraicFile) - 'A';
}

File fileIndex(std::string_view algebraic_square)
{
  return fileIndex(algebraic_square[0]);
}

Rank rankIndex(const Square& squareIndex)
{
  if (squareIndex >= 0 && squareIndex <= 63)
  {
    return squareIndex >> 3;
  }
  else
  {
    return -1;
  }
}

Rank rankIndex(char algebraicRank)
{
  return algebraicRank - '1';
}

Rank rankIndex(std::string_view algebraicSquare)
{
  return fileIndex(algebraicSquare[1]);
}

Square squareIndex(File fileIndex, Rank rankIndex)
{
  return static_cast<Square>((8 * rankIndex) + fileIndex);
}

Square squareIndex(std::string_view square)
{
  File file = fileIndex(square[0]);
  Rank rank = rankIndex(square[1]);
  return squareIndex(file, rank);
}

std::string toAlgebraic(Square square)
{
  static constexpr std::string files{"abcdefgh"};
  return files[fileIndex(square)] + std::to_string(rankIndex(square) + 1);
}

std::string toAlgebraic(File file_index, Rank rank_index)
{
  static constexpr std::string files{"abcdefgh"};
  return files[file_index] + std::to_string(rank_index + 1);
}

} // namespace yak
