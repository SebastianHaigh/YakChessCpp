#include "pieces.h"
#include <memory>

namespace pieces {

Pieces::Pieces(std::shared_ptr<Bitboard> board_ptr,
               std::shared_ptr<Bitboard> empty_squares_ptr) {
    board = board_ptr;
    empty_squares = empty_squares_ptr;
}

std::stack<Move> Pieces::get_all_moves() {
    std::stack<Move> moves;
    return moves;
}

std::stack<Move> Pieces::get_quiet_moves() {
    std::stack<Move> moves;
    return moves;
}

std::stack<Move> Pieces::get_captures() {
    std::stack<Move> moves;
    return moves;
}

Bitboard Pieces::get_all_attacks() {
    Bitboard targets;
    return targets;
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

std::stack<Move> Pawns::get_all_moves(){
    std::stack<Move> moves;
    moves.push(single_push());
    moves.push(double_push());
    return moves;
}

std::stack<Move> Pawns::get_quiet_moves() {
    std::stack<Move> moves;
    moves.push(single_push());
    moves.push(double_push());
    return moves;
}

std::stack<Move> Pawns::get_captures() {
    std::stack<Move> moves;
    return moves;
};
        
Bitboard Pawns::get_all_attacks() {
    return west_attack() | east_attack();
};

Move Pawns::single_push() {
    Bitboard sources, targets;
    sources = sources_from_targets(*empty_squares) & *board;
    targets = targets_from_sources(sources);
    Move move = Move(sources, targets);
    return move;
}

Move Pawns::double_push() {
    Bitboard sources, targets, empty_targets, clear_to_target;
    empty_targets = double_push_target() & *empty_squares;
    clear_to_target = sources_from_targets(empty_targets) & *empty_squares;
    sources = sources_from_targets(clear_to_target) & *board;
    targets = targets_from_sources(sources);
    targets = targets_from_sources(targets);
    return Move(sources, targets);
}

Bitboard Pawns::all_attack() {
    return west_attack() | east_attack();
}
        
Move Pawns::east_captures(uint64_t opponent_piece) {
    Bitboard sources, targets;
    sources = bitboard::north_west_one(opponent_piece) & *board;
    targets = bitboard::south_east_one(sources);
    return Move(sources, targets);
}
        
Move Pawns::west_captures(uint64_t opponent_piece) {
    Bitboard sources, targets;
    sources = bitboard::north_east_one(opponent_piece) & *board;
    targets = bitboard::south_west_one(sources);
    return Move(sources, targets);
}

Bitboard Pawns::targets_from_sources(Bitboard sources) {
    return bitboard::EMPTY;
}

Bitboard Pawns::sources_from_targets(Bitboard targets) {
    return bitboard::EMPTY;
}

Bitboard Pawns::east_attack() {
    return bitboard::EMPTY;
}
        
Bitboard Pawns::west_attack() {
    return bitboard::EMPTY;
}

Bitboard Pawns::double_push_target() {
    return bitboard::EMPTY;
}

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

SlidingPieces::SlidingPieces(std::shared_ptr <Bitboard> board_ptr, std::shared_ptr <Bitboard> empty_squares_ptr) {
    board = board_ptr;
    empty_squares = empty_squares_ptr;
}

std::stack <SerialMove> SlidingPieces::quite_moves() {
    // Calculating the quiet move (i.e., not capture moves);

    Bitboard quiet_targets;
    std::stack <SerialMove> quite_moves;

    std::stack <SerialMove> potential_targets = targets();
    
    while (!potential_targets.empty()) {
        SerialMove target = potential_targets.top();
        // bit-wise AND with target and empty_squares results in the bitboard of 
        // empty squares that this sliding piece can move to.
        quiet_targets = target.get_target() & *empty_squares;
        quite_moves.push(SerialMove(target.get_source(), quiet_targets));
        potential_targets.pop();
    }

    return quite_moves;
}

std::stack <SerialMove> SlidingPieces::targets() {

    Bitboard targets = 0;
    std::stack <Square> serialised_pieces = bitboard::scan_forward(*board);
    std::stack <SerialMove> moves;

    while (!serialised_pieces.empty()) {
        for (size_t i = 0; i < rays.size(); i++)
        {
            targets |= rays[i]->get(serialised_pieces.top(), ~*empty_squares);
        }
        moves.push(SerialMove(serialised_pieces.top(), targets));
        serialised_pieces.pop();
    }
    return moves;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Implementation of Rooks
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

Rooks::Rooks(std::shared_ptr <Bitboard> board_ptr, std::shared_ptr <Bitboard> empty_squares_ptr) : SlidingPieces(board_ptr, empty_squares_ptr) {
    rays.push_back(std::make_unique <attacks::NorthRay>());
    rays.push_back(std::make_unique <attacks::EastRay>());
    rays.push_back(std::make_unique <attacks::SouthRay>());
    rays.push_back(std::make_unique <attacks::WestRay>());
}

Bishops::Bishops(std::shared_ptr <Bitboard> board_ptr, std::shared_ptr <Bitboard> empty_squares_ptr) : SlidingPieces(board_ptr, empty_squares_ptr) {
    rays.push_back(std::make_unique <attacks::NorthWestRay>());
    rays.push_back(std::make_unique <attacks::NorthEastRay>());
    rays.push_back(std::make_unique <attacks::SouthWestRay>());
    rays.push_back(std::make_unique <attacks::SouthEastRay>());
}

Queens::Queens(std::shared_ptr <Bitboard> board_ptr, std::shared_ptr <Bitboard> empty_squares_ptr) : SlidingPieces(board_ptr, empty_squares_ptr) {
    rays.push_back(std::make_unique <attacks::NorthRay>());
    rays.push_back(std::make_unique <attacks::EastRay>());
    rays.push_back(std::make_unique <attacks::SouthRay>());
    rays.push_back(std::make_unique <attacks::WestRay>());
    rays.push_back(std::make_unique <attacks::NorthWestRay>());
    rays.push_back(std::make_unique <attacks::NorthEastRay>());
    rays.push_back(std::make_unique <attacks::SouthWestRay>());
    rays.push_back(std::make_unique <attacks::SouthEastRay>());
}

Move::Move(Bitboard source_squares, Bitboard target_squares) {
    sources = source_squares;
    targets = target_squares;
}

Bitboard Move::get_source() {
    return sources;
}

Bitboard Move::get_target() {
    return targets;
}


SerialMove::SerialMove(Square source_square, Bitboard target_squares) : Move(source_square, target_squares){
    source = source_square;
    targets = target_squares;
}

const Square SerialMove::get_source() {
    return source;
}

} // namespace pieces