#ifndef MOVE_H_
#define MOVE_H_

#include "types.h"

namespace yak::move {

/**
 * \brief Constructs a quiet Move.
 * \param[in] from - The square from which to move.
 * \param[in] to - The square to move to.
 * \return The constructed quiet move.
 */
inline Move makeQuiet(Square from, Square to)
{
  Move move;
  move.from = from;
  move.to = to;
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
inline Move makeDoublePush(Square from, Square to)
{
  Move move;
  move.from = from;
  move.to = to;
  move.doublePush = true;
  return move;
}

/**
 * \brief Constructs a move with capture.
 * \param[in] from - The square from which to move.
 * \param[in] to - The square to move to.
 * \return The constructed capture move.
 */
inline Move makeCapture(Square from, Square to)
{
  Move move;
  move.from = from;
  move.to = to;
  move.capture = true;
  return move;
};

/**
 * \brief Constructs an en passant capture.
 * \param[in] from - The square from which to move.
 * \param[in] to - The square to move to.
 * \return The constructed en passant capture.
 */
inline Move makeEpCapture(Square from, Square to)
{
  Move move;
  move.from = from;
  move.to = to;
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
inline Move makeQuietPromotion(Square from, Square to, PieceType type)
{
  Move move;
  move.from = from;
  move.to = to;
  move.promotion = type;
  return move;
};

/**
 * \brief Constructs a capture with promotion.
 * \param[in] from - The square from which to move.
 * \param[in] to - The square to move to.
 * \param[in] type - The type of piece to promote to.
 * \return The constructed capture promotion.
 */
inline Move makeCapturePromotion(Square from, Square to, PieceType type)
{
  Move move;
  move.from = from;
  move.to = to;
  move.capture = true;
  move.promotion = type;
  return move;
};

inline Move makeKingsideCastle()
{
  Move move;
  move.castle = PieceType::KING;
  return move;
};

inline Move makeQueensideCastle()
{
  Move move;
  move.castle = PieceType::QUEEN;
  return move;
};

} // namespace yak::move

#endif // MOVE_H_

