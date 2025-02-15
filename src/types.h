#ifndef YAK_TYPES_H_
#define YAK_TYPES_H_

#include <cstdint>
#include <string>
#include <string_view>

namespace yak {

using Bitboard = uint64_t;
using File = uint64_t;
using Rank = uint64_t;

enum Square
{
  A1 = 0, B1, C1, D1, E1, F1, G1, H1,
  A2, B2, C2, D2, E2, F2, G2, H2,
  A3, B3, C3, D3, E3, F3, G3, H3,
  A4, B4, C4, D4, E4, F4, G4, H4,
  A5, B5, C5, D5, E5, F5, G5, H5,
  A6, B6, C6, D6, E6, F6, G6, H6,
  A7, B7, C7, D7, E7, F7, G7, H7,
  A8, B8, C8, D8, E8, F8, G8, H8,
  NULL_SQUARE
};

enum class RayType
{
  POSITIVE,
  NEGATIVE
};

enum class Direction
{
  NORTH,
  EAST,
  SOUTH,
  WEST,
  NORTH_EAST,
  NORTH_WEST,
  SOUTH_EAST,
  SOUTH_WEST
};

enum class PieceType
{
  PAWN = 0,
  KNIGHT,
  BISHOP,
  ROOK,
  QUEEN,
  KING,
  NULL_PIECE
};

template<typename T>
concept Type = std::is_same_v<T, PieceType>;

template<PieceType T>
concept SimplySlideable = (T == PieceType::BISHOP) || (T == PieceType::ROOK);

template<PieceType T>
concept CompoundSlideable = (T == PieceType::QUEEN);

template<PieceType T>
concept Slideable = SimplySlideable<T> || CompoundSlideable<T>;

template<PieceType T>
concept Jumpable = (T == PieceType::KNIGHT) || (T == PieceType::KING);

enum class PieceColour {
  BLACK = 0,
  WHITE,
  NULL_COLOUR
};

File fileIndex(const Square& squareIndex);
File fileIndex(char algebraicFile);
File fileIndex(std::string_view algebraic_square);

Rank rankIndex(const Square& squareIndex);
Rank rankIndex(char algebraicRank);
Rank rankIndex(std::string_view algebraicSquare);

Square squareIndex(File fileIndex, Rank rankIndex);
Square squareIndex(std::string_view square);

std::string toAlgebraic(Square square);
std::string toAlgebraic(File file_index, Rank rank_index);


// | F  | R  | O  | M  | -- | -> | T  | O  |
// | -- | -- | -- | -> | F  | L  | A  | GS |
// | MO | VE | D  | -> | CA | PT | UR | ED |
// |    |    |    |    |    |    |    |    |

// TODO this needs to be compacted into a 32 bit int
// from square 6 bits
// to square 6 bits
// flags 4 bits
// captured piece 4 bits?

using Move = uint32_t;
using MoveMask = Move;

static constexpr MoveMask FROM_MASK            { 0b0000'0000'0000'0000'0000'0011'1111 };
static constexpr MoveMask TO_MASK              { 0b0000'0000'0000'0000'1111'1100'0000 };
static constexpr MoveMask PAWN_MOVE_MASK       { 0b0000'0000'0000'0001'0000'0000'0000 };
static constexpr MoveMask DOUBLE_PUSH_MASK     { 0b0000'0000'0000'0010'0000'0000'0000 };
static constexpr MoveMask EP_MASK              { 0b0000'0000'0000'0100'0000'0000'0000 };
static constexpr MoveMask CAPTURE_MASK         { 0b0000'0000'0000'1000'0000'0000'0000 };
static constexpr MoveMask PROMOTION_FLAG_MASK  { 0b0000'0000'0001'0000'0000'0000'0000 };
static constexpr MoveMask CASTLE_MASK          { 0b0000'0000'0110'0000'0000'0000'0000 };
static constexpr MoveMask PROMOTION_MASK       { 0b0000'0011'1000'0000'0000'0000'0000 };
static constexpr MoveMask MOVED_PIECE_MASK     { 0b0001'1100'0000'0000'0000'0000'0000 };
static constexpr MoveMask CAPTURED_PIECE_MASK  { 0b1110'0000'0000'0000'0000'0000'0000 };

static constexpr MoveMask NOT_FROM_MASK{ ~FROM_MASK };
static constexpr MoveMask NOT_TO_MASK{ ~TO_MASK };
static constexpr MoveMask NOT_CASTLE_MASK{ ~CASTLE_MASK };
static constexpr MoveMask NOT_PROMOTION_MASK{ ~PROMOTION_MASK };
static constexpr MoveMask NOT_MOVED_PIECE_MASK{ ~MOVED_PIECE_MASK };
static constexpr MoveMask NOT_CAPTURED_PIECE_MASK{ ~CAPTURED_PIECE_MASK };

static constexpr int FROM_OFFSET{ 0 };
static constexpr int TO_OFFSET{ 6 };
static constexpr int CASTLE_OFFSET{ 17 };
static constexpr int PROMOTION_OFFSET{ 19 };
static constexpr int MOVED_PIECE_OFFSET{ 22 };
static constexpr int CAPTURED_PIECE_OFFSET{ 25 };

enum class MoveFlag
{
  PAWN_MOVE,
  DOUBLE_PUSH,
  EP,
  CAPTURE,
  PROMOTION,
};

enum class MoveValue
{
  FROM,
  TO,
  CASTLE,
  PROMOTION,
  MOVED,
  CAPTURED,
};

static constexpr uint8_t KINGSIDE_CASTLE_VALUE{ 0b01 };
static constexpr uint8_t QUEENSIDE_CASTLE_VALUE{ 0b10 };

template<typename T>
static constexpr bool failed{ false };

template<MoveFlag flag>
consteval auto getMoveMask() -> MoveMask
{
  if constexpr (flag == MoveFlag::PAWN_MOVE) return PAWN_MOVE_MASK;
  else if constexpr (flag == MoveFlag::DOUBLE_PUSH) return DOUBLE_PUSH_MASK;
  else if constexpr (flag == MoveFlag::EP) return EP_MASK;
  else if constexpr (flag == MoveFlag::CAPTURE) return CAPTURE_MASK;
  else if constexpr (flag == MoveFlag::PROMOTION) return PROMOTION_FLAG_MASK;
  else static_assert(failed<MoveFlag>, "Unknown move flag");
}

template<MoveValue value>
consteval auto getValueMask() -> MoveMask
{
  if constexpr (value == MoveValue::FROM) return FROM_MASK;
  else if constexpr (value == MoveValue::TO) return TO_MASK;
  else if constexpr (value == MoveValue::CASTLE) return CASTLE_MASK;
  else if constexpr (value == MoveValue::PROMOTION) return PROMOTION_MASK;
  else if constexpr (value == MoveValue::MOVED) return MOVED_PIECE_MASK;
  else if constexpr (value == MoveValue::CAPTURED) return CAPTURED_PIECE_MASK;
  else static_assert(failed<MoveValue>, "Unknown move value");
}

template<MoveValue value>
consteval auto getNotValueMask() -> MoveMask
{
  if constexpr (value == MoveValue::FROM) return NOT_FROM_MASK;
  else if constexpr (value == MoveValue::TO) return NOT_TO_MASK;
  else if constexpr (value == MoveValue::CASTLE) return NOT_CASTLE_MASK;
  else if constexpr (value == MoveValue::PROMOTION) return NOT_PROMOTION_MASK;
  else if constexpr (value == MoveValue::MOVED) return NOT_MOVED_PIECE_MASK;
  else if constexpr (value == MoveValue::CAPTURED) return NOT_CAPTURED_PIECE_MASK;
  else static_assert(failed<MoveValue>, "Unknown move value");
}

template<MoveValue value>
consteval auto getValueOffset() -> int
{
  if constexpr (value == MoveValue::FROM) return FROM_OFFSET;
  else if constexpr (value == MoveValue::TO) return TO_OFFSET;
  else if constexpr (value == MoveValue::CASTLE) return CASTLE_OFFSET;
  else if constexpr (value == MoveValue::PROMOTION) return PROMOTION_OFFSET;
  else if constexpr (value == MoveValue::MOVED) return MOVED_PIECE_OFFSET;
  else if constexpr (value == MoveValue::CAPTURED) return CAPTURED_PIECE_OFFSET;
  else static_assert(failed<MoveValue>, "Unknown move value");
}

template<MoveFlag flag>
inline constexpr auto getMoveFlag(Move move) -> bool
{
  return (move & getMoveMask<flag>());
}

template<MoveFlag flag>
inline constexpr void setMoveFlag(Move& move)
{
  move |= getMoveMask<flag>();
}

template<MoveFlag... flags>
inline constexpr void setMoveFlags(Move& move)
{
  (..., setMoveFlag<flags>(move));
}

inline constexpr auto isPawnMove(Move move) -> bool
{
  return getMoveFlag<MoveFlag::PAWN_MOVE>(move);
}

inline constexpr auto isDoublePush(Move move) -> bool
{
  return getMoveFlag<MoveFlag::DOUBLE_PUSH>(move);
}

inline constexpr auto isCapture(Move move) -> bool
{
  return getMoveFlag<MoveFlag::CAPTURE>(move);
}

inline constexpr auto isEnPassant(Move move) -> bool
{
  return getMoveFlag<MoveFlag::EP>(move);
}

inline constexpr auto isPromotion(Move move) -> bool
{
  return getMoveFlag<MoveFlag::PROMOTION>(move);
}

template<MoveValue toSet, typename Value>
inline constexpr auto setValue(Move& move, Value value)
{
  auto notValue = move & getNotValueMask<toSet>();
  move = (notValue | ((value << getValueOffset<toSet>()) & getValueMask<toSet>()));
}

template<MoveValue toGet, typename T>
inline constexpr auto getValue(Move move) -> T
{
  return static_cast<T>(((move & getValueMask<toGet>()) >> getValueOffset<toGet>()));
}

inline constexpr auto from(Move move) -> Square
{
  return getValue<MoveValue::FROM, Square>(move);
}

inline constexpr void setFrom(Move& move, Square square)
{
  setValue<MoveValue::FROM>(move, square);
}

inline constexpr auto to(Move move) -> Square
{
  return getValue<MoveValue::TO, Square>(move);
}

inline constexpr void setTo(Move& move, Square square)
{
  setValue<MoveValue::TO>(move, square);
}

inline constexpr auto pieceTypeToInt(PieceType type) -> uint8_t
{
  return (static_cast<uint8_t>(type) & 0x0F);
}

inline constexpr auto moved(Move move) -> PieceType
{
  return getValue<MoveValue::MOVED, PieceType>(move);
}

inline constexpr void setMoved(Move& move, PieceType type)
{
  if (type == PieceType::PAWN)
  {
    setMoveFlag<MoveFlag::PAWN_MOVE>(move);
  }

  setValue<MoveValue::MOVED>(move, pieceTypeToInt(type));
}

inline constexpr auto captured(Move move) -> PieceType
{
  return getValue<MoveValue::CAPTURED, PieceType>(move);
}

inline constexpr void setCaptured(Move& move, PieceType type)
{
  setValue<MoveValue::CAPTURED>(move, pieceTypeToInt(type));
}

inline constexpr auto promotion(Move move) -> PieceType
{
  return getValue<MoveValue::PROMOTION, PieceType>(move);
}

inline constexpr void setPromotion(Move& move, PieceType type)
{
  setValue<MoveValue::PROMOTION>(move, pieceTypeToInt(type));
}

inline constexpr void setKingSideCastle(Move& move)
{
  setValue<MoveValue::CASTLE>(move, KINGSIDE_CASTLE_VALUE);
}

inline constexpr auto isKingSideCastle(Move move) -> bool
{
  return (getValue<MoveValue::CASTLE, uint32_t>(move) == KINGSIDE_CASTLE_VALUE);
}

inline constexpr void setQueenSideCastle(Move& move)
{
  setValue<MoveValue::CASTLE>(move, QUEENSIDE_CASTLE_VALUE);
}

inline constexpr auto isQueenSideCastle(Move move) -> bool
{
  return (getValue<MoveValue::CASTLE, uint32_t>(move) == QUEENSIDE_CASTLE_VALUE);
}

inline auto isCastle(Move move) -> bool
{
  return (getValue<MoveValue::CASTLE, uint32_t>(move) != 0);
}

inline auto toAlgebraic(Move move) -> std::string
{
  return toAlgebraic(from(move)) + toAlgebraic(to(move));
}

} // namespace yak

#endif // YAK_TYPES_H_
