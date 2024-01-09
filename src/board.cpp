#include <cassert>
#include "board.h"

/*
std::string Move::to_algebraic() {
	if(piece_to_move == PieceType::PAWN) {
		std::string output = "";
		if (is_capture()) {
			output += bitboard::to_algebraic(from)[0] + "x" + bitboard::to_algebraic(to);
		}
		else {
			output += bitboard::to_algebraic(to);
		}
		if (is_promotion()) {
			output += "=" + pieces::white_piece_type_to_fen_char(promotion_piece);
		}
		return output;
	}
	else if (piece_to_move != PieceType::PAWN && piece_to_capture == PieceType::NULL_PIECE) {
		return pieces::white_piece_type_to_fen_char(piece_to_move) + bitboard::to_algebraic(to);
	}
	else if (piece_to_move != PieceType::PAWN && piece_to_capture != PieceType::NULL_PIECE) {
		std::string algebraic_move{ pieces::white_piece_type_to_fen_char(piece_to_move) };
		return algebraic_move + "x" + bitboard::to_algebraic(to);
	}
	else if (castle == PieceType::KING) {
		return "OO";
	}
	else if (castle == PieceType::QUEEN) {
		return "OOO";
	}
	else {
		return "XX";
	}
}
*/

namespace yak {

void Board::parseFen(const std::string &fen)
{
  auto end_of_piece_placement = fen.find_first_of(" ");

  Square current_square{56};

  for (int i = 0; i < end_of_piece_placement; i++)
  {
    if (fen[i] == '/')
    {
      // Advance to next rank
      current_square -= 16;
    }
    else if (isdigit(fen[i]))
    {
      // Advance squares (digits are empty squares)
      current_square += (fen[i] - '0');
    }
    else
    {
      PieceType piece_type = pieces::fenCharToPieceType(fen[i]);
      PieceColour piece_colour = pieces::fenCharToPieceColour(fen[i]);

      bitboard::setSquare(m_pieceTypeBitboard[static_cast<int>(piece_type)], current_square);
      bitboard::setSquare(m_colourBitboard[static_cast<int>(piece_colour)], current_square);
      current_square++;
    }
  }
  m_currentState = GameState(fen.substr(end_of_piece_placement + 1));
}

PieceType Board::getPieceTypeOn(Square square)
{
  Bitboard square_bb = bitboard::toBitboard(square);
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
  Bitboard square_bb = bitboard::toBitboard(square);
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

std::vector<piece::Move> Board::generateMoves()
{
  piece::Move psuedoLegalMoveList[330];
  piece::Move enPassantMove;
  int move_counter{0};

  PieceColour thisSide = m_currentState.sideToMove();
  PieceColour otherSide = m_currentState.sideNotToMove();

  if (thisSide == PieceColour::WHITE)
  {
    piece::generate_pawn_moves<PieceColour::WHITE>(&psuedoLegalMoveList[move_counter],
                                                   move_counter,
                                                   getPosition(thisSide, PieceType::PAWN),
                                                   emptySquares(),
                                                   get_position(otherSide));

    piece::generateEpCaptures<PieceColour::WHITE>(&enPassantMove,
                                                  move_counter,
                                                  getPosition(thisSide, PieceType::PAWN),
                                                  epTarget());
  }
  else
  {
    piece::generate_pawn_moves<PieceColour::BLACK>(&psuedoLegalMoveList[move_counter],
                                                   move_counter,
                                                   getPosition(thisSide, PieceType::PAWN),
                                                   emptySquares(),
                                                   get_position(otherSide));

    piece::generateEpCaptures<PieceColour::BLACK>(&psuedoLegalMoveList[move_counter],
                                                  move_counter,
                                                  getPosition(thisSide, PieceType::PAWN),
                                                  epTarget());
  }

  piece::generatePieceMoves<PieceType::KNIGHT>(&psuedoLegalMoveList[move_counter],
                                               move_counter,
                                               getPosition(thisSide, PieceType::KNIGHT),
                                               emptySquares(),
                                               get_position(otherSide));

  piece::generatePieceMoves<PieceType::KING>(&psuedoLegalMoveList[move_counter],
                                             move_counter,
                                             getPosition(thisSide, PieceType::KING),
                                             emptySquares(),
                                             get_position(otherSide));

  piece::generatePieceMoves<PieceType::BISHOP>(&psuedoLegalMoveList[move_counter],
                                               move_counter,
                                               getPosition(thisSide, PieceType::BISHOP),
                                               emptySquares(),
                                               get_position(otherSide));

  piece::generatePieceMoves<PieceType::ROOK>(&psuedoLegalMoveList[move_counter],
                                             move_counter,
                                             getPosition(thisSide, PieceType::ROOK),
                                             emptySquares(),
                                             get_position(otherSide));

  piece::generatePieceMoves<PieceType::QUEEN>(&psuedoLegalMoveList[move_counter],
                                              move_counter,
                                              getPosition(thisSide, PieceType::QUEEN),
                                              emptySquares(),
                                              get_position(otherSide));

  std::vector<piece::Move> legal_moves;
  for (int i = 0; i < move_counter; i++)
  {
    makeMove(psuedoLegalMoveList[i]);
    if (!isCheck(m_previousState.sideToMove()))
    {
      legal_moves.push_back(psuedoLegalMoveList[i]);
    }
    undoMove();
  }

  legal_moves = generateCastlingMoves(legal_moves);

  return legal_moves;
}

std::vector<piece::Move> Board::generateCastlingMoves(std::vector<piece::Move> moves)
{
  Bitboard squares_attacked_by_enemy = attacked_by(m_currentState.sideNotToMove());

  Bitboard king = getPosition(m_currentState.sideToMove(), PieceType::KING);

  if (m_currentState.canKingSideCastle() && !isCheck())
  {
    Bitboard king_path = bitboard::shift<Direction::EAST>(king) | bitboard::shift<Direction::EAST>(bitboard::shift<Direction::EAST>(king));
    if ((king_path & occupiedSquares()) == 0 && (king_path & squares_attacked_by_enemy) == 0)
    {
      moves.push_back(piece::makeKingsideCastle());
    }
  }

  if (m_currentState.canQueenSideCastle() && !isCheck())
  {
    Bitboard king_path = bitboard::shift<Direction::WEST>(king) | bitboard::shift<Direction::WEST>(bitboard::shift<Direction::WEST>(king));
    Bitboard rook_path = king_path | bitboard::shift<Direction::WEST>(king_path);
    if ((rook_path & occupiedSquares()) == 0 && (king_path & squares_attacked_by_enemy) == 0)
    {
      moves.push_back(piece::makeQueensideCastle());
    }

  }
  return moves;
}

void Board::makeMove(PieceType type, PieceColour colour, Square from, Square to)
{
  Bitboard from_to_bitboard = bitboard::toBitboard(from) ^ bitboard::toBitboard(to);
  m_pieceTypeBitboard[static_cast<int>(type)] ^= from_to_bitboard;
  m_colourBitboard[static_cast<int>(colour)] ^= from_to_bitboard;
}

void Board::makeMove(const piece::Move &move)
{
  if (m_currentState.sideToMove() == PieceColour::WHITE)
  {
    processMove<PieceColour::WHITE>(move, false);
  }
  else
  {
    processMove<PieceColour::BLACK>(move, false);
  }

  m_previousState = m_currentState;
  m_currentState.update(move);
  m_currentState.toggleSideToMove();
  m_previousMoveF = move;
}

void Board::undoMove()
{
  if (m_previousMoveF.from != m_previousMoveF.to)
  {
    m_currentState.toggleSideToMove();
    if (m_currentState.sideToMove() == PieceColour::WHITE)
    {
      processMove<PieceColour::WHITE>(m_previousMoveF, true);
    }
    else
    {
      processMove<PieceColour::BLACK>(m_previousMoveF, true);
    }
    m_currentState = m_previousState;
  }
}

bool Board::isCheck()
{
  Bitboard king = getPosition(m_currentState.sideToMove(), PieceType::KING);
  Bitboard attackers = attacked_by(pieces::otherColour(m_currentState.sideToMove()));
  return (king & attackers) > 0;
}

bool Board::isCheck(PieceColour colour)
{
  Bitboard king = getPosition(colour, PieceType::KING);
  Bitboard attackers = attacked_by(pieces::otherColour(colour));
  return (king & attackers) > 0;
}

bool Board::isCheckmate()
{
  if (isCheck())
  {
    std::vector<piece::Move> moves = generateMoves();
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
  fen += m_currentState.toFen();
  return fen;
}

bool Board::canKingSideCastle(PieceColour colour)
{
  return m_currentState.canKingSideCastle(colour);
}

bool Board::canQueenSideCastle(PieceColour colour)
{
  return m_currentState.canQueenSideCastle(colour);
}

std::string Board::rankToFen(Rank rank)
{
  std::string fen{""};
  int emptySum = 0;
  for (File fileIndex = 0; fileIndex < 8; fileIndex++)
  {
    PieceType type = getPieceTypeOn(bitboard::squareIndex(fileIndex, rank));
    if (type != PieceType::NULL_PIECE)
    {
      PieceColour colour = getPieceColourOn(bitboard::squareIndex(fileIndex, rank));
      if (emptySum != 0)
      {
        fen += std::to_string(emptySum);
        emptySum = 0;
      }
      fen += pieces::pieceToFenChar(type, colour);
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

  return (colour == PieceColour::BLACK) ? piece::pawn_attacks<PieceColour::BLACK>(pawnsBitboard)
                                        : piece::pawn_attacks<PieceColour::WHITE>(pawnsBitboard);
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

CastlingRights::CastlingRights(std::string fen)
{
  if (fen.empty() || (fen.size() == 1 && fen[0] == '-'))
  {
    return;
  }

  for (const auto &character : fen)
  {
    switch (character)
    {
      case 'K':
      {
        white[0] = true;
        break;
      }
      case 'Q':
      {
        white[1] = true;
        break;
      }
      case 'k':
      {
        black[0] = true;
        break;
      }
      case 'q':
      {
        black[1] = true;
        break;
      }
    }
  }
}

bool CastlingRights::kingSide(PieceColour colour)
{
  if (colour == PieceColour::BLACK)
  {
    return black[0];
  }
  else
  {
    return white[0];
  }
}

bool CastlingRights::queenSide(PieceColour colour)
{
  if (colour == PieceColour::BLACK)
  {
    return black[1];
  }
  else
  {
    return white[1];
  }
}

void CastlingRights::update(const piece::Move &move, PieceColour side)
{
  if (move.castle != PieceType::NULL_PIECE)
  {
    if (side == PieceColour::WHITE)
    {
      white[0] = false;
      white[1] = false;
    }
    else
    {
      black[0] = false;
      black[1] = false;
    }
    return;
  }
  white[0] = !(move.from == 7 || move.to == 7 || move.from == 4) && white[0];
  white[1] = !(move.from == 0 || move.to == 0 || move.from == 4) && white[1];
  black[0] = !(move.from == 63 || move.to == 63 || move.from == 60) && black[0];
  black[1] = !(move.from == 56 || move.to == 56 || move.from == 60) && black[1];

}

std::string CastlingRights::fen()
{
  std::string fen_string = "";
  fen_string += white[0] ? "K" : "";
  fen_string += white[1] ? "Q" : "";
  fen_string += black[0] ? "k" : "";
  fen_string += black[1] ? "q" : "";
  if (fen_string.empty())
  {
    fen_string += "-";
  }
  return fen_string;
}

GameState::GameState() : m_side(PieceColour::WHITE)
{
}

GameState::GameState(const std::string& fen)
{
  parseFen(fen);
}

void GameState::parseFen(const std::string& fen)
{
  auto startOfCastlingRights = fen.find_first_of(" ");
  auto startOfEpTarget = fen.find_first_of(" ", startOfCastlingRights + 1);
  auto startOfMoveNumber = fen.find_first_of(" ", startOfEpTarget + 1);
  if (fen[0] == 'w')
  {
    m_side = PieceColour::WHITE;
  }
  else if (fen[0] == 'b')
  {
    m_side = PieceColour::BLACK;
  }
  else
  {
    // TODO Throw an exception for this
  }

  if (fen[startOfEpTarget + 1] == '-')
  {
    m_hasEpTarget = false;
  }
  else
  {
    m_hasEpTarget = true;
    m_epSquare = bitboard::squareIndex(fen.substr(startOfEpTarget + 1, 2));
  }

  int lengthCastlingRights = startOfEpTarget - startOfCastlingRights;
  m_castlingRights = CastlingRights(fen.substr(startOfCastlingRights + 1, lengthCastlingRights));
}

std::string GameState::toFen()
{
  std::string fen = "";

  if (m_side == PieceColour::WHITE)
  {
    fen += 'w';
  }
  else
  {
    fen += 'b';
  }

  fen += " " + m_castlingRights.fen() + " ";
  fen += m_hasEpTarget ? bitboard::to_algebraic(m_epSquare) : "-";
  fen += " 0 ";
  fen += std::to_string(m_moveClock);

  return fen;
}

/*
void GameState::update(Move move) {
	if (move.is_double_push()) {
		m_hasEpTarget = true;
		if (m_side == PieceColour::WHITE) {
			m_epSquare = move.to_square() - 8;
		}
		else {
			m_epSquare = move.to_square() + 8;
		}
	}
	else {
		m_hasEpTarget = false;
	}

	m_castlingRights.update(move, m_side);
	if (m_side == PieceColour::BLACK) {
		m_moveClock++;
	}
}
*/

void GameState::update(const piece::Move &move)
{
  if (move.double_push)
  {
    m_hasEpTarget = true;
    if (m_side == PieceColour::WHITE)
    {
      m_epSquare = move.to - 8;
    }
    else
    {
      m_epSquare = move.to + 8;
    }
  }
  else
  {
    m_hasEpTarget = false;
  }

  m_castlingRights.update(move, m_side);

  if (m_side == PieceColour::BLACK)
  {
    m_moveClock++;
  }
}

bool GameState::canKingSideCastle(PieceColour colour)
{
  return m_castlingRights.kingSide(colour);
}

bool GameState::canKingSideCastle()
{
  return m_castlingRights.kingSide(m_side);
}

bool GameState::canQueenSideCastle(PieceColour colour)
{
  return m_castlingRights.queenSide(colour);
}

bool GameState::canQueenSideCastle()
{
  return m_castlingRights.queenSide(m_side);
}

void GameState::toggleSideToMove()
{
  if (m_side == PieceColour::BLACK)
  {
    m_side = PieceColour::WHITE;
  }
  else
  {
    m_side = PieceColour::BLACK;
  }
}

Bitboard GameState::epTarget() const
{
  if (m_hasEpTarget)
  {
    return bitboard::toBitboard(m_epSquare);
  }
  else
  {
    return Bitboard(0);
  }
}

Square GameState::epTargetSquare() const
{
  return m_epSquare;
}

PieceColour GameState::sideNotToMove() const
{
  return pieces::otherColour(m_side);
}

} // namespace yak
