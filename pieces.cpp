#include "pieces.h"
#include <memory>

namespace pieces {

std::shared_ptr<Piece> ChessMen::get_piece(Square square_index) {
    if (pawns->has_piece_on_square(square_index)) {
        return pawns;
    } else {
        return std::make_shared<NullPiece>();
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Implementation of Pawns
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

Bitboard Pawns::all_attacked_squares() {
    return colour->pawn_west_attack_targets(*board) | colour->pawn_east_attack_targets(*board);
};

Bitboard Pawns::single_push() {
    return colour->pawn_push_sources(*empty_squares) & *board;;
}

Bitboard Pawns::double_push() {
    Bitboard empty_targets, clear_to_target;
    empty_targets = colour->pawn_double_push_target() & *empty_squares;
    clear_to_target = colour->pawn_push_sources(empty_targets) & *empty_squares;
    return colour->pawn_push_sources(clear_to_target) & *board;
}

PawnTargets Pawns::single_pushes() {
    auto sources = single_push();
    auto targets = colour->pawn_push_targets(sources);

    return PawnTargets(sources, targets);
}

PawnTargets Pawns::double_pushes() {
    auto sources = double_push();
    auto targets = colour->pawn_double_push_target();

    return PawnTargets(sources, targets);
}
        
PawnTargets Pawns::west_captures() {
    auto sources = colour->pawn_west_attack_sources() & *board;
    auto targets = colour->pawn_west_attack_targets(sources);
    return PawnTargets(sources, targets);
}
        
PawnTargets Pawns::east_captures() {
    auto sources = colour->pawn_east_attack_sources() & *board;
    auto targets = colour->pawn_east_attack_targets(sources);
    return PawnTargets(sources, targets);
}

void Pawns::make_move(Square source, Square target) {
    auto source_bitboard = Bitboard(1) << source;
    auto target_bitboard = Bitboard(1) << target;
    auto source_and_target_bitboard = source_bitboard ^ target_bitboard;

    *board ^= source_and_target_bitboard;
    *empty_squares ^= source_and_target_bitboard;
    colour->make_move(source_and_target_bitboard);
}

bool Pawns::has_piece_on_square(Square square_index) {
    auto target_bitboard = Bitboard(1) << square_index;
    
    return (target_bitboard & *board) > 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Implementation of Sliding Pieces
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// SlidingPieces::SlidingPieces(std::shared_ptr <Bitboard> board_ptr, std::shared_ptr <Bitboard> empty_squares_ptr) {
//     board = board_ptr;
//     empty_squares = empty_squares_ptr;
// }

// std::stack <Move> SlidingPieces::quite_moves() {
//     // Calculating the quiet move (i.e., not capture moves);

//     Bitboard quiet_targets;
//     std::stack <Move> quite_moves;

//     std::stack <Move> potential_targets = targets();
    
//     while (!potential_targets.empty()) {
//         Move target = potential_targets.top();
//         // bit-wise AND with target and empty_squares results in the bitboard of 
//         // empty squares that this sliding piece can move to.
//         quiet_targets = target.get_target() & *empty_squares;
//         quite_moves.push(SerialMove(target.get_source(), quiet_targets));
//         potential_targets.pop();
//     }

//     return quite_moves;
// }

// std::stack <Move> SlidingPieces::targets() {

//     Bitboard targets = 0;
//     std::stack <Square> serialised_pieces = bitboard::scan_forward(*board);
//     std::stack <Move> moves;

//     while (!serialised_pieces.empty()) {
//         for (size_t i = 0; i < rays.size(); i++)
//         {
//             targets |= rays[i]->get(serialised_pieces.top(), ~*empty_squares);
//         }
//         moves.push(Move(serialised_pieces.top(), targets));
//         serialised_pieces.pop();
//     }
//     return moves;
// }


// ///////////////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////////////////////////
// // Implementation of Rooks
// ///////////////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////////////////////////

// Rooks::Rooks(std::shared_ptr <Bitboard> board_ptr, std::shared_ptr <Bitboard> empty_squares_ptr) : SlidingPieces(board_ptr, empty_squares_ptr) {
//     rays.push_back(std::make_unique <attacks::NorthRay>());
//     rays.push_back(std::make_unique <attacks::EastRay>());
//     rays.push_back(std::make_unique <attacks::SouthRay>());
//     rays.push_back(std::make_unique <attacks::WestRay>());
// }

// Bishops::Bishops(std::shared_ptr <Bitboard> board_ptr, std::shared_ptr <Bitboard> empty_squares_ptr) : SlidingPieces(board_ptr, empty_squares_ptr) {
//     rays.push_back(std::make_unique <attacks::NorthWestRay>());
//     rays.push_back(std::make_unique <attacks::NorthEastRay>());
//     rays.push_back(std::make_unique <attacks::SouthWestRay>());
//     rays.push_back(std::make_unique <attacks::SouthEastRay>());
// }

// Queens::Queens(std::shared_ptr <Bitboard> board_ptr, std::shared_ptr <Bitboard> empty_squares_ptr) : SlidingPieces(board_ptr, empty_squares_ptr) {
//     rays.push_back(std::make_unique <attacks::NorthRay>());
//     rays.push_back(std::make_unique <attacks::EastRay>());
//     rays.push_back(std::make_unique <attacks::SouthRay>());
//     rays.push_back(std::make_unique <attacks::WestRay>());
//     rays.push_back(std::make_unique <attacks::NorthWestRay>());
//     rays.push_back(std::make_unique <attacks::NorthEastRay>());
//     rays.push_back(std::make_unique <attacks::SouthWestRay>());
//     rays.push_back(std::make_unique <attacks::SouthEastRay>());
// }

// void Move::execute() {
//     piece_to_move.make_move(source, target);
// }

Bitboard White::pawn_push_targets(Bitboard sources) {
    return bitboard::north_one(sources);
}

Bitboard White::pawn_push_sources(Bitboard targets) {
    return bitboard::south_one(targets);
}

Bitboard White::pawn_east_attack_targets(Bitboard sources) {
    return bitboard::north_east_one(sources);
}

Bitboard White::pawn_east_attack_sources() {
    return bitboard::south_west_one(*opponent_pieces);
}

Bitboard White::pawn_west_attack_targets(Bitboard sources) {
    return bitboard::north_west_one(sources);
}

Bitboard White::pawn_west_attack_sources() {
    return bitboard::south_east_one(*opponent_pieces);
}

Bitboard White::pawn_double_push_target() {
    return bitboard::RANK_4;
}

void White::make_move(Bitboard source_and_target_bitboard) {
    *this_colour_pieces ^= source_and_target_bitboard;
}


Bitboard Black::pawn_push_targets(Bitboard sources) {
    return bitboard::south_one(sources);
}

Bitboard Black::pawn_push_sources(Bitboard targets) {
    return bitboard::north_one(targets);
}

Bitboard Black::pawn_east_attack_targets(Bitboard sources) {
    return bitboard::south_east_one(sources);
}

Bitboard Black::pawn_east_attack_sources() {
    return bitboard::north_west_one(*opponent_pieces);
}

Bitboard Black::pawn_west_attack_targets(Bitboard sources) {
    return bitboard::south_west_one(sources);
}

Bitboard Black::pawn_west_attack_sources() {
    return bitboard::north_east_one(*opponent_pieces);
}

Bitboard Black::pawn_double_push_target() {
    return bitboard::RANK_5;
}

void Black::make_move(Bitboard source_and_target_bitboard) {
    *this_colour_pieces ^= source_and_target_bitboard;
}

} // namespace pieces