#pragma once

#include "bitboard.h"
#include "types.h"

#include <type_traits>
#include <array>

namespace yak::attackmap {

/* ------------------------------------------------------------------ */
/* GENERATE STATIC TABLE FOR RAY MAP LOOKUP                           */
/* ------------------------------------------------------------------ */

/**
 * \brief Metafunction for ray generation.
 * \tparam D - Ray direction.
 * \tparam S - Square of the piece casting the ray.
 */
template<Direction D, Square S, Bitboard B = bitboard::shift<D>(Bitboard{1} << S)>
struct Ray
{
  static constexpr Bitboard value = B | Ray<D, S, bitboard::shift<D>(B)>::value;
};

/**
 * \brief Base case specialisation when bitboard is equal to 0.
 * \tparam D - Ray direction.
 * \tparam S - Square of the piece casting the ray.
 */
template<Direction D, Square S>
struct Ray<D, S, 0>
{
  static constexpr Bitboard value{0};
};

/**
 * \brief Static table of sliding piece rays.
 * \tparam D - The direction of the ray.
 * \details
 * Usage: ray_map<Direction::NORTH>::value[42];
 */
template<Direction D, Square S = A1, Bitboard... B>
struct RayMap : RayMap<D, static_cast<Square>(S + 1), B..., Ray<D, S>::value>
{
};

template<Direction D, Bitboard... B>
struct RayMap<D, static_cast<Square>(64), B...>
{
  static constexpr std::array<Bitboard, 64> value = {B...};
};

template<Direction D> struct IsPositiveRay : std::true_type {};
template<> struct IsPositiveRay<Direction::SOUTH> : std::false_type {};
template<> struct IsPositiveRay<Direction::WEST> : std::false_type {};
template<> struct IsPositiveRay<Direction::SOUTH_EAST> : std::false_type {};
template<> struct IsPositiveRay<Direction::SOUTH_WEST> : std::false_type {};

template<Direction D>
constexpr auto blockedRay(Square square, Bitboard occupied) -> Bitboard
{
  constexpr std::array<Bitboard, 64> thisDirectionRayMap = RayMap<D>::value;

  const Bitboard piecesInRay = thisDirectionRayMap[square] & occupied;

  if (piecesInRay == 0)
  {
    return thisDirectionRayMap[square];
  }

  if constexpr (IsPositiveRay<D>::value)
  {
    return thisDirectionRayMap[square] ^ thisDirectionRayMap[bitboard::LS1B(piecesInRay)];
  }
  else
  {
    return thisDirectionRayMap[square] ^ thisDirectionRayMap[bitboard::MS1B(piecesInRay)];
  }
}

} // namespace yak::attackmap
