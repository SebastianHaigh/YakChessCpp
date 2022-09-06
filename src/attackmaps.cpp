#include "attackmaps.h"

namespace attacks {




} // namespace attacks
Bitboard faster::BishopMap::attacks(Square square, Bitboard occupied_squares)
{
  Bitboard atk_bb{ 0 };
  atk_bb |= blocked_ray<Direction::NORTH_EAST>(square, occupied_squares);
  atk_bb |= blocked_ray<Direction::NORTH_WEST>(square, occupied_squares);
  atk_bb |= blocked_ray<Direction::SOUTH_EAST>(square, occupied_squares);
  atk_bb |= blocked_ray<Direction::SOUTH_WEST>(square, occupied_squares);
  return atk_bb;
}
Bitboard faster::QueenMap::attacks(Square square, Bitboard occupied_squares)
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
Bitboard faster::RookMap::attacks(Square square, Bitboard occupied_squares)
{
  Bitboard atk_bb{ 0 };
  atk_bb |= blocked_ray<Direction::NORTH>(square, occupied_squares);
  atk_bb |= blocked_ray<Direction::EAST>(square, occupied_squares);
  atk_bb |= blocked_ray<Direction::SOUTH>(square, occupied_squares);
  atk_bb |= blocked_ray<Direction::WEST>(square, occupied_squares);
  return atk_bb;
}
