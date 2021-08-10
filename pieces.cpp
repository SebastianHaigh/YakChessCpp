#include "pieces.h"
#include <memory>

namespace pieces {

Pieces::Pieces(std::shared_ptr<Bitboard> board_ptr,
               std::shared_ptr<Bitboard> empty_squares_ptr) {
    board = board_ptr;
    empty_squares = empty_squares_ptr;
}

std::stack<SourceTargetPair> Pieces::get_all_moves() {
    std::stack<SourceTargetPair> moves;
    return moves;
}

// std::stack<Move> Pieces::get_quiet_moves() {
//     std::stack<Move> moves;
//     return moves;
// }

// std::stack<Move> Pieces::get_captures(Bitboard opponent_piece) {
//     std::stack<Move> moves;
//     return moves;
// }

Bitboard Pieces::get_all_attacks() {
    return Bitboard(0);
}

void Pieces::make_move(Square source, Square target) {
    Bitboard source_bitboard, target_bitboard, source_target_bitboard;
    source_bitboard = Bitboard(1) << source;
    target_bitboard = Bitboard(1) << target;
    source_target_bitboard = source_bitboard ^ target_bitboard;
    *board ^= source_target_bitboard;
    *empty_squares ^=  source_target_bitboard;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Implementation of Pawns
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

Pawns::Pawns(std::shared_ptr<Bitboard> board_ptr,
             std::shared_ptr<Bitboard> empty_squares_ptr) 
             : Pieces (board_ptr, empty_squares_ptr)  {

}

std::stack<SourceTargetPair> Pawns::get_all_moves(){
    std::stack<SourceTargetPair> moves;
    std::stack<Square> serial_sources, serial_targets;
    Bitboard sources, targets;

    sources = single_push();
    targets = targets_from_sources(sources);
    serial_sources = bitboard::scan_forward(sources);
    serial_targets = bitboard::scan_forward(targets);
    while (!serial_sources.empty()) {
        moves.push(SourceTargetPair(serial_sources.top(), serial_targets.top()));
        serial_sources.pop();
        serial_targets.pop();
    }

    sources = double_push();
    targets = targets_from_sources(sources);
    serial_sources = bitboard::scan_forward(sources);
    serial_targets = bitboard::scan_forward(targets);
    while (!serial_sources.empty()) {
        moves.push(SourceTargetPair(serial_sources.top(), serial_targets.top()));
        serial_sources.pop();
        serial_targets.pop();
    }
    return moves;
}

// std::stack<Move> Pawns::get_quiet_moves() {
//     std::stack<Move> moves;
//     //moves.push(single_push());
//     //moves.push(double_push());
//     return moves;
// }

// std::stack<Move> Pawns::get_captures(Bitboard opponent_piece) {
//     std::stack<Move> moves;
//     moves.push(Move(this, 0, 0));
//     // moves.push(east_captures(opponent_piece));
//     // moves.push(west_captures(opponent_piece));
//     return moves;
// };
        
Bitboard Pawns::get_all_attacks() {
    return west_attack() | east_attack();
};

Bitboard Pawns::single_push() {
    Bitboard sources;
    sources = sources_from_targets(*empty_squares) & *board;
    return sources;
}

Bitboard Pawns::double_push() {
    Bitboard sources, empty_targets, clear_to_target;
    empty_targets = double_push_target() & *empty_squares;
    clear_to_target = sources_from_targets(empty_targets) & *empty_squares;
    sources = sources_from_targets(clear_to_target) & *board;
    return sources;
}

Bitboard Pawns::all_attack() {
    return west_attack() | east_attack();
}
        
// Bitboard Pawns::east_captures(Bitboard opponent_piece) {
//     Bitboard sources, targets;
//     sources = bitboard::north_west_one(opponent_piece) & *board;
//     //targets = bitboard::south_east_one(sources);
//     return sources;
// }
        
// Bitboard Pawns::west_captures(Bitboard opponent_piece) {
//     Bitboard sources, targets;
//     sources = bitboard::north_east_one(opponent_piece) & *board;
//     //targets = bitboard::south_west_one(sources);
//     return sources;
// }

Bitboard Pawns::targets_from_sources(Bitboard sources) { return bitboard::EMPTY; }

Bitboard Pawns::sources_from_targets(Bitboard targets) { return bitboard::EMPTY; }

Bitboard Pawns::east_attack() { return bitboard::EMPTY; }
        
Bitboard Pawns::west_attack() { return bitboard::EMPTY; }

Bitboard Pawns::double_push_target() { return bitboard::EMPTY; }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Implementation of White Pieces
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

WhitePawns::WhitePawns(std::shared_ptr<Bitboard> board_ptr,
                       std::shared_ptr<Bitboard> empty_squares_ptr) 
                       : Pawns (board_ptr, empty_squares_ptr)  {

}

Bitboard WhitePawns::targets_from_sources(Bitboard sources) {
    return bitboard::north_one(sources);
}

Bitboard WhitePawns::sources_from_targets(Bitboard targets) {
    return bitboard::south_one(targets);
}

Bitboard WhitePawns::east_attack() {
    return bitboard::north_east_one(*board);
}
        
Bitboard WhitePawns::west_attack() {
    return bitboard::north_west_one(*board);
}

Bitboard WhitePawns::double_push_target() {
    return bitboard::RANK_4;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Implementation of Black Pawns
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BlackPawns::BlackPawns(std::shared_ptr<Bitboard> board_ptr,
                       std::shared_ptr<Bitboard> empty_squares_ptr) 
                       : Pawns (board_ptr, empty_squares_ptr)  {

}

Bitboard BlackPawns::targets_from_sources(Bitboard sources) {
    return bitboard::south_one(sources);
}

Bitboard BlackPawns::sources_from_targets(Bitboard targets) {
    return bitboard::north_one(targets);
}

Bitboard BlackPawns::east_attack() {
    return bitboard::south_east_one(*board);
}
        
Bitboard BlackPawns::west_attack() {
    return bitboard::south_west_one(*board);
}

Bitboard BlackPawns::double_push_target() {
    return bitboard::RANK_5;
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

void Move::execute() {
    piece_to_move->make_move(source, target);
}

} // namespace pieces