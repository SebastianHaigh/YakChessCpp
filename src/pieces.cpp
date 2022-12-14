#include "pieces.h"

namespace yak {
namespace piece {

void generate_pawn_moves(PieceColour colour,
                         Move *move_list,
                         int &move_counter,
                         Bitboard pawn_positions,
                         Bitboard empty_squares,
                         Bitboard opponent_pieces)
{

  Bitboard promotable = (colour == PieceColour::BLACK ? bitboard::RANK_2 : bitboard::RANK_7) & pawn_positions;
  Bitboard not_promotable = pawn_positions & ~promotable;

  /* NOT PROMOTIONS              */
  /* --------------------------- */
  Bitboard sources = (colour == PieceColour::BLACK ? bitboard::shift<Direction::NORTH>(empty_squares) : bitboard::shift<Direction::SOUTH>(empty_squares)) & not_promotable;
  Bitboard targets = colour == PieceColour::BLACK ? bitboard::shift<Direction::SOUTH>(sources) : bitboard::shift<Direction::NORTH>(sources);
  while (sources)
  {
    *move_list++ = makeQuiet(bitboard::popLS1B(sources), bitboard::popLS1B(targets));
    move_counter++;
  }

  targets = ((colour == PieceColour::BLACK) ? bitboard::RANK_5 : bitboard::RANK_4) & empty_squares;
  sources = (colour == PieceColour::BLACK) ? bitboard::shift<Direction::NORTH>(targets) : bitboard::shift<Direction::SOUTH>(targets) & empty_squares;
  sources = (colour == PieceColour::BLACK) ? bitboard::shift<Direction::NORTH>(sources) : bitboard::shift<Direction::SOUTH>(sources) & not_promotable;
  while (sources)
  {
    *move_list++ = makeCapture(bitboard::popLS1B(sources), bitboard::popLS1B(targets));
    move_counter++;
  }

  sources = (colour == PieceColour::BLACK ? bitboard::shift<Direction::NORTH_WEST>(opponent_pieces) : bitboard::shift<Direction::SOUTH_WEST>(opponent_pieces)) & not_promotable;
  targets = colour == PieceColour::BLACK ? bitboard::shift<Direction::SOUTH_EAST>(sources) : bitboard::shift<Direction::NORTH_EAST>(sources);
  while (sources)
  {
    *move_list++ = makeCapture(bitboard::popLS1B(sources), bitboard::popLS1B(targets));
    move_counter++;
  }

  sources = (colour == PieceColour::BLACK ? bitboard::shift<Direction::NORTH_EAST>(opponent_pieces) : bitboard::shift<Direction::SOUTH_EAST>(opponent_pieces)) & not_promotable;
  targets = colour == PieceColour::BLACK ? bitboard::shift<Direction::SOUTH_WEST>(sources) : bitboard::shift<Direction::NORTH_WEST>(sources);
  while (sources)
  {
    *move_list++ = makeCapture(bitboard::popLS1B(sources), bitboard::popLS1B(targets));
    move_counter++;
  }

  // PROMOTIONS
  sources = (colour == PieceColour::BLACK ? bitboard::shift<Direction::NORTH>(empty_squares) : bitboard::shift<Direction::SOUTH>(empty_squares)) & promotable;
  targets = colour == PieceColour::BLACK ? bitboard::shift<Direction::SOUTH>(sources) : bitboard::shift<Direction::NORTH>(sources);
  while (sources)
  {
    Square from = bitboard::popLS1B(sources);
    Square to = bitboard::popLS1B(targets);
    *move_list++ = makeQuietPromotion(from, to, PieceType::KNIGHT);
    *move_list++ = makeQuietPromotion(from, to, PieceType::BISHOP);
    *move_list++ = makeQuietPromotion(from, to, PieceType::ROOK);
    *move_list++ = makeQuietPromotion(from, to, PieceType::QUEEN);
    move_counter += 4;
  }

  sources = (colour == PieceColour::BLACK ? bitboard::shift<Direction::NORTH_WEST>(opponent_pieces) : bitboard::shift<Direction::SOUTH_WEST>(opponent_pieces)) & promotable;
  targets = colour == PieceColour::BLACK ? bitboard::shift<Direction::SOUTH_EAST>(sources) : bitboard::shift<Direction::NORTH_EAST>(sources);
  while (sources)
  {
    Square from = bitboard::popLS1B(sources);
    Square to = bitboard::popLS1B(targets);
    *move_list++ = makeCapturePromotion(from, to, PieceType::KNIGHT);
    *move_list++ = makeCapturePromotion(from, to, PieceType::BISHOP);
    *move_list++ = makeCapturePromotion(from, to, PieceType::ROOK);
    *move_list++ = makeCapturePromotion(from, to, PieceType::QUEEN);
    move_counter += 4;
  }

  sources = (colour == PieceColour::BLACK ? bitboard::shift<Direction::NORTH_EAST>(empty_squares) : bitboard::shift<Direction::SOUTH_EAST>(empty_squares)) & promotable;
  targets = colour == PieceColour::BLACK ? bitboard::shift<Direction::SOUTH_WEST>(sources) : bitboard::shift<Direction::NORTH_WEST>(sources);
  while (sources)
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

void generateSlidingPieceMoves(const attackmap::BishopMap &attackMap,
                               Move *moveList,
                               int &moveCounter,
                               Bitboard piecePositions,
                               Bitboard emptySquares,
                               Bitboard opponentPieces)
{
  while (piecePositions)
  {
    Square from = bitboard::popLS1B(piecePositions);
    Bitboard attackBitboard = attackMap.attacks(from, ~emptySquares);

    Bitboard quiet = attackBitboard & emptySquares;
    while (quiet)
    {
      *moveList++ = makeQuiet(from, bitboard::popLS1B(quiet));
      moveCounter++;
    }

    Bitboard capture = attackBitboard & opponentPieces;
    while (capture)
    {
      *moveList++ = makeCapture(from, bitboard::popLS1B(capture));
      moveCounter++;
    }
  }
}

void generateSlidingPieceMoves(const attackmap::QueenMap &attackMap,
                               Move *moveList,
                               int &moveCounter,
                               Bitboard piecePositions,
                               Bitboard emptySquares,
                               Bitboard opponentPieces)
{
  while (piecePositions)
  {
    Square from = bitboard::popLS1B(piecePositions);
    Bitboard attackBitboard = attackMap.attacks(from, ~emptySquares);

    Bitboard quiet = attackBitboard & emptySquares;
    while (quiet)
    {
      *moveList++ = makeQuiet(from, bitboard::popLS1B(quiet));
      moveCounter++;
    }

    Bitboard capture = attackBitboard & opponentPieces;
    while (capture)
    {
      *moveList++ = makeCapture(from, bitboard::popLS1B(capture));
      moveCounter++;
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
  if (colour == PieceColour::WHITE)
  {
    return whitePieceTypeToFenChar(type);
  }
  else
  {
    return blackPieceTypeToFenChar(type);
  }
}

char blackPieceTypeToFenChar(const PieceType& type) {
  switch (type) {
    case PieceType::PAWN:   return 'p';
    case PieceType::KNIGHT: return 'n';
    case PieceType::BISHOP: return 'b';
    case PieceType::ROOK:   return 'r';
    case PieceType::QUEEN:  return 'q';
    case PieceType::KING:   return 'k';
    default:                return 'x'; // ERROR CASE, should never run
  }
}

char whitePieceTypeToFenChar(const PieceType& type) {
  switch (type) {
    case PieceType::PAWN:   return 'P';
    case PieceType::KNIGHT: return 'N';
    case PieceType::BISHOP: return 'B';
    case PieceType::ROOK:   return 'R';
    case PieceType::QUEEN:  return 'Q';
    case PieceType::KING:   return 'K';
    default:                return 'X'; // ERROR CASE, should never run
  }
}

} // namespace pieces