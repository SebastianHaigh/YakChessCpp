#ifndef YAK_BOARD_H_
#define YAK_BOARD_H_

#include <iostream>
#include <string>

#include "GameState.h"
#include "pieces.h"
#include "bitboard.h"

namespace yak {

template<PieceType T, PieceColour C>
struct KingCastleTarget {
  static constexpr Bitboard value{0};
};

template<> struct KingCastleTarget<PieceType::KING, PieceColour::WHITE> {
  static constexpr Bitboard value = bitboard::static_bitboard<G1>::value;
};

template<> struct KingCastleTarget<PieceType::KING, PieceColour::BLACK> {
  static constexpr Bitboard value = bitboard::static_bitboard<G8>::value;
};

template<> struct KingCastleTarget<PieceType::QUEEN, PieceColour::WHITE> {
  static constexpr Bitboard value = bitboard::static_bitboard<C1>::value;
};

template<> struct KingCastleTarget<PieceType::QUEEN, PieceColour::BLACK> {
  static constexpr Bitboard value = bitboard::static_bitboard<C8>::value;
};

template<PieceType T, PieceColour C>
struct RookCastleSource {
  static constexpr Bitboard value{0};
};

template<> struct RookCastleSource<PieceType::KING, PieceColour::WHITE> {
  static constexpr Bitboard value = bitboard::static_bitboard<H1>::value;
};

template<> struct RookCastleSource<PieceType::KING, PieceColour::BLACK> {
  static constexpr Bitboard value = bitboard::static_bitboard<H8>::value;
};

template<> struct RookCastleSource<PieceType::QUEEN, PieceColour::WHITE> {
  static constexpr Bitboard value = bitboard::static_bitboard<A1>::value;
};

template<> struct RookCastleSource<PieceType::QUEEN, PieceColour::BLACK> {
  static constexpr Bitboard value = bitboard::static_bitboard<A8>::value;
};

template<PieceType T, PieceColour C>
struct RookCastleTarget {
  static constexpr Bitboard value{0};
};

template<> struct RookCastleTarget<PieceType::KING, PieceColour::WHITE> {
  static constexpr Bitboard value = bitboard::static_bitboard<F1>::value;
};

template<> struct RookCastleTarget<PieceType::KING, PieceColour::BLACK> {
  static constexpr Bitboard value = bitboard::static_bitboard<F8>::value;
};

template<> struct RookCastleTarget<PieceType::QUEEN, PieceColour::WHITE> {
  static constexpr Bitboard value = bitboard::static_bitboard<D1>::value;
};

template<> struct RookCastleTarget<PieceType::QUEEN, PieceColour::BLACK> {
  static constexpr Bitboard value = bitboard::static_bitboard<D8>::value;
};

template<PieceColour C>
struct OppositeColour {
  static constexpr PieceColour value = PieceColour::NULL_COLOUR;
};

template<> struct OppositeColour<PieceColour::WHITE> {
  static constexpr PieceColour value = PieceColour::BLACK;
};

template<> struct OppositeColour<PieceColour::BLACK> {
  static constexpr PieceColour value = PieceColour::WHITE;
};

class Board
{
public:
  Board() = default;
  Board(const std::string &fen);

  Board(const Board&) = delete;
  Board& operator=(const Board&) = delete;

  PieceType getPieceTypeOn(Square square);
  PieceColour getPieceColourOn(Square square);

  Bitboard occupiedSquares();
  Bitboard emptySquares();

  enum class MoveResult
  {
    SUCCESS = 0,
    NO_PIECE_TO_MOVE,
    NO_PIECE_TO_CAPTURE,
    INVALID_TO_OR_FROM,
  };

  std::vector<piece::Move> generateMoves();
  MoveResult makeMove(const piece::Move &move);
  MoveResult undoMove();

  /**
   * \brief Check if the king of the current m_side to move is in check/
   * \return true if the king is in check, false otherwise.
   */
  bool isCheck();
  bool isCheck(PieceColour colour);
  bool isCheckmate();

  std::string toFen();

  Bitboard attacked_by(PieceColour colour)
  {
    return allAttacks(colour);
  }

  bool canKingSideCastle(PieceColour colour);
  bool canQueenSideCastle(PieceColour colour);

  /**
   * \brief Get the bitboard for a particular piece type (both colours).
   * \param[in] type - The type of piece.
   * \return A bitboard containing the piece positions.
   */
  Bitboard get_position(PieceType type);

  /**
   * \brief Get the bitboard for a particular colour.
   * \param[in] colour - The colour of the m_side.
   * \return A bitboard containing the piece positions.
   */
  Bitboard get_position(PieceColour colour);

  /**
   * \brief Get the bitboard for a particular colour and piece type.
   * \param[in] pieceColour - The colour of the m_side.
   * \param[in] pieceType - The type of piece.
   * \return A bitboard containing the piece positions.
   */
  Bitboard getPosition(PieceColour pieceColour, PieceType pieceType);

  template<PieceType T>
  int generatePieceMoves(piece::Move *moveList,
                         PieceColour colour);

private:
  Bitboard m_pieceTypeBitboard[6] = { 0, 0, 0, 0, 0, 0 };
  Bitboard m_colourBitboard[2] { 0, 0 };

  GameStateManager m_state;

  attackmap::RookMap m_rookAtks;      /* \brief Attack map for Rooks */
  attackmap::BishopMap m_bishopAtks;    /* \brief Attack map for Bishops */
  attackmap::QueenMap m_queenAtks;    /* \brief Attack map for Queens */

  /*!
   * \brief Executes a move on the underlying board representation.
   * \tparam C - Colour of the m_side on which to execute the move.
   * \param[in] move - The move to be executed.
   * \param[in] undo - If true this will process an undo move.
   */
  template<PieceColour C>
  MoveResult processMove(const piece::Move &move, bool undo);

  /*!
   * \brief Executes a castling move on the underlying board representation.
   * \tparam C - Colour of the m_side on which to execute the move.
   * \tparam T - The m_side of the board on which to castle.
   * \param[in] move - The move to be executed.
   */
  template<PieceType T, PieceColour C>
  MoveResult processCastle(const piece::Move &move);

  template<PieceColour C>
  MoveResult processEp(const piece::Move &move);

  /*!
   * \brief Returns all of the squares attacked by pawns of a given colour.
   * \param[in] colour - The coloyr of the pawns.
   * \return The bitboard of all the attacked squares.
   */
  Bitboard pawnAttacks(PieceColour colour);

  /*!
   * \brief Returns all of the squares attacked by a particular type of piece.
   * \param[in] pieceType - The type of piece.
   * \param[in] pieceColour - The colour of the piece.
   * \return A bitboard of all the attacked squares.
   */
  Bitboard pieceAttacks(PieceType pieceType, PieceColour pieceColour);

  /*!
   * \brief Returns all of the squares attacked by a single m_side.
   * \param[in] colour - The colour of the m_side.
   * \return A bitboard of all the attacked squares.
   */
  Bitboard allAttacks(PieceColour colour);

  template<PieceColour C, bool PROMOTIONS>
  void generatePawnSinglePushes(piece::Move *move_list,
                                int &move_counter,
                                Bitboard pawn_positions,
                                Bitboard empty_squares);

  template<PieceColour C, bool PROMOTIONS>
  void generatePawnDoublePushes(piece::Move *moveList,
                                int &moveCounter,
                                Bitboard pawnPositions,
                                Bitboard emptySquares);

  template<PieceColour C, bool PROMOTIONS>
  void generatePawnWestCaptures(piece::Move *moveList,
                                int &moveCounter,
                                Bitboard pawnPositions);

  template<PieceColour C, bool PROMOTIONS>
  void generatePawnEastCaptures(piece::Move *moveList,
                                int &moveCounter,
                                Bitboard pawnPositions);

  template<PieceColour C>
  void generatePawnMoves(piece::Move *moveList,
                         int &moveCounter,
                         Bitboard pawnPositions,
                         Bitboard emptySquares);

  std::vector<piece::Move> generateCastlingMoves(std::vector<piece::Move> moves);
  void parseFen(const std::string &fen);
  std::string rankToFen(Rank rank);
  std::string rankToBoardFen(Rank rank);
};

template<PieceColour C>
Board::MoveResult Board::processMove(const piece::Move &move, bool undo)
{
  // If the move to be processed is a castle then we can handle this here and then exit.
  if (move.castle == PieceType::KING)
  {
    return processCastle<PieceType::KING, C>(move);
  }
  else if (move.castle == PieceType::QUEEN)
  {
    return processCastle<PieceType::QUEEN, C>(move);
  }

  /* if (move.to == move.from) return MoveResult::INVALID_TO_OR_FROM; */

  // Same for en passant
  if (move.en_passant)
  {
    return processEp<C>(move);
  }

  // Bitboards that will be used to update the board representation
  const Bitboard toBitboard = bitboard::toBitboard(move.to);
  const Bitboard fromBitboard = bitboard::toBitboard(move.from);
  const Bitboard fromToBitboard = toBitboard ^ fromBitboard;

  // TODO (haigh) Change get piece type on to maintain a map of all the piece positions
  const PieceType pieceToMove = getPieceTypeOn(undo ? move.to : move.from);

  if (pieceToMove == PieceType::NULL_PIECE) return MoveResult::NO_PIECE_TO_MOVE;

  int colourToMove = static_cast<int>(C);
  int opposingColour = static_cast<int>(OppositeColour<C>::value);

  // Make the basic move.
  m_pieceTypeBitboard[static_cast<int>(pieceToMove)] ^= fromToBitboard;
  m_colourBitboard[colourToMove] ^= fromToBitboard;

  // If the move is a capture, remove the captured piece.
  if (move.capture)
  {
    PieceType pieceToCapture = move.capturePiece;
    Bitboard captureSquare = (move.en_passant ? m_state->epTarget() : toBitboard);

    m_pieceTypeBitboard[static_cast<int>(pieceToCapture)] ^= captureSquare;
    m_colourBitboard[opposingColour] ^= captureSquare;
  }

  // If the move is a promotion, swap the moved piece for a piece of the promoted type.
  if (move.promotion != PieceType::NULL_PIECE)
  {
    m_pieceTypeBitboard[static_cast<int>(PieceType::PAWN)] ^= toBitboard;
    m_colourBitboard[colourToMove] ^= toBitboard;
    m_pieceTypeBitboard[static_cast<int>(move.promotion)] ^= toBitboard;
    m_colourBitboard[colourToMove] ^= toBitboard;
  }

  return MoveResult::SUCCESS;
}

template<PieceColour C>
Board::MoveResult Board::processEp(const piece::Move &move)
{
  const Bitboard toBitboard = bitboard::toBitboard(move.to);
  const Bitboard fromBitboard = bitboard::toBitboard(move.from);
  const Bitboard fromToBitboard = toBitboard ^ fromBitboard;
  m_pieceTypeBitboard[static_cast<int>(PieceType::PAWN)] ^= fromToBitboard;
  m_colourBitboard[static_cast<int>(C)] ^= fromToBitboard;

  // get the ep target square from move.to square.
  // This is better than using the Boards m_epSquare because
  // it will give the same result when the move is undo'd.
  Bitboard captureSquare = piece::pawnSinglePushSource<C>(toBitboard);
  m_pieceTypeBitboard[static_cast<int>(PieceType::PAWN)] ^= captureSquare;
  m_colourBitboard[static_cast<int>(OppositeColour<C>::value)] ^= captureSquare;

  // TODO (haigh) Can this fail?
  return MoveResult::SUCCESS;
}

template<PieceType T, PieceColour C>
Board::MoveResult Board::processCastle(const piece::Move &move)
{
  if (m_state->can_castle<T>())
  {
    // Move the king
    Bitboard fromToBitboard = getPosition(C, PieceType::KING) ^ KingCastleTarget<T, C>::value;
    m_pieceTypeBitboard[static_cast<int>(PieceType::KING)] ^= fromToBitboard;
    m_colourBitboard[static_cast<int>(C)] ^= fromToBitboard;

    // Move the rook
    fromToBitboard = RookCastleSource<T, C>::value ^ RookCastleTarget<T, C>::value;
    m_pieceTypeBitboard[static_cast<int>(PieceType::ROOK)] ^= fromToBitboard;
    m_colourBitboard[static_cast<int>(C)] ^= fromToBitboard;
  }

  // TODO (haigh) Can this fail?
  return MoveResult::SUCCESS;
}

template<PieceType T>
int Board::generatePieceMoves(piece::Move *moveList,
                              PieceColour colour)
{
  int moveCounter{ 0 };

  Bitboard piecePositions = getPosition(colour, T);

  while (piecePositions)
  {
    Square from = bitboard::popLS1B(piecePositions);
    Bitboard atk_bb = (T == PieceType::KNIGHT) ? piece::KnightMap::attacks(from) :
      (T == PieceType::BISHOP) ? attackmap::BishopMap::attacks(from, ~emptySquares()) :
      (T == PieceType::ROOK) ? attackmap::RookMap::attacks(from, ~emptySquares()) :
      (T == PieceType::QUEEN) ? attackmap::QueenMap::attacks(from, ~emptySquares()) :
      (T == PieceType::KING) ? piece::KingMap::attacks(from) : Bitboard{0};

    Bitboard quiet = atk_bb & emptySquares();
    while (quiet)
    {
      *moveList++ = piece::makeQuiet(from, bitboard::popLS1B(quiet));
      ++moveCounter;
    }

    Bitboard capture = atk_bb & get_position(pieces::otherColour(colour));
    while (capture)
    {
      Square captureSquare = bitboard::popLS1B(capture);
      PieceType capturePiece = getPieceTypeOn(captureSquare);

      *moveList = piece::makeCapture(from, captureSquare);
      moveList->capturePiece = capturePiece;
      ++moveList;
      ++moveCounter;
    }
  }

  return moveCounter;
}

template<PieceColour C, bool PROMOTIONS>
void Board::generatePawnSinglePushes(piece::Move *moveList,
                                     int &moveCounter,
                                     Bitboard pawnPositions,
                                     Bitboard emptySquares)
{
  pawnPositions = PROMOTIONS ? piece::promotablePawns<C>(pawnPositions) : piece::non_promotable_pawns<C>(pawnPositions);
  Bitboard sources = piece::pawnSinglePushSource<C>(emptySquares) & pawnPositions;
  Bitboard targets = piece::pawnSinglePushTarget<C>(sources);
  while (sources && !PROMOTIONS)
  {
    *moveList++ = piece::makeQuiet(bitboard::popLS1B(sources), bitboard::popLS1B(targets));
    moveCounter++;
  }

  while (sources && PROMOTIONS)
  {
    const Square from = bitboard::popLS1B(sources);
    const Square to = bitboard::popLS1B(targets);
    *moveList++ = piece::makeQuietPromotion(from, to, PieceType::KNIGHT);
    *moveList++ = piece::makeQuietPromotion(from, to, PieceType::BISHOP);
    *moveList++ = piece::makeQuietPromotion(from, to, PieceType::ROOK);
    *moveList++ = piece::makeQuietPromotion(from, to, PieceType::QUEEN);
    moveCounter += 4;
  }
}

template<PieceColour C, bool PROMOTIONS>
void Board::generatePawnDoublePushes(piece::Move *moveList,
                                     int &moveCounter,
                                     Bitboard pawnPositions,
                                     Bitboard emptySquares)
{
  Bitboard targets = piece::pawnDoublePushTarget<C>() & emptySquares;
  Bitboard sources = piece::pawnSinglePushSource<C>(targets) & emptySquares;
  sources = piece::pawnSinglePushSource<C>(sources) & pawnPositions;
  while (sources)
  {
    *moveList++ = piece::makeDoublePush(bitboard::popLS1B(sources), bitboard::popLS1B(targets));
    moveCounter++;
  }
}

template<PieceColour C, bool PROMOTIONS>
void Board::generatePawnWestCaptures(piece::Move *moveList,
                                     int &moveCounter,
                                     Bitboard pawnPositions)
{
  Bitboard opponentPieces = get_position(OppositeColour<C>::value);
  pawnPositions = PROMOTIONS ? piece::promotablePawns<C>(pawnPositions) : piece::non_promotable_pawns<C>(pawnPositions);
  Bitboard sources = piece::pawnWestAttackSource<C>(opponentPieces) & pawnPositions;
  Bitboard targets = piece::pawnWestAttackTarget<C>(sources);

  while (sources && !PROMOTIONS)
  {
    Square targetSquare = bitboard::popLS1B(targets);
    *moveList = piece::makeCapture(bitboard::popLS1B(sources), targetSquare);
    moveList->capturePiece = getPieceTypeOn(targetSquare);
    ++moveList;
    ++moveCounter;
  }

  while (sources && PROMOTIONS)
  {
    Square from = bitboard::popLS1B(sources);
    Square to = bitboard::popLS1B(targets);
    *moveList++ = piece::makeCapturePromotion(from, to, PieceType::KNIGHT);
    *moveList++ = piece::makeCapturePromotion(from, to, PieceType::BISHOP);
    *moveList++ = piece::makeCapturePromotion(from, to, PieceType::ROOK);
    *moveList++ = piece::makeCapturePromotion(from, to, PieceType::QUEEN);
    moveCounter += 4;
  }
}

template<PieceColour C, bool PROMOTIONS>
void Board::generatePawnEastCaptures(piece::Move *moveList,
                                     int &moveCounter,
                                     Bitboard pawnPositions)
{
  Bitboard opponentPieces = get_position(OppositeColour<C>::value);
  pawnPositions = PROMOTIONS ? piece::promotablePawns<C>(pawnPositions) : piece::non_promotable_pawns<C>(pawnPositions);
  Bitboard sources = piece::pawn_east_attack_source<C>(opponentPieces) & pawnPositions;
  Bitboard targets = piece::pawn_east_attack_target<C>(sources);
  while (sources && !PROMOTIONS)
  {
    Square targetSquare = bitboard::popLS1B(targets);
    *moveList = piece::makeCapture(bitboard::popLS1B(sources), targetSquare);
    moveList->capturePiece = getPieceTypeOn(targetSquare);
    ++moveList;
    ++moveCounter;
  }

  while (sources && PROMOTIONS)
  {
    Square from = bitboard::popLS1B(sources);
    Square to = bitboard::popLS1B(targets);
    *moveList++ = piece::makeCapturePromotion(from, to, PieceType::KNIGHT);
    *moveList++ = piece::makeCapturePromotion(from, to, PieceType::BISHOP);
    *moveList++ = piece::makeCapturePromotion(from, to, PieceType::ROOK);
    *moveList++ = piece::makeCapturePromotion(from, to, PieceType::QUEEN);
    moveCounter += 4;
  }
}

template<PieceColour C>
void Board::generatePawnMoves(piece::Move *moveList,
                              int &moveCounter,
                              Bitboard pawnPositions,
                              Bitboard emptySquares)
{
  /* NOT PROMOTIONS             */
  /* ----------------------------- */
  generatePawnSinglePushes<C, false>(&moveList[moveCounter], moveCounter, pawnPositions, emptySquares);
  generatePawnDoublePushes<C, false>(&moveList[moveCounter], moveCounter, pawnPositions, emptySquares);
  generatePawnWestCaptures<C, false>(&moveList[moveCounter], moveCounter, pawnPositions);
  generatePawnEastCaptures<C, false>(&moveList[moveCounter], moveCounter, pawnPositions);

  /* PROMOTIONS                    */
  /* ----------------------------- */
  generatePawnSinglePushes<C, true>(&moveList[moveCounter], moveCounter, pawnPositions, emptySquares);
  generatePawnWestCaptures<C, true>(&moveList[moveCounter], moveCounter, pawnPositions);
  generatePawnEastCaptures<C, true>(&moveList[moveCounter], moveCounter, pawnPositions);
};
} // namespace yak

#endif // YAK_BOARD_H_
