#include <AttackMaps.hpp>
#include <MagicCommon.h>
#include <types.h>

namespace yak {

template<Square from, Square to>
consteval Bitboard betweenSet()
{
  constexpr Bitboard to_bb = bitboard::static_bitboard<to>::value;

  if (attackmap::Ray<Direction::NORTH, from>::value & to_bb)
  {
    return attackmap::blockedRay<Direction::NORTH>(from, to_bb) ^ to_bb;
  }
  else if (attackmap::Ray<Direction::NORTH_EAST, from>::value & to_bb)
  {
    return attackmap::blockedRay<Direction::NORTH_EAST>(from, to_bb) ^ to_bb;
  }
  else if (attackmap::Ray<Direction::EAST, from>::value & to_bb)
  {
    return attackmap::blockedRay<Direction::EAST>(from, to_bb) ^ to_bb;
  }
  else if (attackmap::Ray<Direction::SOUTH_EAST, from>::value & to_bb)
  {
    return attackmap::blockedRay<Direction::SOUTH_EAST>(from, to_bb) ^ to_bb;
  }
  else if (attackmap::Ray<Direction::SOUTH, from>::value & to_bb)
  {
    return attackmap::blockedRay<Direction::SOUTH>(from, to_bb) ^ to_bb;
  }
  else if (attackmap::Ray<Direction::SOUTH_WEST, from>::value & to_bb)
  {
    return attackmap::blockedRay<Direction::SOUTH_WEST>(from, to_bb) ^ to_bb;
  }
  else if (attackmap::Ray<Direction::WEST, from>::value & to_bb)
  {
    return attackmap::blockedRay<Direction::WEST>(from, to_bb) ^ to_bb;
  }
  else if (attackmap::Ray<Direction::NORTH_WEST, from>::value & to_bb)
  {
    return attackmap::blockedRay<Direction::NORTH_WEST>(from, to_bb) ^ to_bb;
  }

  return 0;
}

template<Square from, Square to, Bitboard... sets>
struct OneSquareInBetween : OneSquareInBetween<from, static_cast<Square>(to + 1), sets..., betweenSet<from, to>()>
{
};

template<Square from, Bitboard... sets>
struct OneSquareInBetween<from, H8, sets...>
{
  static constexpr std::array<Bitboard, 64> value{ sets..., betweenSet<from, H8>() };
};

template<Square S, std::array<Bitboard, 64>... arrays>
struct InBetweenImpl : InBetweenImpl<static_cast<Square>(S + 1), arrays..., OneSquareInBetween<S, A1>::value>
{
};

template<std::array<Bitboard, 64>... arrays>
struct InBetweenImpl<H8, arrays...>
{
  static constexpr std::array<std::array<Bitboard, 64>, 64> value{ arrays... };
};

struct InBetween
{
  static constexpr std::array<std::array<Bitboard, 64>, 64> value{ InBetweenImpl<A1>::value };
};

template<PieceType Type>
constexpr Bitboard xRayAttacks(Bitboard occupied_bb, Bitboard blocker_bb, Square piece)
{
  auto attacks = magic::singleSquareAttack<Type>(piece, occupied_bb);
  blocker_bb &= attacks;
  return attacks ^ magic::singleSquareAttack<Type>(piece, occupied_bb ^ blocker_bb);
}

template<PieceType Type>
constexpr Bitboard pinners(Bitboard piece_bb, Bitboard occupied_bb, Bitboard friendly_bb, Square king)
{
  return xRayAttacks<Type>(occupied_bb, friendly_bb, king) & piece_bb;
}

template<PieceType Type>
constexpr Bitboard pinned(Bitboard piece_bb, Square kingSquare, Bitboard occupied_bb, Bitboard blocker_bb)
{
  auto pinners_bb = pinners<Type>(piece_bb, occupied_bb, blocker_bb, kingSquare);

  Bitboard pinned_bb{0};

  while (pinners_bb)
  {
    auto square = bitboard::popLS1B(pinners_bb);
    pinned_bb |= InBetween::value[kingSquare][square] & blocker_bb;
  }

  return pinned_bb;
}

} // namespace yak
