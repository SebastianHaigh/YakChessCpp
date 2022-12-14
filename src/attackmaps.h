#ifndef YAK_ATTACK_MAPS_H_
#define YAK_ATTACK_MAPS_H_

#include <stdint.h>
#include "bitboard.h"
#include <stack>
#include <array>

namespace yak {
namespace attackmap {

//template<Direction D>
//constexpr Bitboard shifta(Bitboard board)
//{
//  if (D == Direction::NORTH)
//    return board << 8;
//  if (D == Direction::SOUTH)
//    return board >> 8;
//  if (D == Direction::EAST)
//    return (board << 1) & bitboard::NOT_FILE_A;
//  if (D == Direction::WEST)
//    return (board >> 1) & bitboard::NOT_FILE_H;
//  if (D == Direction::NORTH_EAST)
//    return (board << 9) & bitboard::NOT_FILE_A;
//  if (D == Direction::SOUTH_EAST)
//    return (board >> 7) & bitboard::NOT_FILE_A;
//  if (D == Direction::NORTH_WEST)
//    return (board << 7) & bitboard::NOT_FILE_H;
//  if (D == Direction::SOUTH_WEST)
//    return (board >> 9) & bitboard::NOT_FILE_H;
//  return board;
//};

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
template<Direction D, Square S = 0, Bitboard... B>
struct rayMap : rayMap<D, S + 1, B..., ray<D, S>::value>
{
};

template<Direction D, Bitboard... B>
struct rayMap<D, 63, B...>
{
  static constexpr std::array<Bitboard, 64> value = {B...};
};

template<Direction D> struct isPositiveRay : std::true_type {};
template<> struct isPositiveRay<Direction::SOUTH> : std::false_type {};
template<> struct isPositiveRay<Direction::WEST> : std::false_type {};
template<> struct isPositiveRay<Direction::SOUTH_EAST> : std::false_type {};
template<> struct isPositiveRay<Direction::SOUTH_WEST> : std::false_type {};

template<Direction D>
Bitboard blocked_ray(Square square, Bitboard occupied)
{
  constexpr std::array<Bitboard, 64> thisDirectionRayMap = rayMap<D>::value;

  Bitboard piecesInRay = thisDirectionRayMap[square] & occupied;

  if (piecesInRay == 0)
    return thisDirectionRayMap[square];

  Square blockerSquare{ 0};

  if (isPositiveRay<D>::value)
    blockerSquare = bitboard::LS1B(piecesInRay);
  else
    blockerSquare = bitboard::MS1B(piecesInRay);

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

/**
 * \brief Attack map for a knight on a given square.
 * \tparam S - The square the the knight is on.
 */
template<Square S>
struct knight_map
{
  static constexpr Bitboard
    value = bitboard::shift<Direction::NORTH>(bitboard::shift<Direction::NORTH_EAST>(bitboard::static_bitboard<S>::value))
    | bitboard::shift<Direction::NORTH>(bitboard::shift<Direction::NORTH_WEST>(bitboard::static_bitboard<S>::value))
    | bitboard::shift<Direction::EAST>(bitboard::shift<Direction::NORTH_EAST>(bitboard::static_bitboard<S>::value))
    | bitboard::shift<Direction::EAST>(bitboard::shift<Direction::SOUTH_EAST>(bitboard::static_bitboard<S>::value))
    | bitboard::shift<Direction::SOUTH>(bitboard::shift<Direction::SOUTH_EAST>(bitboard::static_bitboard<S>::value))
    | bitboard::shift<Direction::SOUTH>(bitboard::shift<Direction::SOUTH_WEST>(bitboard::static_bitboard<S>::value))
    | bitboard::shift<Direction::WEST>(bitboard::shift<Direction::SOUTH_WEST>(bitboard::static_bitboard<S>::value))
    | bitboard::shift<Direction::WEST>(bitboard::shift<Direction::NORTH_WEST>(bitboard::static_bitboard<S>::value));
};

/**
 * \brief Attack map for a king on a given square.
 * \tparam S - The square the the king is on.
 */
template<Square S>
struct king_map
{
  static constexpr Bitboard value = bitboard::shift<Direction::NORTH>(bitboard::static_bitboard<S>::value)
    | bitboard::shift<Direction::EAST>(bitboard::static_bitboard<S>::value)
    | bitboard::shift<Direction::SOUTH>(bitboard::static_bitboard<S>::value)
    | bitboard::shift<Direction::WEST>(bitboard::static_bitboard<S>::value)
    | bitboard::shift<Direction::NORTH_EAST>(bitboard::static_bitboard<S>::value)
    | bitboard::shift<Direction::NORTH_WEST>(bitboard::static_bitboard<S>::value)
    | bitboard::shift<Direction::SOUTH_EAST>(bitboard::static_bitboard<S>::value)
    | bitboard::shift<Direction::SOUTH_WEST>(bitboard::static_bitboard<S>::value);
};

} // namespace attackmap

} // namespace yak



#endif // YAK_ATTACK_MAPS_H_