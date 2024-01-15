#ifndef YAK_PIECES_H_
#define YAK_PIECES_H_

#include "bitboard.h"
#include "attackmaps.h"

#include <iostream>
#include <vector>
#include <memory>
#include <stack>
#include <stdint.h>
#include <stdio.h>

enum class PieceType {
  PAWN = 0,
  KNIGHT,
  BISHOP,
  ROOK,
  QUEEN,
  KING,
  NULL_PIECE
};

enum class PieceColour {
  BLACK = 0,
  WHITE,
  NULL_COLOUR
};

namespace yak {

/** faster_pieces namespace for benchmark testing. */
namespace piece {

template<PieceType T>
std::string to_string()
{
  if (T == PieceType::PAWN) return "PAWN";
  if (T == PieceType::KNIGHT) return "KNIGHT";
  if (T == PieceType::BISHOP) return "BISHOP";
  if (T == PieceType::ROOK) return "ROOK";
  if (T == PieceType::QUEEN) return "QUEEN";
  if (T == PieceType::KING) return "KING";
  return "NULL";
}

/**
 * \brief Static table of jumping piece attacks.
 */
template<PieceType T, Square S = A1, Bitboard... B>
struct jump_map : jump_map<T,
                           static_cast<Square>(S + 1),
                           B...,
                           (T == PieceType::KNIGHT) ?
                           attackmap::knight_map<S>::value :
                           (T == PieceType::KING) ?
                           attackmap::king_map<S>::value :
                           Bitboard{0}>
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

/**
 * \brief Target squares of a pawn single push.
 * \tparam COLOUR - the colour of the pawns.
 * \param[in] source - the source squares for which to find the targets.
 * \return The Bitboard of the target squares.
 */
template<PieceColour C>
Bitboard pawnSinglePushTarget(Bitboard source)
{
  return (C == PieceColour::WHITE) ? bitboard::shift<Direction::NORTH>(source) :
         (C == PieceColour::BLACK) ? bitboard::shift<Direction::SOUTH>(source) : source;
}

/**
 * \brief Source squares of a pawn single push.
 * \tparam COLOUR - the colour of the pawns.
 * \param[in] target - the target squares for which to find the sources.
 * \return The Bitboard of the source squares.
 */
template<PieceColour C>
Bitboard pawnSinglePushSource(Bitboard target)
{
  return (C == PieceColour::WHITE) ? bitboard::shift<Direction::SOUTH>(target) :
         (C == PieceColour::BLACK) ? bitboard::shift<Direction::NORTH>(target) : target;
}

/**
 * \brief The target squares for a double push from the starting rank.
 * \tparam COLOUR - the colour of the pawns.
 * \return The Bitboard of the target squares.
 */
template<PieceColour C>
Bitboard pawnDoublePushTarget()
{
  return (C == PieceColour::WHITE) ? bitboard::RANK_4 :
         (C == PieceColour::BLACK) ? bitboard::RANK_5 : bitboard::EMPTY;
}

/**
 * \brief Source squares of pawns attacking to the west.
 * \tparam COLOUR - the colour of the pawns.
 * \param[in] target - the target squares for which to find the sources.
 * \return The Bitboard of the source squares.
 */
template<PieceColour C>
Bitboard pawnWestAttackSource(Bitboard target)
{
  return (C == PieceColour::WHITE) ? bitboard::shift<Direction::SOUTH_EAST>(target) :
         (C == PieceColour::BLACK) ? bitboard::shift<Direction::NORTH_EAST>(target) : target;
}

/**
 * \brief Target squares of pawns attacking to the west.
 * \tparam COLOUR - the colour of the pawns.
 * \param[in] source - the source squares for which to find the targets.
 * \return The Bitboard of the target squares.
 */
template<PieceColour C>
Bitboard pawnWestAttackTarget(Bitboard source)
{
  return (C == PieceColour::WHITE) ? bitboard::shift<Direction::NORTH_WEST>(source) :
         (C == PieceColour::BLACK) ? bitboard::shift<Direction::SOUTH_WEST>(source) : source;
}

/**
 * \brief Target squares of pawns attacking to the east.
 * \tparam COLOUR - the colour of the pawns.
 * \param[in] source - the source squares for which to find the targets.
 * \return The Bitboard of the target squares.
 */
template<PieceColour C>
Bitboard pawn_east_attack_source(Bitboard target)
{
  return (C == PieceColour::WHITE) ? bitboard::shift<Direction::SOUTH_WEST>(target) :
         (C == PieceColour::BLACK) ? bitboard::shift<Direction::NORTH_WEST>(target) : target;
}

/**
 * \brief Target squares of pawns attacking to the east.
 * \tparam COLOUR - the colour of the pawns.
 * \param[in] source - the source squares for which to find the targets.
 * \return The Bitboard of the target squares.
 */
template<PieceColour C>
Bitboard pawn_east_attack_target(Bitboard source)
{
  return (C == PieceColour::WHITE) ? bitboard::shift<Direction::NORTH_EAST>(source) :
         (C == PieceColour::BLACK) ? bitboard::shift<Direction::SOUTH_EAST>(source) : source;
}

template<PieceColour C>
Bitboard promotablePawns(Bitboard pawn_positions)
{
  return (C == PieceColour::WHITE) ? (bitboard::RANK_7 & pawn_positions) :
         (C == PieceColour::BLACK) ? (bitboard::RANK_2 & pawn_positions) : bitboard::EMPTY;
}

template<PieceColour C>
Bitboard non_promotable_pawns(Bitboard pawn_positions)
{
  return ~promotablePawns<C>(pawn_positions) & pawn_positions;
}

// TODO this needs to be compacted into a 32 bit int
// from square 6 bits
// to square 6 bits
// flags 4 bits
// captured piece 4 bits?
struct Move
{
  Square from{NULL_SQUARE};
  Square to{NULL_SQUARE};
  bool capture = false;
  PieceType capturePiece = PieceType::NULL_PIECE;
  bool en_passant = false;
  bool double_push = false;
  bool pawn_move = false;
  Square ep_target = NULL_SQUARE;
  PieceType castle = PieceType::NULL_PIECE;
  PieceType promotion = PieceType::NULL_PIECE;
  std::string to_algebraic() const
  {
    return bitboard::to_algebraic(from) + bitboard::to_algebraic(to);
  }
};

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
  move.double_push = true;
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
inline Move make_ep_capture(Square from, Square to)
{
  Move move;
  move.from = from;
  move.to = to;
  move.capture = true;
  move.en_passant = true;
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

template<PieceColour C, bool PROMOTIONS>
inline void generate_pawn_single_pushes(Move *move_list,
                                        int &move_counter,
                                        Bitboard pawn_positions,
                                        Bitboard empty_squares)
{
  pawn_positions = PROMOTIONS ? promotablePawns<C>(pawn_positions) : non_promotable_pawns<C>(pawn_positions);
  Bitboard sources = pawnSinglePushSource<C>(empty_squares) & pawn_positions;
  Bitboard targets = pawnSinglePushTarget<C>(sources);
  while (sources && !PROMOTIONS)
  {
    *move_list++ = makeQuiet(bitboard::popLS1B(sources), bitboard::popLS1B(targets));
    move_counter++;
  }

  while (sources && PROMOTIONS)
  {
    Square from = bitboard::popLS1B(sources);
    Square to = bitboard::popLS1B(targets);
    *move_list++ = makeQuietPromotion(from, to, PieceType::KNIGHT);
    *move_list++ = makeQuietPromotion(from, to, PieceType::BISHOP);
    *move_list++ = makeQuietPromotion(from, to, PieceType::ROOK);
    *move_list++ = makeQuietPromotion(from, to, PieceType::QUEEN);
    move_counter += 4;
  }
}

template<PieceColour C, bool PROMOTIONS>
inline void generatePawnDoublePushes(Move *moveList,
                                     int &moveCounter,
                                     Bitboard pawnPositions,
                                     Bitboard emptySquares)
{
  Bitboard targets = pawnDoublePushTarget<C>() & emptySquares;
  Bitboard sources = pawnSinglePushSource<C>(targets) & emptySquares;
  sources = pawnSinglePushSource<C>(sources) & pawnPositions;
  while (sources)
  {
    *moveList++ = makeDoublePush(bitboard::popLS1B(sources), bitboard::popLS1B(targets));
    moveCounter++;
  }

}

template<PieceColour C, bool PROMOTIONS>
inline void generatePawnWestCaptures(Move *moveList,
                                     int &moveCounter,
                                     Bitboard pawnPositions,
                                     Bitboard opponentPieces)
{
  pawnPositions = PROMOTIONS ? promotablePawns<C>(pawnPositions) : non_promotable_pawns<C>(pawnPositions);
  Bitboard sources = pawnWestAttackSource<C>(opponentPieces) & pawnPositions;
  Bitboard targets = pawnWestAttackTarget<C>(sources);
  while (sources && !PROMOTIONS)
  {
    *moveList = makeCapture(bitboard::popLS1B(sources), bitboard::popLS1B(targets));
    moveCounter++;
  }

  while (sources && PROMOTIONS)
  {
    Square from = bitboard::popLS1B(sources);
    Square to = bitboard::popLS1B(targets);
    *moveList++ = makeCapturePromotion(from, to, PieceType::KNIGHT);
    *moveList++ = makeCapturePromotion(from, to, PieceType::BISHOP);
    *moveList++ = makeCapturePromotion(from, to, PieceType::ROOK);
    *moveList++ = makeCapturePromotion(from, to, PieceType::QUEEN);
    moveCounter += 4;
  }
}

template<PieceColour C, bool PROMOTIONS>
inline void generate_pawn_east_captures(Move *move_list,
                                        int &move_counter,
                                        Bitboard pawn_positions,
                                        Bitboard opponent_pieces)
{
  pawn_positions = PROMOTIONS ? promotablePawns<C>(pawn_positions) : non_promotable_pawns<C>(pawn_positions);
  Bitboard sources = pawn_east_attack_source<C>(opponent_pieces) & pawn_positions;
  Bitboard targets = pawn_east_attack_target<C>(sources);
  while (sources && !PROMOTIONS)
  {
    *move_list++ = makeCapture(bitboard::popLS1B(sources), bitboard::popLS1B(targets));
    move_counter++;
  }

  while (sources && PROMOTIONS)
  {
    Square from = bitboard::popLS1B(sources);
    Square to = bitboard::popLS1B(targets);
    *move_list++ = makeCapturePromotion(from, to, PieceType::KNIGHT);
    *move_list++ = makeCapturePromotion(from, to, PieceType::BISHOP);
    *move_list++ = makeCapturePromotion(from, to, PieceType::ROOK);
    *move_list++ = makeCapturePromotion(from, to, PieceType::QUEEN);
    move_counter += 4;
  }
}

template<PieceColour C>
void generate_pawn_moves(Move *moveList,
                       int &moveCounter,
                       Bitboard pawnPositions,
                       Bitboard emptySquares,
                       Bitboard opponentPieces)
{
  /* NOT PROMOTIONS             */
  /* ----------------------------- */
  generate_pawn_single_pushes<C, false>(&moveList[moveCounter], moveCounter, pawnPositions, emptySquares);
  generatePawnDoublePushes<C, false>(&moveList[moveCounter], moveCounter, pawnPositions, emptySquares);
  generatePawnWestCaptures<C, false>(&moveList[moveCounter], moveCounter, pawnPositions, opponentPieces);
  generate_pawn_east_captures<C, false>(&moveList[moveCounter], moveCounter, pawnPositions, opponentPieces);

  /* PROMOTIONS                    */
  /* ----------------------------- */
  /* generate_pawn_single_pushes<C, true>(&moveList[moveCounter], moveCounter, pawnPositions, emptySquares); */
  /* generatePawnWestCaptures<C, true>(&moveList[moveCounter], moveCounter, pawnPositions, opponentPieces); */
  /* generate_pawn_east_captures<C, true>(&moveList[moveCounter], moveCounter, pawnPositions, opponentPieces); */
};

template<PieceColour C>
void generateEpCaptures(Move *moveList,
                        int &moveCounter,
                        Bitboard pawnPositions,
                        Bitboard epTarget)
{
  Bitboard sources = pawnWestAttackSource<C>(epTarget) & pawnPositions;
  Bitboard targets = pawnWestAttackTarget<C>(sources);
  while (sources)
  {
    *moveList++ = make_ep_capture(bitboard::popLS1B(sources), bitboard::popLS1B(targets));
    moveCounter++;
  }

  sources = pawn_east_attack_source<C>(epTarget) & pawnPositions;
  targets = pawn_east_attack_target<C>(sources);
  while (sources)
  {
    *moveList++ = make_ep_capture(bitboard::popLS1B(sources), bitboard::popLS1B(targets));
    moveCounter++;
  }
};

void generate_sliding_piece_moves(const attackmap::RookMap &,
                                  Move *move_list,
                                  int &move_counter,
                                  Bitboard piece_positions,
                                  Bitboard empty_squares,
                                  Bitboard opponent_pieces);

void generateSlidingPieceMoves(const attackmap::BishopMap &,
                               Move *moveList,
                               int &moveCounter,
                               Bitboard piecePositions,
                               Bitboard emptySquares,
                               Bitboard opponentPieces);

void generateSlidingPieceMoves(const attackmap::QueenMap &,
                               Move *moveList,
                               int &moveCounter,
                               Bitboard piecePositions,
                               Bitboard emptySquares,
                               Bitboard opponentPieces);

// Can this be moved into the board?
// TODO I need to update this method so that it can save the piece to be capture if there is a capture
template<PieceType T>
int generatePieceMoves(Move *moveList,
                       Bitboard piecePositions,
                       Bitboard emptySquares,
                       Bitboard opponentPieces)
{
  int moveCounter{ 0 };

  while (piecePositions)
  {
    Square from = bitboard::popLS1B(piecePositions);
    Bitboard atk_bb = (T == PieceType::KNIGHT) ? KnightMap::attacks(from) :
                      (T == PieceType::BISHOP) ? attackmap::BishopMap::attacks(from, ~emptySquares) :
                      (T == PieceType::ROOK) ? attackmap::RookMap::attacks(from, ~emptySquares) :
                      (T == PieceType::QUEEN) ? attackmap::QueenMap::attacks(from, ~emptySquares) :
                      (T == PieceType::KING) ? KingMap::attacks(from) : Bitboard{0};

    Bitboard quiet = atk_bb & emptySquares;
    while (quiet)
    {
      *moveList++ = makeQuiet(from, bitboard::popLS1B(quiet));
      ++moveCounter;
    }

    Bitboard capture = atk_bb & opponentPieces;

    while (capture)
    {
      *moveList++ = makeCapture(from, bitboard::popLS1B(capture));
      ++moveCounter;
    }
  }

  return moveCounter;
}

/**
 * \brief Get all of the squares attacked by pawns of a given COLOUR.
 * \tparam COLOUR - The colour of the pawns.
 * \param[in] pawn_positions - Bitboard of the pawn positions.
 * \return Bitboard of the squares attack by the pawns.
 */
template<PieceColour COLOUR>
Bitboard pawn_attacks(Bitboard pawn_positions)
{
  Bitboard attackBitboard = (COLOUR == PieceColour::BLACK) ?
    bitboard::shift<Direction::SOUTH_EAST>(pawn_positions) :
    bitboard::shift<Direction::NORTH_EAST>(pawn_positions);

  attackBitboard |= (COLOUR == PieceColour::BLACK) ?
    bitboard::shift<Direction::SOUTH_WEST>(pawn_positions) :
    bitboard::shift<Direction::NORTH_WEST>(pawn_positions);

  return attackBitboard;
}

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

} // namespace piece

} // namespace yak

namespace pieces {

PieceColour otherColour(PieceColour colour);
PieceType fenCharToPieceType(const char fenChar);
PieceColour fenCharToPieceColour(const char fenChar);
char pieceToFenChar(const PieceType& type, const PieceColour& colour);

class PawnTargets {
  // PawnTargets store pawn move target bitboards and their source bitboards.
private:
  Bitboard source;
  Bitboard target;

public:
  PawnTargets(Bitboard sources, Bitboard targets)
    : source(sources), target(targets) {};
  const Bitboard get_target() { return target; };
  const Bitboard get_source() { return source; };
};



} // namespace name


#endif // YAK_PIECES_H_
