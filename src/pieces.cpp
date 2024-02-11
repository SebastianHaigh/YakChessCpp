#include "pieces.h"

namespace yak::piece {

PieceColour otherColour(PieceColour colour) {
  if (colour == PieceColour::WHITE)
  {
    return PieceColour::BLACK;
  }
  else if (colour == PieceColour::BLACK)
  {
    return PieceColour::WHITE;
  }
  else
  {
    return PieceColour::NULL_COLOUR;
  }
}

PieceType fenCharToPieceType(const char fenChar) {
  switch (fenChar) {
    case 'p':
    case 'P':
      return PieceType::PAWN;
    case 'n':
    case 'N':
      return PieceType::KNIGHT;
    case 'b':
    case 'B':
      return PieceType::BISHOP;
    case 'r':
    case 'R':
      return PieceType::ROOK;
    case 'q':
    case 'Q':
      return PieceType::QUEEN;
    case 'k':
    case 'K':
      return PieceType::KING;
    default:
      return PieceType::NULL_PIECE;
  }
}

PieceColour fenCharToPieceColour(const char fenChar) {
  switch (fenChar) {
    case 'p':
    case 'n':
    case 'b':
    case 'r':
    case 'q':
    case 'k':
      return PieceColour::BLACK;
    case 'P':
    case 'N':
    case 'B':
    case 'R':
    case 'Q':
    case 'K':
      return PieceColour::WHITE;
    default:
      return PieceColour::NULL_COLOUR;
  }
}

char pieceToFenChar(const PieceType& type, const PieceColour& colour) {

  if (type == PieceType::NULL_PIECE) return 'x';

  static char chars[15] = {"pPnNbBrRqQkK"};

  const int index = static_cast<bool>(colour) + (2 * static_cast<int>(type));

  return chars[index];
}

} // namespace yak::piece
