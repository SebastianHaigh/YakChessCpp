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
  Bitboard sources = (colour == PieceColour::BLACK ? bitboard::shift<Direction::NORTH>(empty_squares) : bitboard::south_one(empty_squares)) & not_promotable;
  Bitboard targets = colour == PieceColour::BLACK ? bitboard::south_one(sources) : bitboard::north_one(sources);
  while (sources)
  {
    *move_list++ = make_quiet(bitboard::pop_LS1B(sources), bitboard::pop_LS1B(targets));
    move_counter++;
  }

  targets = ((colour == PieceColour::BLACK) ? bitboard::RANK_5 : bitboard::RANK_4) & empty_squares;
  sources = (colour == PieceColour::BLACK) ? bitboard::north_one(targets) : bitboard::south_one(targets) & empty_squares;
  sources = (colour == PieceColour::BLACK) ? bitboard::north_one(sources) : bitboard::south_one(sources) & not_promotable;
  while (sources)
  {
    *move_list++ = make_capture(bitboard::pop_LS1B(sources), bitboard::pop_LS1B(targets));
    move_counter++;
  }

  sources = (colour == PieceColour::BLACK ? bitboard::north_west_one(opponent_pieces) : bitboard::south_west_one(opponent_pieces)) & not_promotable;
  targets = colour == PieceColour::BLACK ? bitboard::south_east_one(sources) : bitboard::north_east_one(sources);
  while (sources)
  {
    *move_list++ = make_capture(bitboard::pop_LS1B(sources), bitboard::pop_LS1B(targets));
    move_counter++;
  }

  sources = (colour == PieceColour::BLACK ? bitboard::north_east_one(opponent_pieces) : bitboard::south_east_one(opponent_pieces)) & not_promotable;
  targets = colour == PieceColour::BLACK ? bitboard::south_west_one(sources) : bitboard::north_west_one(sources);
  while (sources)
  {
    *move_list++ = make_capture(bitboard::pop_LS1B(sources), bitboard::pop_LS1B(targets));
    move_counter++;
  }

  // PROMOTIONS
  sources = (colour == PieceColour::BLACK ? bitboard::north_one(empty_squares) : bitboard::south_one(empty_squares)) & promotable;
  targets = colour == PieceColour::BLACK ? bitboard::south_one(sources) : bitboard::north_one(sources);
  while (sources)
  {
    Square from = bitboard::pop_LS1B(sources);
    Square to = bitboard::pop_LS1B(targets);
    *move_list++ = make_quiet_promotion(from, to, PieceType::KNIGHT);
    *move_list++ = make_quiet_promotion(from, to, PieceType::BISHOP);
    *move_list++ = make_quiet_promotion(from, to, PieceType::ROOK);
    *move_list++ = make_quiet_promotion(from, to, PieceType::QUEEN);
    move_counter += 4;
  }

  sources = (colour == PieceColour::BLACK ? bitboard::north_west_one(opponent_pieces) : bitboard::south_west_one(opponent_pieces)) & promotable;
  targets = colour == PieceColour::BLACK ? bitboard::south_east_one(sources) : bitboard::north_east_one(sources);
  while (sources)
  {
    Square from = bitboard::pop_LS1B(sources);
    Square to = bitboard::pop_LS1B(targets);
    *move_list++ = make_capture_promotion(from, to, PieceType::KNIGHT);
    *move_list++ = make_capture_promotion(from, to, PieceType::BISHOP);
    *move_list++ = make_capture_promotion(from, to, PieceType::ROOK);
    *move_list++ = make_capture_promotion(from, to, PieceType::QUEEN);
    move_counter += 4;
  }

  sources = (colour == PieceColour::BLACK ? bitboard::north_east_one(empty_squares) : bitboard::south_east_one(empty_squares)) & promotable;
  targets = colour == PieceColour::BLACK ? bitboard::south_west_one(sources) : bitboard::north_west_one(sources);
  while (sources)
  {
    Square from = bitboard::pop_LS1B(sources);
    Square to = bitboard::pop_LS1B(targets);
    *move_list++ = make_capture_promotion(from, to, PieceType::KNIGHT);
    *move_list++ = make_capture_promotion(from, to, PieceType::BISHOP);
    *move_list++ = make_capture_promotion(from, to, PieceType::ROOK);
    *move_list++ = make_capture_promotion(from, to, PieceType::QUEEN);
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
    Square from = bitboard::pop_LS1B(piece_positions);
    Bitboard atk_bb = atk_map.attacks(from, ~empty_squares);

    Bitboard quiet = atk_bb & empty_squares;
    while (quiet)
    {
      *move_list++ = make_quiet(from, bitboard::pop_LS1B(quiet));
      move_counter++;
    }

    Bitboard capture = atk_bb & opponent_pieces;
    while (capture)
    {
      *move_list++ = make_capture(from, bitboard::pop_LS1B(capture));
      move_counter++;
    }
  }
}

void generate_sliding_piece_moves(const attackmap::BishopMap &atk_map,
                                  Move *move_list,
                                  int &move_counter,
                                  Bitboard piece_positions,
                                  Bitboard empty_squares,
                                  Bitboard opponent_pieces)
{
  while (piece_positions)
  {
    Square from = bitboard::pop_LS1B(piece_positions);
    Bitboard atk_bb = atk_map.attacks(from, ~empty_squares);

    Bitboard quiet = atk_bb & empty_squares;
    while (quiet)
    {
      *move_list++ = make_quiet(from, bitboard::pop_LS1B(quiet));
      move_counter++;
    }

    Bitboard capture = atk_bb & opponent_pieces;
    while (capture)
    {
      *move_list++ = make_capture(from, bitboard::pop_LS1B(capture));
      move_counter++;
    }
  }
}

void generate_sliding_piece_moves(const attackmap::QueenMap &atk_map,
                                  Move *move_list,
                                  int &move_counter,
                                  Bitboard piece_positions,
                                  Bitboard empty_squares,
                                  Bitboard opponent_pieces)
{
  while (piece_positions)
  {
    Square from = bitboard::pop_LS1B(piece_positions);
    Bitboard atk_bb = atk_map.attacks(from, ~empty_squares);

    Bitboard quiet = atk_bb & empty_squares;
    while (quiet)
    {
      *move_list++ = make_quiet(from, bitboard::pop_LS1B(quiet));
      move_counter++;
    }

    Bitboard capture = atk_bb & opponent_pieces;
    while (capture)
    {
      *move_list++ = make_capture(from, bitboard::pop_LS1B(capture));
      move_counter++;
    }
  }
}

} // namespace piece

} // namespace yak

namespace pieces {

    PieceColour other_colour(PieceColour colour) {
        if (colour == PieceColour::WHITE) {
            return PieceColour::BLACK;
        }
        else if (colour == PieceColour::BLACK) {
            return PieceColour::WHITE;
        }
        else {
            return PieceColour::NULL_COLOUR;
        }
    }

    PieceType fen_char_to_piece_type(const char fen_char) {
        switch (fen_char) {
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

    PieceColour fen_char_to_piece_colour(const char fen_char) {
        switch (fen_char) {
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

    char piece_to_fen_char(const PieceType type, const PieceColour colour) {
        if (colour == PieceColour::WHITE) {
            return white_piece_type_to_fen_char(type);
        }
        else {
            return black_piece_type_to_fen_char(type);
        }
    }

    char black_piece_type_to_fen_char(const PieceType type) {
        switch (type) {
            case PieceType::PAWN:
                return 'p';
            case PieceType::KNIGHT:
                return 'n';
            case PieceType::BISHOP:
                return 'b';
            case PieceType::ROOK:
                return 'r';
            case PieceType::QUEEN:
                return 'q';
            case PieceType::KING:
                return 'k';
            default:
                return 'x'; // ERROR CASE, should never run
        }
    }

    char white_piece_type_to_fen_char(const PieceType type) {
        switch (type) {
            case PieceType::PAWN:
                return 'P';
            case PieceType::KNIGHT:
                return 'N';
            case PieceType::BISHOP:
                return 'B';
            case PieceType::ROOK:
                return 'R';
            case PieceType::QUEEN:
                return 'Q';
            case PieceType::KING:
                return 'K';
            default:
                return 'X'; // ERROR CASE, should never run
        }
    }


} // namespace pieces