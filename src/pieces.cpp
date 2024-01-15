#include "pieces.h"

namespace yak {
namespace piece {

void generate_sliding_piece_moves(const attackmap::RookMap &atk_map,
                                  Move *move_list,
                                  int &move_counter,
                                  Bitboard piece_positions,
                                  Bitboard empty_squares,
                                  Bitboard opponent_pieces)
{
  while (piece_positions)
  {
    Square from = bitboard::popLS1B(piece_positions);
    Bitboard atk_bb = atk_map.attacks(from, ~empty_squares);

    Bitboard quiet = atk_bb & empty_squares;
    while (quiet)
    {
      *move_list++ = makeQuiet(from, bitboard::popLS1B(quiet));
      move_counter++;
    }

    Bitboard capture = atk_bb & opponent_pieces;
    while (capture)
    {
      *move_list++ = makeCapture(from, bitboard::popLS1B(capture));
      move_counter++;
    }
  }
}

} // namespace piece

} // namespace yak

namespace pieces {

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

} // namespace pieces
