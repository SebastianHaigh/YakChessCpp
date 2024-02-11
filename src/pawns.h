#ifndef YAK_PAWNS_H_
#define YAK_PAWNS_H_

#include "bitboard.h"
#include "types.h"

namespace yak::pawns {

/**
 * \brief Target squares of a pawn single push.
 * \tparam COLOUR - the colour of the pawns.
 * \param[in] source - the source squares for which to find the targets.
 * \return The Bitboard of the target squares.
 */
template<PieceColour Colour>
constexpr Bitboard pawnSinglePushTarget(Bitboard source)
{
  return (Colour == PieceColour::WHITE) ? bitboard::shift<Direction::NORTH>(source) :
         (Colour == PieceColour::BLACK) ? bitboard::shift<Direction::SOUTH>(source) : source;
}

/**
 * \brief Source squares of a pawn single push.
 * \tparam COLOUR - the colour of the pawns.
 * \param[in] target - the target squares for which to find the sources.
 * \return The Bitboard of the source squares.
 */
template<PieceColour Colour>
constexpr Bitboard pawnSinglePushSource(Bitboard target)
{
  return (Colour == PieceColour::WHITE) ? bitboard::shift<Direction::SOUTH>(target) :
         (Colour == PieceColour::BLACK) ? bitboard::shift<Direction::NORTH>(target) : target;
}

/**
 * \brief The target squares for a double push from the starting rank.
 * \tparam COLOUR - the colour of the pawns.
 * \return The Bitboard of the target squares.
 */
template<PieceColour Colour>
constexpr Bitboard pawnDoublePushTarget()
{
  return (Colour == PieceColour::WHITE) ? bitboard::RANK_4 :
         (Colour == PieceColour::BLACK) ? bitboard::RANK_5 : bitboard::EMPTY;
}

/**
 * \brief Source squares of pawns attacking to the west.
 * \tparam COLOUR - the colour of the pawns.
 * \param[in] target - the target squares for which to find the sources.
 * \return The Bitboard of the source squares.
 */
template<PieceColour Colour>
constexpr Bitboard pawnWestAttackSource(Bitboard target)
{
  return (Colour == PieceColour::WHITE) ? bitboard::shift<Direction::SOUTH_EAST>(target) :
         (Colour == PieceColour::BLACK) ? bitboard::shift<Direction::NORTH_EAST>(target) : target;
}

/**
 * \brief Target squares of pawns attacking to the west.
 * \tparam COLOUR - the colour of the pawns.
 * \param[in] source - the source squares for which to find the targets.
 * \return The Bitboard of the target squares.
 */
template<PieceColour Colour>
constexpr Bitboard pawnWestAttackTarget(Bitboard source)
{
  return (Colour == PieceColour::WHITE) ? bitboard::shift<Direction::NORTH_WEST>(source) :
         (Colour == PieceColour::BLACK) ? bitboard::shift<Direction::SOUTH_WEST>(source) : source;
}

/**
 * \brief Target squares of pawns attacking to the east.
 * \tparam COLOUR - the colour of the pawns.
 * \param[in] source - the source squares for which to find the targets.
 * \return The Bitboard of the target squares.
 */
template<PieceColour Colour>
constexpr Bitboard pawnEastAttackSource(Bitboard target)
{
  return (Colour == PieceColour::WHITE) ? bitboard::shift<Direction::SOUTH_WEST>(target) :
         (Colour == PieceColour::BLACK) ? bitboard::shift<Direction::NORTH_WEST>(target) : target;
}

/**
 * \brief Target squares of pawns attacking to the east.
 * \tparam COLOUR - the colour of the pawns.
 * \param[in] source - the source squares for which to find the targets.
 * \return The Bitboard of the target squares.
 */
template<PieceColour Colour>
constexpr Bitboard pawnEastAttackTarget(Bitboard source)
{
  return (Colour == PieceColour::WHITE) ? bitboard::shift<Direction::NORTH_EAST>(source) :
         (Colour == PieceColour::BLACK) ? bitboard::shift<Direction::SOUTH_EAST>(source) : source;
}

template<PieceColour Colour>
constexpr Bitboard promotablePawns(Bitboard pawnPositions)
{
  return (Colour == PieceColour::WHITE) ? (bitboard::RANK_7 & pawnPositions) :
         (Colour == PieceColour::BLACK) ? (bitboard::RANK_2 & pawnPositions) : bitboard::EMPTY;
}

template<PieceColour Colour>
constexpr Bitboard nonPromotablePawns(Bitboard pawnPositions)
{
  return ~promotablePawns<Colour>(pawnPositions) & pawnPositions;
}

/**
 * \brief Get all of the squares attacked by pawns of a given COLOUR.
 * \tparam COLOUR - The colour of the pawns.
 * \param[in] pawn_positions - Bitboard of the pawn positions.
 * \return Bitboard of the squares attack by the pawns.
 */
template<PieceColour Colour>
constexpr Bitboard pawnAttacks(Bitboard pawnPositions)
{
  Bitboard attackBitboard = (Colour == PieceColour::BLACK) ?
    bitboard::shift<Direction::SOUTH_EAST>(pawnPositions) :
    bitboard::shift<Direction::NORTH_EAST>(pawnPositions);

  attackBitboard |= (Colour == PieceColour::BLACK) ?
    bitboard::shift<Direction::SOUTH_WEST>(pawnPositions) :
    bitboard::shift<Direction::NORTH_WEST>(pawnPositions);

  return attackBitboard;
}

}


#endif // YAK_PAWNS_H_
