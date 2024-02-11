#include <attackmaps.h>

namespace yak::attackmap {

Bitboard BishopMap::attacks(Square square, Bitboard occupiedSquares)
{
  Bitboard atk_bb{ 0 };
  atk_bb |= blockedRay<Direction::NORTH_EAST>(square, occupiedSquares);
  atk_bb |= blockedRay<Direction::NORTH_WEST>(square, occupiedSquares);
  atk_bb |= blockedRay<Direction::SOUTH_EAST>(square, occupiedSquares);
  atk_bb |= blockedRay<Direction::SOUTH_WEST>(square, occupiedSquares);
  return atk_bb;
}

Bitboard QueenMap::attacks(Square square, Bitboard occupiedSquares)
{
  Bitboard atk_bb{ 0 };
  atk_bb |= blockedRay<Direction::NORTH>(square, occupiedSquares);
  atk_bb |= blockedRay<Direction::EAST>(square, occupiedSquares);
  atk_bb |= blockedRay<Direction::SOUTH>(square, occupiedSquares);
  atk_bb |= blockedRay<Direction::WEST>(square, occupiedSquares);
  atk_bb |= blockedRay<Direction::NORTH_EAST>(square, occupiedSquares);
  atk_bb |= blockedRay<Direction::NORTH_WEST>(square, occupiedSquares);
  atk_bb |= blockedRay<Direction::SOUTH_EAST>(square, occupiedSquares);
  atk_bb |= blockedRay<Direction::SOUTH_WEST>(square, occupiedSquares);
  return atk_bb;
}

Bitboard RookMap::attacks(Square square, Bitboard occupiedSquares)
{
  Bitboard atk_bb{ 0 };
  atk_bb |= blockedRay<Direction::NORTH>(square, occupiedSquares);
  atk_bb |= blockedRay<Direction::EAST>(square, occupiedSquares);
  atk_bb |= blockedRay<Direction::SOUTH>(square, occupiedSquares);
  atk_bb |= blockedRay<Direction::WEST>(square, occupiedSquares);
  return atk_bb;
}

} // namespace yak::attackmap


