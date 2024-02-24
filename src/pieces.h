#ifndef YAK_PIECES_H_
#define YAK_PIECES_H_

#include "bitboard.h"
#include "attackmaps.h"
#include "MagicBitboards.h"
#include "types.h"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>
#include <stack>
#include <stdint.h>
#include <stdio.h>
#include <cassert>

#define MAGIC
namespace yak::piece {

template<PieceType Type>
class MagicBitboardImpl
{
public:
  MagicBitboardImpl()
  {
    m_magics = magic::findAllMagics<Type>();
  }

  auto attacks(Square square, Bitboard occupied) -> Bitboard
  {
    if (square > H8) [[unlikely]] return 0;

    auto const& magic = m_magics[square];

    return magic.m_map[magic::transform(occupied & magic.m_mask,
                                        magic.m_magic,
                                        bitboard::countSetBits(occupied & magic.m_mask))];
  }
private:
  std::array<magic::MagicReturn, 64> m_magics;
};

template<PieceType Type>
class SlidingPieceMap
{
public:
  /* auto attacks(Square square, Bitboard occupied) -> Bitboard */
  /* { */
  /*   /1* m_impl.attacks(square, occupied); *1/ */
  /* } */

private:

#ifdef MAGIC
  MagicBitboardImpl<Type> m_impl;
#else
#endif
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

template<PieceType Type, Square S>
requires Jumpable<Type>
constexpr Bitboard jumpingPieceMap()
{
  if constexpr (Type == PieceType::KNIGHT)
  {
    return knight_map<S>::value;
  }

  return king_map<S>::value;
}

/**
 * \brief Static table of jumping piece attacks.
 */
template<PieceType T, Square S = A1, Bitboard... B>
struct jump_map : jump_map<T,
                           static_cast<Square>(S + 1),
                           B...,
                           jumpingPieceMap<T, S>()>
                           /* (T == PieceType::KNIGHT) */
                           /*   ? attackmap::knight_map<S>::value */
                           /*   : (T == PieceType::KING) */
                           /*     ? attackmap::king_map<S>::value */
                           /*     : Bitboard{0}> */
{
};

template<PieceType T, Bitboard... B>
struct jump_map<T, H8, B...>
{
  static constexpr std::array<Bitboard, 64> value = {B...};
};

constexpr std::array<Bitboard, 64> knightMap = jump_map<PieceType::KNIGHT>::value;
constexpr std::array<Bitboard, 64> kingMap = jump_map<PieceType::KING>::value;

class KnightMap
{
public:
  static Bitboard attacks(Square square)
  {
    return knightMap[square];
  }
};

class KingMap
{
public:
  static Bitboard attacks(Square square)
  {
    return kingMap[square];
  }
};

static SlidingPieceMap<PieceType::BISHOP> bishopMap{};
static SlidingPieceMap<PieceType::ROOK> rookMap{};
/**
 * \brief Get all of the squares attacked by a piece type.
 * \tparam TYPE - The piece type.
 * \param[in] piecePositions - Bitboard of the piece positions.
 * \return Bitboard of the squares attacked by the piece type.
 */
template<PieceType TYPE>
Bitboard pieceAttacks(Bitboard piecePositions, Bitboard occupiedSquares)
{
  Bitboard atk_bb{0};
  while (piecePositions)
  {
    if (TYPE == PieceType::KNIGHT)
      atk_bb |= KnightMap::attacks(bitboard::popLS1B(piecePositions));
    if (TYPE == PieceType::KING)
      atk_bb |= KingMap::attacks(bitboard::popLS1B(piecePositions));
    if (TYPE == PieceType::BISHOP)
      atk_bb |= attackmap::BishopMap::attacks(bitboard::popLS1B(piecePositions), occupiedSquares);
    if (TYPE == PieceType::ROOK)
      atk_bb |= attackmap::RookMap::attacks(bitboard::popLS1B(piecePositions), occupiedSquares);
    if (TYPE == PieceType::QUEEN)
      atk_bb |= attackmap::QueenMap::attacks(bitboard::popLS1B(piecePositions), occupiedSquares);
  }
  return atk_bb;
}
template<PieceType Type>
std::string toString()
{
  if (Type == PieceType::PAWN) return "PAWN";
  if (Type == PieceType::KNIGHT) return "KNIGHT";
  if (Type == PieceType::BISHOP) return "BISHOP";
  if (Type == PieceType::ROOK) return "ROOK";
  if (Type == PieceType::QUEEN) return "QUEEN";
  if (Type == PieceType::KING) return "KING";
  return "NULL";
}

PieceColour otherColour(PieceColour colour);
PieceType fenCharToPieceType(const char fenChar);
PieceColour fenCharToPieceColour(const char fenChar);
char pieceToFenChar(const PieceType& type, const PieceColour& colour);

} // namespace yak::piece

#endif // YAK_PIECES_H_
