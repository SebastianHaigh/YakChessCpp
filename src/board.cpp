#include <board.h>

#include <stdexcept>
#include <cassert>
#include "move.h"
#include "pieces.h"
#include "attackmaps.h"
#include "generation.h"

namespace yak {

Board::Board(std::string_view fen)
{
  parseFen(fen);
}

void Board::parseFen(std::string_view fen)
{
  auto endOfPiecePlacement = fen.find_first_of(" ");

  Square currentSquare{static_cast<Square>(56)};

  for (int i = 0; i < endOfPiecePlacement; i++)
  {
    if (fen[i] == '/')
    {
      // Advance to next rank
      currentSquare = static_cast<Square>(currentSquare - 16);
    }
    else if (isdigit(fen[i]))
    {
      // Advance squares (digits are empty squares)
      currentSquare = static_cast<Square>(currentSquare + (fen[i] - '0'));
    }
    else
    {
      const PieceType pieceType = piece::fenCharToPieceType(fen[i]);
      const PieceColour pieceColour = piece::fenCharToPieceColour(fen[i]);

      bitboard::setSquare(m_pieceTypeBitboard[static_cast<int>(pieceType)], currentSquare);
      bitboard::setSquare(m_colourBitboard[static_cast<int>(pieceColour)], currentSquare);
      currentSquare = static_cast<Square>(currentSquare + 1);
    }
  }

  m_state.loadFen(fen.substr(endOfPiecePlacement + 1));
}

Bitboard Board::get_position(PieceType type)
{
  return m_pieceTypeBitboard[static_cast<int>(type)];
}

Bitboard Board::get_position(PieceColour colour)
{
  return m_colourBitboard[static_cast<int>(colour)];
}

Bitboard Board::getPosition(PieceColour pieceColour, PieceType pieceType)
{
  return get_position(pieceType) & get_position(pieceColour);
}

PieceType Board::getPieceTypeOn(Square square)
{
  const Bitboard square_bb = bitboard::createBitboard(square);

  for (int i = 0; i < 6; i++)
  {
    if ((square_bb & m_pieceTypeBitboard[i]) > 0)
    {
      return static_cast<PieceType>(i);
    }
  }
  return PieceType::NULL_PIECE;
}

PieceColour Board::getPieceColourOn(Square square)
{
  const Bitboard square_bb = bitboard::createBitboard(square);

  for (int i = 0; i < 2; i++)
  {
    if ((square_bb & m_colourBitboard[i]) > 0)
    {
      return static_cast<PieceColour>(i);
    }
  }
  return PieceColour::NULL_COLOUR;
}

Bitboard Board::occupiedSquares()
{
  return m_colourBitboard[0] | m_colourBitboard[1];
}

Bitboard Board::emptySquares()
{
  return ~occupiedSquares();
}

std::vector<Move> Board::generateMoves()
{
  Move psuedoLegalMoveList[330];
  Move enPassantMove;
  int moveCounter{0};

  const PieceColour thisSide = m_state->sideToMove();
  const PieceColour otherSide = m_state->sideNotToMove();

  if (thisSide == PieceColour::WHITE)
  {
    generatePawnMoves<PieceColour::WHITE>(&psuedoLegalMoveList[moveCounter],
                                          moveCounter,
                                          getPosition(thisSide, PieceType::PAWN),
                                          emptySquares());

    move::generateEpCaptures<PieceColour::WHITE>(&enPassantMove,
                                           moveCounter,
                                           getPosition(thisSide, PieceType::PAWN),
                                           m_state->epTarget());
  }
  else
  {
    generatePawnMoves<PieceColour::BLACK>(&psuedoLegalMoveList[moveCounter],
                                          moveCounter,
                                          getPosition(thisSide, PieceType::PAWN),
                                          emptySquares());

    move::generateEpCaptures<PieceColour::BLACK>(&psuedoLegalMoveList[moveCounter],
                                           moveCounter,
                                           getPosition(thisSide, PieceType::PAWN),
                                           m_state->epTarget());
  }

  moveCounter += generatePieceMoves<PieceType::KNIGHT>(&psuedoLegalMoveList[moveCounter],
                                                       thisSide);

  moveCounter += generatePieceMoves<PieceType::KING>(&psuedoLegalMoveList[moveCounter],
                                                     thisSide);

  moveCounter += generatePieceMoves<PieceType::BISHOP>(&psuedoLegalMoveList[moveCounter],
                                                       thisSide);

  moveCounter += generatePieceMoves<PieceType::ROOK>(&psuedoLegalMoveList[moveCounter],
                                                     thisSide);

  moveCounter += generatePieceMoves<PieceType::QUEEN>(&psuedoLegalMoveList[moveCounter],
                                                      thisSide);

  std::vector<Move> legal_moves;
  for (int i = 0; i < moveCounter; i++)
  {
    makeMove(psuedoLegalMoveList[i]);
    // TODO (haigh) not safe, check for null here
    if (!isCheck(m_state->getPrevState()->sideToMove()))
    {
      legal_moves.push_back(psuedoLegalMoveList[i]);
    }
    undoMove();
  }

  legal_moves = generateCastlingMoves(legal_moves);

  return legal_moves;
}

std::vector<Move> Board::generateCastlingMoves(std::vector<Move> moves)
{
  const Bitboard squaresAttackedByEnemy = attacked_by(m_state->sideNotToMove());
  const Bitboard king = getPosition(m_state->sideToMove(), PieceType::KING);

  if (m_state->canKingSideCastle() && !isCheck())
  {
    const Bitboard kingPath = bitboard::shift<Direction::EAST>(king) | bitboard::shift<Direction::EAST>(bitboard::shift<Direction::EAST>(king));
    if ((kingPath & occupiedSquares()) == 0 && (kingPath & squaresAttackedByEnemy) == 0)
    {
      moves.push_back(move::makeKingsideCastle());
    }
  }

  if (m_state->canQueenSideCastle() && !isCheck())
  {
    const Bitboard kingPath = bitboard::shift<Direction::WEST>(king) | bitboard::shift<Direction::WEST>(bitboard::shift<Direction::WEST>(king));
    const Bitboard rookPath = kingPath | bitboard::shift<Direction::WEST>(kingPath);
    if ((rookPath & occupiedSquares()) == 0 && (kingPath & squaresAttackedByEnemy) == 0)
    {
      moves.push_back(move::makeQueensideCastle());
    }

  }
  return moves;
}

Board::MoveResult Board::makeMove(const Move &move)
{
  MoveResult result{};

  if (m_state->sideToMove() == PieceColour::WHITE)
  {
    result = processMove<PieceColour::WHITE>(move, false);
  }
  else
  {
    result = processMove<PieceColour::BLACK>(move, false);
  }

  m_state.update(move);

  return result;
}

Board::MoveResult Board::undoMove()
{
  const Move* move = m_state.pop();

  // TODO (haigh) check that move and state not null

  if (m_state->sideToMove() == PieceColour::WHITE)
  {
    return processMove<PieceColour::WHITE>(*move, true);
  }

  return processMove<PieceColour::BLACK>(*move, true);
}

bool Board::isCheck()
{
  Bitboard king = getPosition(m_state->sideToMove(), PieceType::KING);
  Bitboard attackers = attacked_by(m_state->sideNotToMove());
  return (king & attackers) > 0;
}

bool Board::isCheck(PieceColour colour)
{
  Bitboard king = getPosition(colour, PieceType::KING);
  Bitboard attackers = attacked_by(piece::otherColour(colour));
  return (king & attackers) > 0;
}

bool Board::isCheckmate()
{
  if (isCheck())
  {
    std::vector<Move> moves = generateMoves();
    if (moves.empty())
    {
      return true;
    }
  }
  return false;
}

std::string Board::toFen()
{
  std::string fen{""};
  for (int iRank = 7; iRank >= 0; iRank--)
  {
    fen += rankToFen((Rank) iRank);
    if (iRank != 0)
    {
      // No slash after last rank
      fen += "/";
    }
  }
  fen += " ";
  fen += m_state->toFen();
  return fen;
}

bool Board::canKingSideCastle(PieceColour colour)
{
  return m_state->canKingSideCastle(colour);
}

bool Board::canQueenSideCastle(PieceColour colour)
{
  return m_state->canQueenSideCastle(colour);
}

std::string Board::rankToFen(Rank rank)
{
  std::string fen{""};
  int emptySum = 0;
  for (File fileIndex = 0; fileIndex < 8; fileIndex++)
  {
    PieceType type = getPieceTypeOn(squareIndex(fileIndex, rank));
    if (type != PieceType::NULL_PIECE)
    {
      PieceColour colour = getPieceColourOn(squareIndex(fileIndex, rank));
      if (emptySum != 0)
      {
        fen += std::to_string(emptySum);
        emptySum = 0;
      }
      fen += piece::pieceToFenChar(type, colour);
    }
    else
    {
      emptySum++;
    }
  }
  if (emptySum != 0)
  {
    fen += std::to_string(emptySum);
  }

  return fen;
}

std::string Board::rankToBoardFen(Rank rank)
{
  std::string fen{""};
  int emptySum = 0;
  for (File fileIndex = 0; fileIndex < 8; fileIndex++)
  {
    PieceType type = getPieceTypeOn(squareIndex(fileIndex, rank));
    if (type != PieceType::NULL_PIECE)
    {
      PieceColour colour = getPieceColourOn(squareIndex(fileIndex, rank));
      if (emptySum != 0)
      {
        while (emptySum)
        {
          fen += ".";
          --emptySum;
        }
      }
      fen += piece::pieceToFenChar(type, colour);
    }
    else
    {
      emptySum++;
    }
  }
  if (emptySum != 0)
  {
    fen += std::to_string(emptySum);
  }

  return fen;
}

Bitboard Board::pawnAttacks(PieceColour colour)
{
  Bitboard pawnsBitboard = getPosition(colour, PieceType::PAWN);

  return (colour == PieceColour::BLACK) ? pawns::pawnAttacks<PieceColour::BLACK>(pawnsBitboard)
                                        : pawns::pawnAttacks<PieceColour::WHITE>(pawnsBitboard);
}

Bitboard Board::pieceAttacks(PieceType pieceType, PieceColour pieceColour)
{
  switch (pieceType)
  {
    case PieceType::KNIGHT:
    {
      return piece::pieceAttacks<PieceType::KNIGHT>(getPosition(pieceColour, pieceType),
                                                        occupiedSquares());
    }

    case PieceType::BISHOP:
    {
      return piece::pieceAttacks<PieceType::BISHOP>(getPosition(pieceColour, pieceType),
                                                        occupiedSquares());
    }

    case PieceType::ROOK:
    {
      return piece::pieceAttacks<PieceType::ROOK>(getPosition(pieceColour, pieceType),
                                                      occupiedSquares());
    }

    case PieceType::QUEEN:
    {
      return piece::pieceAttacks<PieceType::QUEEN>(getPosition(pieceColour, pieceType),
                                                       occupiedSquares());
    }

    case PieceType::KING:
    {
      return piece::pieceAttacks<PieceType::KING>(getPosition(pieceColour, pieceType),
                                                      occupiedSquares());
    }

    case PieceType::PAWN:
    case PieceType::NULL_PIECE:
    default:
    {
      break;
    }
  }
  // If the wrong piece has been provided then we return 0, i.e., this piece attacks no squares.
  return Bitboard{0};
}

Bitboard Board::allAttacks(PieceColour colour)
{
  Bitboard attacked_squares{0};
  Bitboard friendly_pieces = get_position(colour);
  attacked_squares |= (pawnAttacks(colour) & ~friendly_pieces);
  attacked_squares |= (pieceAttacks(PieceType::KNIGHT, colour) & ~friendly_pieces);
  attacked_squares |= (pieceAttacks(PieceType::BISHOP, colour) & ~friendly_pieces);
  attacked_squares |= (pieceAttacks(PieceType::ROOK, colour) & ~friendly_pieces);
  attacked_squares |= (pieceAttacks(PieceType::QUEEN, colour) & ~friendly_pieces);
  attacked_squares |= (pieceAttacks(PieceType::KING, colour) & ~friendly_pieces);
  return attacked_squares;
}

} // namespace yak
