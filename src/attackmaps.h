#ifndef YAK_ATTACK_MAPS_H_
#define YAK_ATTACK_MAPS_H_

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
struct ray
{
  static constexpr Bitboard value = B | ray<D, S, bitboard::shift<D>(B)>::value;
};

/**
 * \brief Base case specialisation when bitboard is equal to 0.
 * \tparam D - Ray direction.
 * \tparam S - Square of the piece casting the ray.
 */
template<Direction D, Square S>
struct ray<D, S, 0>
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
struct rayMap : rayMap<D, static_cast<Square>(S + 1), B..., ray<D, S>::value>
{
};

template<Direction D, Bitboard... B>
struct rayMap<D, static_cast<Square>(63), B...>
{
  static constexpr std::array<Bitboard, 64> value = {B...};
};

template<Direction D> struct isPositiveRay : std::true_type {};
template<> struct isPositiveRay<Direction::SOUTH> : std::false_type {};
template<> struct isPositiveRay<Direction::WEST> : std::false_type {};
template<> struct isPositiveRay<Direction::SOUTH_EAST> : std::false_type {};
template<> struct isPositiveRay<Direction::SOUTH_WEST> : std::false_type {};

template<Direction D>
constexpr Bitboard blockedRay(Square square, Bitboard occupied)
{
  constexpr std::array<Bitboard, 64> thisDirectionRayMap = rayMap<D>::value;

  const Bitboard piecesInRay = thisDirectionRayMap[square] & occupied;

  if (piecesInRay == 0)
  {
    return thisDirectionRayMap[square];
  }

  const Square blockerSquare = (isPositiveRay<D>::value)
                               ? bitboard::LS1B(piecesInRay)
                               : bitboard::MS1B(piecesInRay);

  return thisDirectionRayMap[square] ^ thisDirectionRayMap[blockerSquare];
}

/**
 * \brief Attack map for rooks.
 */
class RookMap
{
public:
  /**
   * \brief Calculates all squares attacked by a single rook.
   * \param[in] square - the index of the square that the rook is on.
   * \param[in] occupiedSquares - a Bitboard of every occupied square.
   * \return A Bitboard of all squares attacked by the rook.
   *
   * \summary The returned bitboard will included as attacked squares
   * all blocking pieces, whether they are friendly or not.
   */
  static Bitboard attacks(Square square, Bitboard occupiedSquares);
};

class BishopMap
{
public:
  static Bitboard attacks(Square square, Bitboard occupiedSquares);
};

class QueenMap
{
public:
  static Bitboard attacks(Square square, Bitboard occupiedSquares);
};

} // namespace yak::attackmap

#endif // YAK_ATTACK_MAPS_H_
