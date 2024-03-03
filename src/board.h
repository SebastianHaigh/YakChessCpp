#ifndef YAK_BOARD_H_
#define YAK_BOARD_H_

#include <string>
#include <string_view>
#include <vector>

#include "GameState.h"
#include "attackmaps.h"
#include "move.h"
#include "pieces.h"
#include "bitboard.h"
#include "pawns.h"
#include "types.h"

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
  Board(std::string_view fen);

  Board(Board const&) = delete;
  Board& operator=(Board const&) = delete;

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
  MoveResult processCastle(Move const& move);

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
  void generatePawnSinglePushes(Move* move_list,
                                int& move_counter,
                                Bitboard pawn_positions,
                                Bitboard empty_squares);

  template<PieceColour C, bool PROMOTIONS>
  void generatePawnDoublePushes(Move* moveList,
                                int& moveCounter,
                                Bitboard pawnPositions,
                                Bitboard emptySquares);

  template<PieceColour C, bool PROMOTIONS>
  void generatePawnWestCaptures(Move* moveList,
                                int& moveCounter,
                                Bitboard pawnPositions);

  template<PieceColour C, bool PROMOTIONS>
  void generatePawnEastCaptures(Move* moveList,
                                int& moveCounter,
                                Bitboard pawnPositions);

  template<PieceColour C>
  void generatePawnMoves(Move* moveList,
                         int& moveCounter,
                         Bitboard pawnPositions,
                         Bitboard emptySquares);

  std::vector<Move> generateCastlingMoves(std::vector<Move> moves);
  void parseFen(std::string_view fen);
  std::string rankToFen(Rank rank);
  std::string rankToBoardFen(Rank rank);

  Bitboard m_pieceTypeBitboard[6] = { 0, 0, 0, 0, 0, 0 };
  Bitboard m_colourBitboard[2] { 0, 0 };

  GameStateManager m_state;

  attackmap::RookMap m_rookAtks;      /* \brief Attack map for Rooks */
  attackmap::BishopMap m_bishopAtks;    /* \brief Attack map for Bishops */
  attackmap::QueenMap m_queenAtks;    /* \brief Attack map for Queens */

  piece::SlidingPieceMap<PieceType::ROOK> m_rookMap;
  piece::SlidingPieceMap<PieceType::BISHOP> m_bishopMap;

  Move m_psuedoLegalMoveListBuffer[sizeof(Move) * 330];
  int m_psudeoLegalMovePointer{ 0 };
};

template<PieceColour C>
Board::MoveResult Board::processMove(const Move &move, bool undo)
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
  if (move.enPassant)
  {
    return processEp<C>(move);
  }

  // Bitboards that will be used to update the board representation
  const Bitboard toBitboard = bitboard::createBitboard(to(move));
  const Bitboard fromBitboard = bitboard::createBitboard(from(move));
  const Bitboard fromToBitboard = toBitboard ^ fromBitboard;

  // TODO (haigh) Change get piece type on to maintain a map of all the piece positions
  const PieceType pieceToMove = moved(move);

  if (pieceToMove == PieceType::NULL_PIECE) return MoveResult::NO_PIECE_TO_MOVE;

  int colourToMove = static_cast<int>(C);
  int opposingColour = static_cast<int>(OppositeColour<C>::value);

  // Make the basic move.
  m_pieceTypeBitboard[static_cast<int>(pieceToMove)] ^= fromToBitboard;
  m_colourBitboard[colourToMove] ^= fromToBitboard;

  // If the move is a capture, remove the captured piece.
  if (move.capture)
  {
    PieceType pieceToCapture = captured(move);
    Bitboard captureSquare = (move.enPassant ? m_state->epTarget() : toBitboard);

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
  m_colourBitboard[static_cast<int>(OppositeColour<C>::value)] ^= captureSquare;

  // TODO (haigh) Can this fail?
  return MoveResult::SUCCESS;
}

template<PieceType T, PieceColour C>
Board::MoveResult Board::processCastle(const Move &move)
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

template<PieceType Type>
auto Board::generatePieceMoves(Move *moveList,
                               PieceColour colour) -> int
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
      *moveList = move::makeQuiet(from, bitboard::popLS1B(quiet), Type);
      ++moveList;
      ++moveCounter;
    }

    Bitboard capture = atk_bb & get_position(piece::otherColour(colour));
    while (capture)
    {
      Square captureSquare = bitboard::popLS1B(capture);
      *moveList = move::makeCapture(from, captureSquare, Type, getPieceTypeOn(captureSquare));
      ++moveList;
      ++moveCounter;
    }
  }

  return moveCounter;
}

template<PieceColour Colour, bool Promotions>
void Board::generatePawnSinglePushes(Move *moveList,
                                     int &moveCounter,
                                     Bitboard pawnPositions,
                                     Bitboard emptySquares)
{
  pawnPositions = Promotions ? pawns::promotablePawns<Colour>(pawnPositions) : pawns::nonPromotablePawns<Colour>(pawnPositions);
  Bitboard sources = pawns::pawnSinglePushSource<Colour>(emptySquares) & pawnPositions;
  Bitboard targets = pawns::pawnSinglePushTarget<Colour>(sources);

  while (sources && not Promotions)
  {
    *moveList = move::makeQuiet(bitboard::popLS1B(sources), bitboard::popLS1B(targets), PieceType::PAWN);
    ++moveList;
    ++moveCounter;
  }

  while (sources && Promotions)
  {
    const Square from = bitboard::popLS1B(sources);
    const Square to = bitboard::popLS1B(targets);
    *moveList++ = move::makeQuietPromotion(from, to, PieceType::KNIGHT);
    *moveList++ = move::makeQuietPromotion(from, to, PieceType::BISHOP);
    *moveList++ = move::makeQuietPromotion(from, to, PieceType::ROOK);
    *moveList++ = move::makeQuietPromotion(from, to, PieceType::QUEEN);
    moveCounter += 4;
  }
}

template<PieceColour Colour, bool Promotions>
void Board::generatePawnDoublePushes(Move* moveList,
                                     int& moveCounter,
                                     Bitboard pawnPositions,
                                     Bitboard emptySquares)
{
  Bitboard targets = pawns::pawnDoublePushTarget<Colour>() & emptySquares;
  Bitboard sources = pawns::pawnSinglePushSource<Colour>(targets) & emptySquares;
  sources = pawns::pawnSinglePushSource<Colour>(sources) & pawnPositions;
  while (sources)
  {
    *moveList++ = move::makeDoublePush(bitboard::popLS1B(sources), bitboard::popLS1B(targets));
    moveCounter++;
  }
}

template<PieceColour Colour, bool Promotions>
void Board::generatePawnWestCaptures(Move *moveList,
                                     int &moveCounter,
                                     Bitboard pawnPositions)
{
  Bitboard opponentPieces = get_position(OppositeColour<Colour>::value);
  pawnPositions = Promotions ? pawns::promotablePawns<Colour>(pawnPositions) : pawns::nonPromotablePawns<Colour>(pawnPositions);
  Bitboard sources = pawns::pawnWestAttackSource<Colour>(opponentPieces) & pawnPositions;
  Bitboard targets = pawns::pawnWestAttackTarget<Colour>(sources);

  while (sources && not Promotions)
  {
    Square targetSquare = bitboard::popLS1B(targets);
    *moveList = move::makeCapture(bitboard::popLS1B(sources), targetSquare, PieceType::PAWN, getPieceTypeOn(targetSquare));
    setMoved(*moveList, PieceType::PAWN);
    ++moveList;
    ++moveCounter;
  }

  while (sources && Promotions)
  {
    Square from{ bitboard::popLS1B(sources) };
    Square to{ bitboard::popLS1B(targets) };
    PieceType capturedPiece{ getPieceTypeOn(to) };

    *moveList++ = move::makeCapturePromotion(from, to, PieceType::KNIGHT, capturedPiece);
    *moveList++ = move::makeCapturePromotion(from, to, PieceType::BISHOP, capturedPiece);
    *moveList++ = move::makeCapturePromotion(from, to, PieceType::ROOK, capturedPiece);
    *moveList++ = move::makeCapturePromotion(from, to, PieceType::QUEEN, capturedPiece);

    moveCounter += 4;
  }
}

template<PieceColour Colour, bool Promotions>
void Board::generatePawnEastCaptures(Move* moveList,
                                     int& moveCounter,
                                     Bitboard pawnPositions)
{
  Bitboard opponentPieces = get_position(OppositeColour<Colour>::value);
  pawnPositions = Promotions ? pawns::promotablePawns<Colour>(pawnPositions) : pawns::nonPromotablePawns<Colour>(pawnPositions);
  Bitboard sources = pawns::pawnEastAttackSource<Colour>(opponentPieces) & pawnPositions;
  Bitboard targets = pawns::pawnEastAttackTarget<Colour>(sources);

  while (sources && !Promotions)
  {
    Square targetSquare = bitboard::popLS1B(targets);
    *moveList = move::makeCapture(bitboard::popLS1B(sources), targetSquare, PieceType::PAWN, getPieceTypeOn(targetSquare));
    ++moveList;
    ++moveCounter;
  }

  while (sources && Promotions)
  {
    Square from{ bitboard::popLS1B(sources) };
    Square to{ bitboard::popLS1B(targets) };
    PieceType capturedPiece{ getPieceTypeOn(to) };

    *moveList++ = move::makeCapturePromotion(from, to, PieceType::KNIGHT, capturedPiece);
    *moveList++ = move::makeCapturePromotion(from, to, PieceType::BISHOP, capturedPiece);
    *moveList++ = move::makeCapturePromotion(from, to, PieceType::ROOK, capturedPiece);
    *moveList++ = move::makeCapturePromotion(from, to, PieceType::QUEEN, capturedPiece);
    moveCounter += 4;
  }
}

template<PieceColour Colour>
void Board::generatePawnMoves(Move* moveList,
                              int& moveCounter,
                              Bitboard pawnPositions,
                              Bitboard emptySquares)
{
  /* NOT PROMOTIONS             */
  /* ----------------------------- */
  generatePawnSinglePushes<Colour, false>(&moveList[moveCounter], moveCounter, pawnPositions, emptySquares);
  generatePawnDoublePushes<Colour, false>(&moveList[moveCounter], moveCounter, pawnPositions, emptySquares);
  generatePawnWestCaptures<Colour, false>(&moveList[moveCounter], moveCounter, pawnPositions);
  generatePawnEastCaptures<Colour, false>(&moveList[moveCounter], moveCounter, pawnPositions);

  /* PROMOTIONS                    */
  /* ----------------------------- */
  generatePawnSinglePushes<Colour, true>(&moveList[moveCounter], moveCounter, pawnPositions, emptySquares);
  generatePawnWestCaptures<Colour, true>(&moveList[moveCounter], moveCounter, pawnPositions);
  generatePawnEastCaptures<Colour, true>(&moveList[moveCounter], moveCounter, pawnPositions);
};

} // namespace yak

#endif // YAK_BOARD_H_
