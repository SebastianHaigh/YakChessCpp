#ifndef YAK_PIECES_H_
#define YAK_PIECES_H_

#include "bitboard.h"
#include <magic/MagicBitboards.hpp>
#include "types.h"

#include <array>
#include <string>

namespace yak::piece {

template<PieceType Type>
class MagicBitboardImpl
{
public:
  MagicBitboardImpl()
  {
  }

  auto attacks(Square square, Bitboard occupied) -> Bitboard
  {
    if (square > H8) [[unlikely]] return 0;

    return magic::MagicBitboards<Type>(square, occupied);
  }
private:
};

template<PieceType Type>
class SlidingPieceMap
{
public:
  SlidingPieceMap() = default;

  auto attacks(Square square, Bitboard occupied) -> Bitboard
  {
    return m_impl.attacks(square, occupied);
  }

private:
  MagicBitboardImpl<Type> m_impl;
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
template<PieceType Type>
Bitboard pieceAttacks(Bitboard piecePositions, Bitboard occupiedSquares)
{
  // TODO (haigh) move this function to board
  Bitboard atk_bb{ 0 };

  while (piecePositions)
  {
    if constexpr (Type == PieceType::KNIGHT)
    {
      atk_bb |= KnightMap::attacks(bitboard::popLS1B(piecePositions));
    }

    else if constexpr (Type == PieceType::KING)
    {
      atk_bb |= KingMap::attacks(bitboard::popLS1B(piecePositions));
    }

    else if constexpr (Type == PieceType::BISHOP)
    {
      atk_bb |= bishopMap.attacks(bitboard::popLS1B(piecePositions), occupiedSquares);
    }

    else if constexpr (Type == PieceType::ROOK)
    {
      atk_bb |= rookMap.attacks(bitboard::popLS1B(piecePositions), occupiedSquares);
    }

    else if constexpr (Type == PieceType::QUEEN)
    {
      Square square{ bitboard::popLS1B(piecePositions) };
      atk_bb |= bishopMap.attacks(square, occupiedSquares);
      atk_bb |= rookMap.attacks(square, occupiedSquares);
    }

    // TODO (haigh) add failure else case
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
