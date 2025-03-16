#ifndef YAK_BOARD_H_
#define YAK_BOARD_H_

#include <string>
#include <string_view>
#include <vector>

#include "GameState.h"
#include <magic/MagicBitboards.hpp>
#include "move.hpp"
#include "pieces.h"
#include "bitboard.h"
#include "pawns.h"
#include "types.h"

namespace yak {

class Board
{
public:
  Board() = default;
  Board(std::string_view fen);

  Board(Board const&) = delete;
  Board& operator=(Board const&) = delete;

  bool reset(std::string_view fen);

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

  std::vector<Move> generateMoves();
  MoveResult makeMove(Move const& move);
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
  int generatePieceMoves(Move* moveList,
                         PieceColour colour);

private:
  /*!
   * \brief Executes a move on the underlying board representation.
   * \tparam C - Colour of the m_side on which to execute the move.
   * \param[in] move - The move to be executed.
   * \param[in] undo - If true this will process an undo move.
   */
  template<PieceColour C>
  MoveResult processMove(Move const& move, bool undo);

  /*!
   * \brief Executes a castling move on the underlying board representation.
   * \tparam C - Colour of the m_side on which to execute the move.
   * \tparam T - The m_side of the board on which to castle.
   * \param[in] move - The move to be executed.
   */
  template<PieceType T, PieceColour C>
  MoveResult processCastle();

  template<PieceColour C>
  MoveResult processEp(Move const& move);

  /*!
   * \brief Returns all of the squares attacked by pawns of a given colour.
   * \param[in] colour - The coloyr of the pawns.
   * \return The bitboard of all the attacked squares.
   */
  Bitboard pawnAttacks(PieceColour colour);

  /*!
   * \brief Returns all of the squares attacked by a single m_side.
   * \param[in] colour - The colour of the m_side.
   * \return A bitboard of all the attacked squares.
   */
  Bitboard allAttacks(PieceColour colour);

  template<PieceColour C, bool PROMOTIONS>
  int generatePawnSinglePushes(Move* move_list, Bitboard pawn_positions, Bitboard empty_squares);

  template<PieceColour C, bool PROMOTIONS>
  int generatePawnDoublePushes(Move* moveList, Bitboard pawnPositions, Bitboard emptySquares);

  template<PieceColour C, bool PROMOTIONS>
  int generatePawnWestCaptures(Move* moveList, Bitboard pawnPositions);

  template<PieceColour C, bool PROMOTIONS>
  int generatePawnEastCaptures(Move* moveList, Bitboard pawnPositions);

  template<PieceColour Colour>
  int generateEpCaptures(Move *moveList, Bitboard pawnPositions, Bitboard epTarget);

  template<PieceColour C>
  int generatePawnMoves(Move* moveList, Bitboard pawnPositions, Bitboard emptySquares);

  void generateCastlingMoves(std::vector<Move>& moves);
  bool parseFen(std::string_view fen);
  std::string rankToFen(Rank rank);
  std::string rankToBoardFen(Rank rank);

  Bitboard m_pieceTypeBitboard[6] { 0, 0, 0, 0, 0, 0 };
  Bitboard m_colourBitboard[2] { 0, 0 };

  GameStateManager m_state;

  piece::SlidingPieceMap<PieceType::ROOK> m_rookMap;
  piece::SlidingPieceMap<PieceType::BISHOP> m_bishopMap;

  Move m_psuedoLegalMoveListBuffer[sizeof(Move) * 330];
  int m_psudeoLegalMovePointer{ 0 };
};

template<PieceColour C>
Board::MoveResult Board::processMove(const Move &move, bool undo)
{
  // If the move to be processed is a castle then we can handle this here and then exit.
  if (isKingSideCastle(move))
  {
    return processCastle<PieceType::KING, C>();
  }
  else if (isQueenSideCastle(move))
  {
    return processCastle<PieceType::QUEEN, C>();
  }

  // Same for en passant
  if (isEnPassant(move))
  {
    return processEp<C>(move);
  }

  // Bitboards that will be used to update the board representation
  const Bitboard toBitboard = bitboard::createBitboard(to(move));
  const Bitboard fromBitboard = bitboard::createBitboard(from(move));
  const Bitboard fromToBitboard = toBitboard ^ fromBitboard;

  // TODO (haigh) Change get piece type on to maintain a map of all the piece positions
  const PieceType pieceToMove = moved(move);

  int colourToMove = static_cast<int>(C);
  int opposingColour = static_cast<int>(OppositeColour<C>);

  // Make the basic move.
  m_pieceTypeBitboard[static_cast<int>(pieceToMove)] ^= fromToBitboard;
  m_colourBitboard[colourToMove] ^= fromToBitboard;

  // If the move is a capture, remove the captured piece.
  if (isCapture(move))
  {
    PieceType pieceToCapture = captured(move);
    Bitboard captureSquare = (isEnPassant(move) ? m_state->epTarget() : toBitboard);

    m_pieceTypeBitboard[static_cast<int>(pieceToCapture)] ^= captureSquare;
    m_colourBitboard[opposingColour] ^= captureSquare;
  }

  // If the move is a promotion, swap the moved piece for a piece of the promoted type.
  if (isPromotion(move))
  {
    m_pieceTypeBitboard[static_cast<int>(PieceType::PAWN)] ^= toBitboard;
    m_colourBitboard[colourToMove] ^= toBitboard;
    m_pieceTypeBitboard[static_cast<int>(promotion(move))] ^= toBitboard;
    m_colourBitboard[colourToMove] ^= toBitboard;
  }

  return MoveResult::SUCCESS;
}

template<PieceColour C>
Board::MoveResult Board::processEp(const Move &move)
{
  const Bitboard toBitboard = bitboard::createBitboard(to(move));
  const Bitboard fromBitboard = bitboard::createBitboard(from(move));
  const Bitboard fromToBitboard = toBitboard ^ fromBitboard;
  m_pieceTypeBitboard[static_cast<int>(PieceType::PAWN)] ^= fromToBitboard;
  m_colourBitboard[static_cast<int>(C)] ^= fromToBitboard;

  // get the ep target square from move.to square.
  // This is better than using the Boards m_epSquare because
  // it will give the same result when the move is undo'd.
  Bitboard captureSquare = pawns::pawnSinglePushSource<C>(toBitboard);
  m_pieceTypeBitboard[static_cast<int>(PieceType::PAWN)] ^= captureSquare;
  m_colourBitboard[static_cast<int>(OppositeColour<C>)] ^= captureSquare;

  // TODO (haigh) Can this fail?
  return MoveResult::SUCCESS;
}

template<PieceType T, PieceColour C>
Board::MoveResult Board::processCastle()
{
  // Move the king
  Bitboard fromToBitboard = bitboard::KingCastleSource<C> ^ bitboard::KingCastleTarget<T, C>;
  m_pieceTypeBitboard[static_cast<int>(PieceType::KING)] ^= fromToBitboard;
  m_colourBitboard[static_cast<int>(C)] ^= fromToBitboard;

  // Move the rook
  fromToBitboard = bitboard::RookCastleSource<T, C> ^ bitboard::RookCastleTarget<T, C>;
  m_pieceTypeBitboard[static_cast<int>(PieceType::ROOK)] ^= fromToBitboard;
  m_colourBitboard[static_cast<int>(C)] ^= fromToBitboard;

  // TODO (haigh) Can this fail?
  return MoveResult::SUCCESS;
}

template<PieceType Type>
auto Board::generatePieceMoves(Move *moveList, PieceColour colour) -> int
{
  int moveCounter{ 0 };

  Bitboard piecePositions = getPosition(colour, Type);

  while (piecePositions)
  {
    Square from = bitboard::popLS1B(piecePositions);

    Bitboard atk_bb = (Type == PieceType::KNIGHT) ? piece::KnightMap::attacks(from) :
      (Type == PieceType::BISHOP) ? m_bishopMap.attacks(from, ~emptySquares()) :
      (Type == PieceType::ROOK) ? m_rookMap.attacks(from, ~emptySquares()) :
      (Type == PieceType::QUEEN) ? (m_bishopMap.attacks(from, ~emptySquares()) | m_rookMap.attacks(from, ~emptySquares())) :
      (Type == PieceType::KING) ? piece::KingMap::attacks(from) : Bitboard{0};

    Bitboard quiet = atk_bb & emptySquares();
    while (quiet)
    {
      *moveList = makeQuiet(from, bitboard::popLS1B(quiet), Type);
      ++moveList;
      ++moveCounter;
    }

    Bitboard capture = atk_bb & get_position(piece::otherColour(colour));
    while (capture)
    {
      Square captureSquare = bitboard::popLS1B(capture);
      *moveList = makeCapture(from, captureSquare, Type, getPieceTypeOn(captureSquare));
      ++moveList;
      ++moveCounter;
    }
  }

  return moveCounter;
}

template<PieceColour Colour, bool Promotions>
int Board::generatePawnSinglePushes(Move *moveList, Bitboard pawnPositions, Bitboard emptySquares)
{
  pawnPositions = Promotions ? pawns::promotablePawns<Colour>(pawnPositions) : pawns::nonPromotablePawns<Colour>(pawnPositions);
  Bitboard sources = pawns::pawnSinglePushSource<Colour>(emptySquares) & pawnPositions;
  Bitboard targets = pawns::pawnSinglePushTarget<Colour>(sources);

  int moveCounter{ 0 };

  if constexpr (Promotions)
  {
    while (sources)
    {
      const Square from = bitboard::popLS1B(sources);
      const Square to = bitboard::popLS1B(targets);
      *moveList++ = makeQuietPromotion(from, to, PieceType::KNIGHT);
      *moveList++ = makeQuietPromotion(from, to, PieceType::BISHOP);
      *moveList++ = makeQuietPromotion(from, to, PieceType::ROOK);
      *moveList++ = makeQuietPromotion(from, to, PieceType::QUEEN);
      moveCounter += 4;
    }
  }
  else
  {
    while (sources)
    {
      *moveList = makeQuiet(bitboard::popLS1B(sources), bitboard::popLS1B(targets), PieceType::PAWN);
      ++moveList;
      ++moveCounter;
    }
  }

  return moveCounter;
}

template<PieceColour Colour, bool Promotions>
int Board::generatePawnDoublePushes(Move* moveList, Bitboard pawnPositions, Bitboard emptySquares)
{
  Bitboard targets = pawns::pawnSinglePushTarget<Colour>(pawnPositions) & emptySquares;
  targets = pawns::pawnSinglePushTarget<Colour>(targets) & emptySquares & pawns::pawnDoublePushTarget<Colour>();

  // TODO (haigh) the sources could just use the known starting rank for this colour.
  Bitboard sources = pawns::pawnSinglePushSource<Colour>(targets);
  sources = pawns::pawnSinglePushSource<Colour>(sources);

  int moveCounter{ 0 };

  while (sources)
  {
    *moveList++ = makeDoublePush(bitboard::popLS1B(sources), bitboard::popLS1B(targets));
    moveCounter++;
  }

  return moveCounter;
}

template<PieceColour Colour, bool Promotions>
int Board::generatePawnWestCaptures(Move *moveList, Bitboard pawnPositions)
{
  Bitboard opponentPieces = get_position(OppositeColour<Colour>);
  pawnPositions = Promotions ? pawns::promotablePawns<Colour>(pawnPositions) : pawns::nonPromotablePawns<Colour>(pawnPositions);
  Bitboard sources = pawns::pawnWestAttackSource<Colour>(opponentPieces) & pawnPositions;
  Bitboard targets = pawns::pawnWestAttackTarget<Colour>(sources);

  int moveCounter{ 0 };

  while (sources && not Promotions)
  {
    Square targetSquare = bitboard::popLS1B(targets);
    *moveList = makeCapture(bitboard::popLS1B(sources), targetSquare, PieceType::PAWN, getPieceTypeOn(targetSquare));
    setMoved(*moveList, PieceType::PAWN);
    ++moveList;
    ++moveCounter;
  }

  while (sources && Promotions)
  {
    Square from{ bitboard::popLS1B(sources) };
    Square to{ bitboard::popLS1B(targets) };
    PieceType capturedPiece{ getPieceTypeOn(to) };

    *moveList++ = makeCapturePromotion(from, to, PieceType::KNIGHT, capturedPiece);
    *moveList++ = makeCapturePromotion(from, to, PieceType::BISHOP, capturedPiece);
    *moveList++ = makeCapturePromotion(from, to, PieceType::ROOK, capturedPiece);
    *moveList++ = makeCapturePromotion(from, to, PieceType::QUEEN, capturedPiece);

    moveCounter += 4;
  }

  return moveCounter;
}

template<PieceColour Colour, bool Promotions>
int Board::generatePawnEastCaptures(Move* moveList, Bitboard pawnPositions)
{
  Bitboard opponentPieces = get_position(OppositeColour<Colour>);
  pawnPositions = Promotions ? pawns::promotablePawns<Colour>(pawnPositions) : pawns::nonPromotablePawns<Colour>(pawnPositions);
  Bitboard sources = pawns::pawnEastAttackSource<Colour>(opponentPieces) & pawnPositions;
  Bitboard targets = pawns::pawnEastAttackTarget<Colour>(sources);

  int moveCounter{ 0 };

  while (sources && !Promotions)
  {
    Square targetSquare = bitboard::popLS1B(targets);
    *moveList = makeCapture(bitboard::popLS1B(sources), targetSquare, PieceType::PAWN, getPieceTypeOn(targetSquare));
    ++moveList;
    ++moveCounter;
  }

  while (sources && Promotions)
  {
    Square from{ bitboard::popLS1B(sources) };
    Square to{ bitboard::popLS1B(targets) };
    PieceType capturedPiece{ getPieceTypeOn(to) };

    *moveList++ = makeCapturePromotion(from, to, PieceType::KNIGHT, capturedPiece);
    *moveList++ = makeCapturePromotion(from, to, PieceType::BISHOP, capturedPiece);
    *moveList++ = makeCapturePromotion(from, to, PieceType::ROOK, capturedPiece);
    *moveList++ = makeCapturePromotion(from, to, PieceType::QUEEN, capturedPiece);
    moveCounter += 4;
  }

  return moveCounter;
}

template<PieceColour Colour>
int Board::generateEpCaptures(Move *moveList, Bitboard pawnPositions, Bitboard epTarget)
{
  Bitboard sources = pawns::pawnWestAttackSource<Colour>(epTarget) & pawnPositions;
  Bitboard targets = pawns::pawnWestAttackTarget<Colour>(sources);

  int moveCounter{ 0 };

  while (sources)
  {
    *moveList++ = makeEpCapture(bitboard::popLS1B(sources), bitboard::popLS1B(targets));
    moveCounter++;
  }

  sources = pawns::pawnEastAttackSource<Colour>(epTarget) & pawnPositions;
  targets = pawns::pawnEastAttackTarget<Colour>(sources);
  while (sources)
  {
    *moveList++ = makeEpCapture(bitboard::popLS1B(sources), bitboard::popLS1B(targets));
    moveCounter++;
  }

  return moveCounter;
};

template<PieceColour Colour>
int Board::generatePawnMoves(Move* moveList,
                             Bitboard pawnPositions,
                             Bitboard emptySquares)
{
  int moveCounter{ 0 };

  /* NOT PROMOTIONS             */
  /* ----------------------------- */
  moveCounter += generatePawnSinglePushes<Colour, false>(&moveList[moveCounter], pawnPositions, emptySquares);
  moveCounter += generatePawnDoublePushes<Colour, false>(&moveList[moveCounter], pawnPositions, emptySquares);
  moveCounter += generatePawnWestCaptures<Colour, false>(&moveList[moveCounter], pawnPositions);
  moveCounter += generatePawnEastCaptures<Colour, false>(&moveList[moveCounter], pawnPositions);

  /* PROMOTIONS                    */
  /* ----------------------------- */
  moveCounter += generatePawnSinglePushes<Colour, true>(&moveList[moveCounter], pawnPositions, emptySquares);
  moveCounter += generatePawnWestCaptures<Colour, true>(&moveList[moveCounter], pawnPositions);
  moveCounter += generatePawnEastCaptures<Colour, true>(&moveList[moveCounter], pawnPositions);

  return moveCounter;
};

} // namespace yak

#endif // YAK_BOARD_H_
