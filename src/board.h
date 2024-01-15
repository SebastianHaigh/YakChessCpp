#ifndef YAK_BOARD_H_
#define YAK_BOARD_H_

#include <iostream>
#include <string>

#include "GameState.h"
#include "pieces.h"
#include "bitboard.h"

namespace yak {

template<PieceType T, PieceColour C>
struct KingCastleTarget
{
  static constexpr Bitboard value{0};
};

template<>
struct KingCastleTarget<PieceType::KING, PieceColour::WHITE>
{
  static constexpr Bitboard value = bitboard::static_bitboard<G1>::value;
};

template<>
struct KingCastleTarget<PieceType::KING, PieceColour::BLACK>
{
  static constexpr Bitboard value = bitboard::static_bitboard<G8>::value;
};

template<>
struct KingCastleTarget<PieceType::QUEEN, PieceColour::WHITE>
{
  static constexpr Bitboard value = bitboard::static_bitboard<C1>::value;
};

template<>
struct KingCastleTarget<PieceType::QUEEN, PieceColour::BLACK>
{
  static constexpr Bitboard value = bitboard::static_bitboard<C8>::value;
};

template<PieceType T, PieceColour C>
struct RookCastleSource
{
  static constexpr Bitboard value{0};
};

template<>
struct RookCastleSource<PieceType::KING, PieceColour::WHITE>
{
  static constexpr Bitboard value = bitboard::static_bitboard<H1>::value;
};

template<>
struct RookCastleSource<PieceType::KING, PieceColour::BLACK>
{
  static constexpr Bitboard value = bitboard::static_bitboard<H8>::value;
};

template<>
struct RookCastleSource<PieceType::QUEEN, PieceColour::WHITE>
{
  static constexpr Bitboard value = bitboard::static_bitboard<A1>::value;
};

template<>
struct RookCastleSource<PieceType::QUEEN, PieceColour::BLACK>
{
  static constexpr Bitboard value = bitboard::static_bitboard<A8>::value;
};

template<PieceType T, PieceColour C>
struct RookCastleTarget
{
  static constexpr Bitboard value{0};
};

template<>
struct RookCastleTarget<PieceType::KING, PieceColour::WHITE>
{
  static constexpr Bitboard value = bitboard::static_bitboard<F1>::value;
};

template<>
struct RookCastleTarget<PieceType::KING, PieceColour::BLACK>
{
  static constexpr Bitboard value = bitboard::static_bitboard<F8>::value;
};

template<>
struct RookCastleTarget<PieceType::QUEEN, PieceColour::WHITE>
{
  static constexpr Bitboard value = bitboard::static_bitboard<D1>::value;
};

template<>
struct RookCastleTarget<PieceType::QUEEN, PieceColour::BLACK>
{
  static constexpr Bitboard value = bitboard::static_bitboard<D8>::value;
};

template<PieceColour C>
struct OppositeColour
{
  static constexpr PieceColour value = PieceColour::NULL_COLOUR;
};

template<>
struct OppositeColour<PieceColour::WHITE>
{
  static constexpr PieceColour value = PieceColour::BLACK;
};

template<>
struct OppositeColour<PieceColour::BLACK>
{
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

  std::vector<piece::Move> generateMoves();
  void makeMove(PieceType type, PieceColour colour, Square from, Square to);
  //void makeMove(Move move);
  void makeMove(const piece::Move &move);
  void undoMove();

  /**
   * \brief Check if the king of the current m_side to move is in check/
   * \return true if the king is in check, false otherwise.
   */
  bool isCheck();
  bool isCheck(PieceColour colour);
  bool isCheckmate();

  std::string toFen();

  Bitboard epTarget()
  {
    return bitboard::toBitboard(m_state->epTargetSquare());
  }

  Square ep_target_square()
  {
    return m_state->epTargetSquare();
  }

  PieceColour to_move()
  {
    return m_state->sideToMove();
  }

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
  Bitboard get_position(PieceType type)
  {
    return m_pieceTypeBitboard[static_cast<int>(type)];
  }

  /**
   * \brief Get the bitboard for a particular colour.
   * \param[in] colour - The colour of the m_side.
   * \return A bitboard containing the piece positions.
   */
  Bitboard get_position(PieceColour colour)
  {
    return m_colourBitboard[static_cast<int>(colour)];
  }

  /**
   * \brief Get the bitboard for a particular colour and piece type.
   * \param[in] pieceColour - The colour of the m_side.
   * \param[in] pieceType - The type of piece.
   * \return A bitboard containing the piece positions.
   */
  Bitboard getPosition(PieceColour pieceColour, PieceType pieceType)
  {
    return get_position(pieceType) & get_position(pieceColour);
  }

private:
  Bitboard m_pieceTypeBitboard[6] = { 0, 0, 0, 0, 0, 0 };
  Bitboard m_colourBitboard[2] { 0, 0 };

  GameStateManager m_state;

  attackmap::RookMap m_rookAtks;      /* \brief Attack map for Rooks */
  attackmap::BishopMap m_bishopAtks;    /* \brief Attack map for Bishops */
  attackmap::QueenMap m_queenAtks;    /* \brief Attack map for Queens */

  /* \brief The piece captured on the last move, NULL_PIECE if last move was not a capture. */
  PieceType m_previousCapturedPiece = PieceType::NULL_PIECE;

  /*!
   * \brief Executes a move on the underlying board representation.
   * \tparam C - Colour of the m_side on which to execute the move.
   * \param[in] move - The move to be executed.
   * \param[in] undo - If true this will process an undo move.
   */
  template<PieceColour C>
  void processMove(const piece::Move &move, bool undo);

  /*!
   * \brief Executes a castling move on the underlying board representation.
   * \tparam C - Colour of the m_side on which to execute the move.
   * \tparam T - The m_side of the board on which to castle.
   * \param[in] move - The move to be executed.
   */
  template<PieceType T, PieceColour C>
  void processCastle(const piece::Move &move);

  template<PieceColour C>
  void processEp(const piece::Move &move);

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

  std::vector<piece::Move> generateCastlingMoves(std::vector<piece::Move> moves);
  void parseFen(const std::string &fen);
  std::string rankToFen(Rank rank);
};

template<PieceColour C>
void Board::processMove(const piece::Move &move, bool undo)
{

  // If the move to be processed is a castle then we can handle this here and then exit.
  if (move.castle == PieceType::KING)
    return processCastle<PieceType::KING, C>(move);
  else if (move.castle == PieceType::QUEEN)
    return processCastle<PieceType::QUEEN, C>(move);

  if (move.en_passant)
    return processEp<C>(move);

  // Bitboards that will be used to update the board representation
  Bitboard to_bitboard = bitboard::toBitboard(move.to);
  Bitboard from_bitboard = bitboard::toBitboard(move.from);
  Bitboard from_to_bitboard = to_bitboard ^ from_bitboard;

  // Determine the piece type that is to be moved.
  int piece_to_move = 0;
  Bitboard piece_check_bb = undo ? to_bitboard : from_bitboard;
  while (!(piece_check_bb & m_pieceTypeBitboard[piece_to_move]))
    piece_to_move++;

  // Determine the piece type to be captured
  int piece_to_capture = 0;
  if (move.capture && !undo)
  {
    // If this is a make rather than unmake we need to determine the type of the piece to capture.
    while (!(to_bitboard & m_pieceTypeBitboard[piece_to_capture]))
      piece_to_capture++;
    m_previousCapturedPiece = PieceType(piece_to_capture);
  }
  else if (move.capture && undo)
  {
    piece_to_capture = static_cast<int>(m_previousCapturedPiece);
  }

  int colour_to_move = static_cast<int>(C);
  int opposing_colour = static_cast<int>(OppositeColour<C>::value);

  // Make the basic move.
  m_pieceTypeBitboard[piece_to_move] ^= from_to_bitboard;
  m_colourBitboard[colour_to_move] ^= from_to_bitboard;

  // If the move is a capture, remove the captured piece.
  if (move.capture)
  {
    Bitboard capture_square = to_bitboard;
    if (move.en_passant)
      capture_square = m_state->epTarget();

    m_pieceTypeBitboard[piece_to_capture] ^= capture_square;
    m_colourBitboard[opposing_colour] ^= capture_square;
  }

  // If the move is a promotion, swap the moved piece for a piece of the promoted type.
  if (move.promotion != PieceType::NULL_PIECE)
  {
    m_pieceTypeBitboard[static_cast<int>(PieceType::PAWN)] ^= to_bitboard;
    m_colourBitboard[colour_to_move] ^= to_bitboard;
    m_pieceTypeBitboard[static_cast<int>(move.promotion)] ^= to_bitboard;
    m_colourBitboard[colour_to_move] ^= to_bitboard;
  }

  if (undo)
  {
    m_previousCapturedPiece = PieceType::NULL_PIECE;
  }

}

template<PieceColour C>
void Board::processEp(const piece::Move &move)
{
  Bitboard to_bitboard = bitboard::toBitboard(move.to);
  Bitboard from_bitboard = bitboard::toBitboard(move.from);
  Bitboard from_to_bitboard = to_bitboard ^ from_bitboard;
  m_pieceTypeBitboard[static_cast<int>(PieceType::PAWN)] ^= from_to_bitboard;
  m_colourBitboard[static_cast<int>(C)] ^= from_to_bitboard;

  // get the ep target square from move.to square.
  // This is better than using the Boards m_epSquare because
  // it will give the same result when the move is undo'd.
  Bitboard capture_square = piece::pawnSinglePushSource<C>(to_bitboard);
  m_pieceTypeBitboard[static_cast<int>(PieceType::PAWN)] ^= capture_square;
  m_colourBitboard[static_cast<int>(OppositeColour<C>::value)] ^= capture_square;

}

template<PieceType T, PieceColour C>
void Board::processCastle(const piece::Move &move)
{
  if (m_state->can_castle<T>())
  {
    // Move the king
    Bitboard from_to_bitboard = getPosition(C, PieceType::KING) ^ KingCastleTarget<T, C>::value;
    m_pieceTypeBitboard[static_cast<int>(PieceType::KING)] ^= from_to_bitboard;
    m_colourBitboard[static_cast<int>(C)] ^= from_to_bitboard;

    // Move the rook
    from_to_bitboard = RookCastleSource<T, C>::value ^ RookCastleTarget<T, C>::value;
    m_pieceTypeBitboard[static_cast<int>(PieceType::ROOK)] ^= from_to_bitboard;
    m_colourBitboard[static_cast<int>(C)] ^= from_to_bitboard;
  }
}

} // namespace yak

#endif // YAK_BOARD_H_
