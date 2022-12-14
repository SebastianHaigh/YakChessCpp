#include "attackmaps.h"

namespace yak {

namespace attackmap {

Bitboard BishopMap::attacks(Square square, Bitboard occupiedSquares)
{
  Bitboard atk_bb{ 0 };
  atk_bb |= blocked_ray<Direction::NORTH_EAST>(square, occupiedSquares);
  atk_bb |= blocked_ray<Direction::NORTH_WEST>(square, occupiedSquares);
  atk_bb |= blocked_ray<Direction::SOUTH_EAST>(square, occupiedSquares);
  atk_bb |= blocked_ray<Direction::SOUTH_WEST>(square, occupiedSquares);
  return atk_bb;
}

Bitboard QueenMap::attacks(Square square, Bitboard occupiedSquares)
{
  Bitboard atk_bb{ 0 };
  atk_bb |= blocked_ray<Direction::NORTH>(square, occupiedSquares);
  atk_bb |= blocked_ray<Direction::EAST>(square, occupiedSquares);
  atk_bb |= blocked_ray<Direction::SOUTH>(square, occupiedSquares);
  atk_bb |= blocked_ray<Direction::WEST>(square, occupiedSquares);
  atk_bb |= blocked_ray<Direction::NORTH_EAST>(square, occupiedSquares);
  atk_bb |= blocked_ray<Direction::NORTH_WEST>(square, occupiedSquares);
  atk_bb |= blocked_ray<Direction::SOUTH_EAST>(square, occupiedSquares);
  atk_bb |= blocked_ray<Direction::SOUTH_WEST>(square, occupiedSquares);
  return atk_bb;
}

Bitboard RookMap::attacks(Square square, Bitboard occupiedSquares)
{
  Bitboard atk_bb{ 0 };
  atk_bb |= blocked_ray<Direction::NORTH>(square, occupiedSquares);
  atk_bb |= blocked_ray<Direction::EAST>(square, occupiedSquares);
  atk_bb |= blocked_ray<Direction::SOUTH>(square, occupiedSquares);
  atk_bb |= blocked_ray<Direction::WEST>(square, occupiedSquares);
  return atk_bb;
}

} // namespace attackmap

} // namespace yak



