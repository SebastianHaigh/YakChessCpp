#ifndef MOVE_H_
#define MOVE_H_

#include "types.h"

namespace yak::move {

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
  move.doublePush = true;
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
  move.capture = true;
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
  move.capture = true;
  move.enPassant = true;
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
  move.promotion = type;
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
  move.capture = true;
  move.promotion = type;
  return move;
};

inline auto makeKingsideCastle() -> Move
{
  Move move;
  move.castle = PieceType::KING;
  return move;
};

inline auto makeQueensideCastle() -> Move
{
  Move move;
  move.castle = PieceType::QUEEN;
  return move;
};

} // namespace yak::move

#endif // MOVE_H_

