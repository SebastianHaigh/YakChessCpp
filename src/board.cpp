#include "board.h"

#include <cstring>
#include <ctype.h>
#include <sstream>

#include <bitboard.h>
#include <pieces.h>

namespace yak {

Board::Board(std::string_view fen)
{
  parseFen(fen);
}

bool Board::reset(std::string_view fen)
{
  std::memset(m_pieceTypeBitboard, bitboard::EMPTY, sizeof(m_pieceTypeBitboard));
  std::memset(m_colourBitboard, bitboard::EMPTY, sizeof(m_colourBitboard));
  m_psudeoLegalMovePointer = 0;

  return parseFen(fen);
}

bool Board::parseFen(std::string_view fen)
{
  if (fen.empty()) return false;

  auto endOfPiecePlacement = fen.find_first_of(" ");

  if (endOfPiecePlacement == std::string::npos) return false;

  constexpr std::array<Square, 8> hFileSquares = { H8, H7, H6, H5, H4, H3, H2, H1 };
  Square currentSquare{ A8 }; // Starting from the upper left corner of the board

  int squaresOnRank{ 0 };

  auto hFile_p = hFileSquares.begin();

  for (int i = 0; i < endOfPiecePlacement; i++)
  {
    if (fen[i] == '/')
    {
      // Make sure that every square on the last rank was accounted for
      if (squaresOnRank != 8) return false;

      // Advance to next rank
      currentSquare = static_cast<Square>(currentSquare - 16);
      squaresOnRank = 0;
    }
    else if (isdigit(fen[i]))
    {
      // Advance squares (digits are empty squares)
      auto digit = fen[i] - '0';
      currentSquare = static_cast<Square>(currentSquare + digit);
      squaresOnRank += digit;
    }
    else
    {
      const PieceType pieceType = piece::fenCharToPieceType(fen[i]);
      const PieceColour pieceColour = piece::fenCharToPieceColour(fen[i]);

      bitboard::setSquare(m_pieceTypeBitboard[static_cast<int>(pieceType)], currentSquare);
      bitboard::setSquare(m_colourBitboard[static_cast<int>(pieceColour)], currentSquare);
      currentSquare = static_cast<Square>(currentSquare + 1);
      ++squaresOnRank;
    }
  }

  // Make sure that every square on the last rank was accounted for
  if (squaresOnRank != 8) return false;

  return m_state.loadFen(fen.substr(endOfPiecePlacement + 1));
}

Bitboard Board::get_position(PieceType type) const
{
  return m_pieceTypeBitboard[static_cast<int>(type)];
}

Bitboard Board::get_position(PieceColour colour) const
{
  return m_colourBitboard[static_cast<int>(colour)];
}

Bitboard Board::getPosition(PieceColour pieceColour, PieceType pieceType) const
{
  return get_position(pieceType) & get_position(pieceColour);
}

PieceType Board::getPieceTypeOn(Square square) const
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

PieceColour Board::getPieceColourOn(Square square) const
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

Bitboard Board::occupiedSquares() const
{
  return m_colourBitboard[0] | m_colourBitboard[1];
}

Bitboard Board::emptySquares() const
{
  return ~occupiedSquares();
}

PieceColour Board::sideToMove() const
{
  return m_state->sideToMove();
}

std::vector<Move> Board::generateMoves()
{
  m_psudeoLegalMovePointer = 0;
  Move enPassantMove;

  const PieceColour thisSide = m_state->sideToMove();
  const PieceColour otherSide = m_state->sideNotToMove();

  if (thisSide == PieceColour::WHITE)
  {
    m_psudeoLegalMovePointer += generatePawnMoves<PieceColour::WHITE>(&m_psuedoLegalMoveListBuffer[m_psudeoLegalMovePointer],
                                                                      getPosition(thisSide, PieceType::PAWN),
                                                                      emptySquares());

    m_psudeoLegalMovePointer += generateEpCaptures<PieceColour::WHITE>(&m_psuedoLegalMoveListBuffer[m_psudeoLegalMovePointer],
                                                                       getPosition(thisSide, PieceType::PAWN),
                                                                       m_state->epTarget());
  }
  else
  {
    m_psudeoLegalMovePointer += generatePawnMoves<PieceColour::BLACK>(&m_psuedoLegalMoveListBuffer[m_psudeoLegalMovePointer],
                                                                     getPosition(thisSide, PieceType::PAWN),
                                                                     emptySquares());

    m_psudeoLegalMovePointer += generateEpCaptures<PieceColour::BLACK>(&m_psuedoLegalMoveListBuffer[m_psudeoLegalMovePointer],
                                                                       getPosition(thisSide, PieceType::PAWN),
                                                                       m_state->epTarget());
  }

  m_psudeoLegalMovePointer += generatePieceMoves<PieceType::KNIGHT>(&m_psuedoLegalMoveListBuffer[m_psudeoLegalMovePointer],
                                                                    thisSide);

  m_psudeoLegalMovePointer += generatePieceMoves<PieceType::KING>(&m_psuedoLegalMoveListBuffer[m_psudeoLegalMovePointer],
                                                                  thisSide);

  m_psudeoLegalMovePointer += generatePieceMoves<PieceType::BISHOP>(&m_psuedoLegalMoveListBuffer[m_psudeoLegalMovePointer],
                                                                    thisSide);

  m_psudeoLegalMovePointer += generatePieceMoves<PieceType::ROOK>(&m_psuedoLegalMoveListBuffer[m_psudeoLegalMovePointer],
                                                                  thisSide);

  m_psudeoLegalMovePointer += generatePieceMoves<PieceType::QUEEN>(&m_psuedoLegalMoveListBuffer[m_psudeoLegalMovePointer],
                                                                   thisSide);

  std::vector<Move> legal_moves;
  legal_moves.reserve(m_psudeoLegalMovePointer);
  for (int i = 0; i < m_psudeoLegalMovePointer; i++)
  {
    makeMove(m_psuedoLegalMoveListBuffer[i]);
    // TODO (haigh) not safe, check for null here
    if (!isCheck(m_state->getPrevState()->sideToMove()))
    {
      legal_moves.push_back(m_psuedoLegalMoveListBuffer[i]);
    }
    undoMove();
  }

  generateCastlingMoves(legal_moves);

  return legal_moves;
}

void Board::generateCastlingMoves(std::vector<Move>& moves) const
{
  const Bitboard squaresAttackedByEnemy = attacked_by(m_state->sideNotToMove());
  const Bitboard king = getPosition(m_state->sideToMove(), PieceType::KING);

  if (m_state->canKingSideCastle() && !isCheck())
  {
    const Bitboard kingPath = bitboard::shift<Direction::EAST>(king) | bitboard::shift<Direction::EAST>(bitboard::shift<Direction::EAST>(king));
    if ((kingPath & occupiedSquares()) == 0 && (kingPath & squaresAttackedByEnemy) == 0)
    {
      moves.push_back(makeKingsideCastle());
    }
  }

  if (m_state->canQueenSideCastle() && !isCheck())
  {
    const Bitboard kingPath = bitboard::shift<Direction::WEST>(king) | bitboard::shift<Direction::WEST>(bitboard::shift<Direction::WEST>(king));
    const Bitboard rookPath = kingPath | bitboard::shift<Direction::WEST>(kingPath);
    if ((rookPath & occupiedSquares()) == 0 && (kingPath & squaresAttackedByEnemy) == 0)
    {
      moves.push_back(makeQueensideCastle());
    }

  }
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

bool Board::isCheck() const
{
  Bitboard king = getPosition(m_state->sideToMove(), PieceType::KING);
  Bitboard attackers = attacked_by(m_state->sideNotToMove());
  return (king & attackers) > 0;
}

bool Board::isCheck(PieceColour colour) const
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

std::string Board::toFen() const
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

std::string Board::to_string() const
{
  std::stringstream ss;

  const Square squares[8][8] = {
    {A8, B8, C8, D8, E8, F8, G8, H8 },
    {A7, B7, C7, D7, E7, F7, G7, H7 },
    {A6, B6, C6, D6, E6, F6, G6, H6 },
    {A5, B5, C5, D5, E5, F5, G5, H5 },
    {A4, B4, C4, D4, E4, F4, G4, H4 },
    {A3, B3, C3, D3, E3, F3, G3, H3 },
    {A2, B2, C2, D2, E2, F2, G2, H2 },
    {A1, B1, C1, D1, E1, F1, G1, H1 }
  };

  for (const auto* file : squares)
  {
    for (int i = 0; i < 8; ++i)
    {
      auto type = getPieceTypeOn(file[i]);
      auto colour = getPieceColourOn(file[i]);

      ss << " ";

      switch (type)
      {
        case PieceType::PAWN:
          {
            if (colour == PieceColour::WHITE)
            { ss << "P"; }
            else
            { ss << "p"; }
            break;
          }
        case PieceType::KNIGHT: if (colour == PieceColour::WHITE) { ss << "N"; } else { ss << "n"; } break;
        case PieceType::BISHOP: if (colour == PieceColour::WHITE) { ss << "B"; } else { ss << "b"; } break;
        case PieceType::ROOK: if (colour == PieceColour::WHITE) { ss << "R"; } else { ss << "r"; } break;
        case PieceType::QUEEN: if (colour == PieceColour::WHITE) { ss << "Q"; } else { ss << "q"; } break;
        case PieceType::KING: if (colour == PieceColour::WHITE) { ss << "K"; } else { ss << "k"; } break;
        case PieceType::NULL_PIECE: ss << "."; break;
      }
    }
    ss << '\n';
  }
  ss << '\n';

  return ss.str();
}

bool Board::canKingSideCastle(PieceColour colour) const
{
  return m_state->canKingSideCastle(colour);
}

bool Board::canQueenSideCastle(PieceColour colour) const
{
  return m_state->canQueenSideCastle(colour);
}

std::string Board::rankToFen(Rank rank) const
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

std::string Board::rankToBoardFen(Rank rank) const
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

Bitboard Board::pawnAttacks(PieceColour colour) const
{
  Bitboard pawnsBitboard = getPosition(colour, PieceType::PAWN);

  return (colour == PieceColour::BLACK) ? pawns::pawnAttacks<PieceColour::BLACK>(pawnsBitboard)
                                        : pawns::pawnAttacks<PieceColour::WHITE>(pawnsBitboard);
}

Bitboard Board::allAttacks(PieceColour colour) const
{
  Bitboard attacked_bb{0};
  Bitboard friendly_bb = get_position(colour);
  attacked_bb |= (pawnAttacks(colour) & ~friendly_bb);
  attacked_bb |= (piece::pieceAttacks<PieceType::KNIGHT>(getPosition(colour, PieceType::KNIGHT), occupiedSquares()) & ~friendly_bb);
  attacked_bb |= (piece::pieceAttacks<PieceType::BISHOP>(getPosition(colour, PieceType::BISHOP), occupiedSquares()) & ~friendly_bb);
  attacked_bb |= (piece::pieceAttacks<PieceType::ROOK>(getPosition(colour, PieceType::ROOK), occupiedSquares()) & ~friendly_bb);
  attacked_bb |= (piece::pieceAttacks<PieceType::QUEEN>(getPosition(colour, PieceType::QUEEN), occupiedSquares()) & ~friendly_bb);
  attacked_bb |= (piece::pieceAttacks<PieceType::KING>(getPosition(colour, PieceType::KING), occupiedSquares()) & ~friendly_bb);
  return attacked_bb;
}

} // namespace yak
