#ifndef MOVE_H_
#define MOVE_H_

#include "types.h"

namespace yak {

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

/**
 * \brief Constructs a quiet Move.
 * \param[in] from - The square from which to move.
 * \param[in] to - The square to move to.
 * \param[in] moved - The PieceType being moved.
 * \return The constructed quiet move.
 */
inline auto makeQuiet(Square from, Square to, PieceType moved) -> Move
{
  Move move;
  setFrom(move, from);
  setTo(move, to);
  setMoved(move, moved);
  return move;
}

/**
 * \brief Constructs a double pawn push.
 * \param[in] from - The square from which to move.
 * \param[in] to - The square to move to.
 * \return The constructed double push move.
 *
 * \note Use this when constructing double push moves to ensure
 * that the en passant target is properly set after making the move.
 */
inline auto makeDoublePush(Square from, Square to) -> Move
{
  Move move;
  setFrom(move, from);
  setTo(move, to);
  setMoved(move, PieceType::PAWN);
  setMoveFlag<MoveFlag::DOUBLE_PUSH>(move);
  return move;
}

/**
 * \brief Constructs a move with capture.
 * \param[in] from - The square from which to move.
 * \param[in] to - The square to move to.
 * \param[in] moved - The PieceType being moved.
 * \param[in] captured - The PieceType being captured.
 * \return The constructed capture move.
 */
inline auto makeCapture(Square from, Square to, PieceType moved, PieceType captured) -> Move
{
  Move move;
  setFrom(move, from);
  setTo(move, to);
  setMoved(move, moved);
  setCaptured(move, captured);
  setMoveFlag<MoveFlag::CAPTURE>(move);
  return move;
};

/**
 * \brief Constructs an en passant capture.
 * \param[in] from - The square from which to move.
 * \param[in] to - The square to move to.
 * \return The constructed en passant capture.
 */
inline auto makeEpCapture(Square from, Square to) -> Move
{
  Move move;
  setFrom(move, from);
  setTo(move, to);
  setMoved(move, PieceType::PAWN);
  setCaptured(move, PieceType::PAWN);
  setMoveFlags<MoveFlag::CAPTURE, MoveFlag::EP>(move);
  return move;
};

/**
 * \brief Constructs a quiet move with promotion.
 * \param[in] from - The square from which to move.
 * \param[in] to - The square to move to.
 * \param[in] type - The type of piece to promote to.
 * \return The constructed quiet promotion.
 */
inline auto makeQuietPromotion(Square from, Square to, PieceType type) -> Move
{
  Move move;
  setFrom(move, from);
  setTo(move, to);
  setMoved(move, PieceType::PAWN);
  setMoveFlag<MoveFlag::PROMOTION>(move);
  setPromotion(move, type);
  return move;
};

/**
 * \brief Constructs a capture with promotion.
 * \param[in] from - The square from which to move.
 * \param[in] to - The square to move to.
 * \param[in] type - The type of piece to promote to.
 * \param[in] captured - The PieceType of the captured piece.
 * \return The constructed capture promotion.
 */
inline auto makeCapturePromotion(Square from, Square to, PieceType type, PieceType captured) -> Move
{
  Move move;
  setFrom(move, from);
  setTo(move, to);
  setMoved(move, PieceType::PAWN);
  setCaptured(move, captured);
  setMoveFlags<MoveFlag::CAPTURE, MoveFlag::PROMOTION>(move);
  setPromotion(move, type);
  return move;
};

inline auto makeKingsideCastle() -> Move
{
  Move move;
  setKingSideCastle(move);
  return move;
};

inline auto makeQueensideCastle() -> Move
{
  Move move;
  setQueenSideCastle(move);
  return move;
};

} // namespace yak

#endif // MOVE_H_

