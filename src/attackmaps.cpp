#include "attackmaps.h"

namespace yak {

namespace attackmap {

Bitboard BishopMap::attacks(Square square, Bitboard occupied_squares)
{
  Bitboard atk_bb{ 0 };
  atk_bb |= blocked_ray<Direction::NORTH_EAST>(square, occupied_squares);
  atk_bb |= blocked_ray<Direction::NORTH_WEST>(square, occupied_squares);
  atk_bb |= blocked_ray<Direction::SOUTH_EAST>(square, occupied_squares);
  atk_bb |= blocked_ray<Direction::SOUTH_WEST>(square, occupied_squares);
  return atk_bb;
}

Bitboard QueenMap::attacks(Square square, Bitboard occupied_squares)
{
  Bitboard atk_bb{ 0 };
  atk_bb |= blocked_ray<Direction::NORTH>(square, occupied_squares);
  atk_bb |= blocked_ray<Direction::EAST>(square, occupied_squares);
  atk_bb |= blocked_ray<Direction::SOUTH>(square, occupied_squares);
  atk_bb |= blocked_ray<Direction::WEST>(square, occupied_squares);
  atk_bb |= blocked_ray<Direction::NORTH_EAST>(square, occupied_squares);
  atk_bb |= blocked_ray<Direction::NORTH_WEST>(square, occupied_squares);
  atk_bb |= blocked_ray<Direction::SOUTH_EAST>(square, occupied_squares);
  atk_bb |= blocked_ray<Direction::SOUTH_WEST>(square, occupied_squares);
  return atk_bb;
}

Bitboard RookMap::attacks(Square square, Bitboard occupied_squares)
{
  Bitboard atk_bb{ 0 };
  atk_bb |= blocked_ray<Direction::NORTH>(square, occupied_squares);
  atk_bb |= blocked_ray<Direction::EAST>(square, occupied_squares);
  atk_bb |= blocked_ray<Direction::SOUTH>(square, occupied_squares);
  atk_bb |= blocked_ray<Direction::WEST>(square, occupied_squares);
  return atk_bb;
}

} // namespace attackmap

} // namespace yak



