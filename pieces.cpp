#include "pieces.h"

namespace pieces {

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Implementation of Black Pieces
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

uint64_t BlackPieces::pawn_targets_from_sources(uint64_t sources) {
    return bitboard::south_one(sources);
}

uint64_t BlackPieces::pawn_sources_from_targets(uint64_t targets) {
    return bitboard::north_one(targets);
}

uint64_t BlackPieces::pawn_east_attacks(uint64_t sources) {
    return bitboard::south_east_one(sources);
}

uint64_t BlackPieces::pawn_west_attacks(uint64_t sources) {
    return bitboard::south_west_one(sources);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Implementation of White Pieces
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

uint64_t WhitePieces::pawn_targets_from_sources(uint64_t sources) {
    return bitboard::north_one(sources);
}

uint64_t WhitePieces::pawn_sources_from_targets(uint64_t targets) {
    return bitboard::south_one(targets);
}

uint64_t WhitePieces::pawn_east_attacks(uint64_t sources) {
    return bitboard::north_east_one(sources);
}

uint64_t WhitePieces::pawn_west_attacks(uint64_t sources) {
    return bitboard::north_west_one(sources);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Implementation of Black Pawns
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BlackPawns::BlackPawns(std::shared_ptr <uint64_t> board_ptr, std::shared_ptr <uint64_t> empty_squares_ptr) {
    board = board_ptr;
    empty_squares = empty_squares_ptr;
}

Move BlackPawns::single_push() {
    uint64_t sources, targets;
    sources = pawn_sources_from_targets(*empty_squares) & *board;
    targets = pawn_targets_from_sources(sources);
    Move move = Move(sources, targets);
    return move;
}

Move BlackPawns::double_push() {
    uint64_t sources, targets, empty_targets, clear_to_target;
    empty_targets = double_push_targets & *empty_squares;
    clear_to_target = pawn_sources_from_targets(empty_targets) & *empty_squares;
    sources = pawn_sources_from_targets(clear_to_target) & *board;
    targets = pawn_targets_from_sources(sources);
    targets = pawn_targets_from_sources(targets);
    return Move(sources, targets);
}

uint64_t BlackPawns::east_attack() {
    return bitboard::south_east_one(*board);
}
        
uint64_t BlackPawns::west_attack() {
    return bitboard::south_west_one(*board);
}
        
uint64_t BlackPawns::all_attack() {
    return west_attack() | east_attack();
}
        
Move BlackPawns::east_captures(uint64_t opponent_piece) {
    uint64_t sources, targets;
    sources = bitboard::north_west_one(opponent_piece) & *board;
    targets = bitboard::south_east_one(sources);
    return Move(sources, targets);
}
        
Move BlackPawns::west_captures(uint64_t opponent_piece) {
    uint64_t sources, targets;
    sources = bitboard::north_east_one(opponent_piece) & *board;
    targets = bitboard::south_west_one(sources);
    return Move(sources, targets);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Implementation of White Pawns
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

WhitePawns::WhitePawns(std::shared_ptr <uint64_t> board_ptr, std::shared_ptr <uint64_t> empty_squares_ptr) {
    board = board_ptr;
    empty_squares = empty_squares_ptr;
}

Move WhitePawns::single_push() {
    uint64_t sources, targets;
    sources = bitboard::north_one(*empty_squares) & *board;
    targets = bitboard::south_one(sources);
    return Move(sources, targets);
}

Move WhitePawns::double_push() {
    uint64_t sources, targets, empty_squares_on_rank_4, clear_to_rank_4;
    empty_squares_on_rank_4 = bitboard::RANK_4 & *empty_squares;
    clear_to_rank_4 = bitboard::south_one(empty_squares_on_rank_4) & *empty_squares;
    sources = bitboard::south_one(clear_to_rank_4) & *board;
    targets = bitboard::north_one(sources);
    targets = bitboard::north_one(targets);
    return Move(sources, targets);
}

uint64_t WhitePawns::east_attack() {
    return bitboard::north_east_one(*board);
}
        
uint64_t WhitePawns::west_attack() {
    return bitboard::north_west_one(*board);
}
        
uint64_t WhitePawns::all_attack() {
    return west_attack() | east_attack();
}
        
Move WhitePawns::east_captures(uint64_t opponent_piece) {
    uint64_t sources, targets;
    sources = bitboard::south_west_one(opponent_piece) & *board;
    targets = bitboard::north_east_one(sources);
    return Move(sources, targets);
}
        
Move WhitePawns::west_captures(uint64_t opponent_piece) {
    uint64_t sources, targets;
    sources = bitboard::south_east_one(opponent_piece) & *board;
    targets = bitboard::north_west_one(sources);
    return Move(sources, targets);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Implementation of Sliding Pieces
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

SlidingPieces::SlidingPieces(std::shared_ptr <uint64_t> board_ptr, std::shared_ptr <uint64_t> empty_squares_ptr) {
    board = board_ptr;
    empty_squares = empty_squares_ptr;
}

std::stack <SerialMove> SlidingPieces::quite_moves() {
    // Calculating the quiet move (i.e., not capture moves);

    uint64_t quiet_targets;
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

    uint64_t targets = 0;
    std::stack <int> serialised_pieces = bitboard::scan_forward(*board);
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

Rooks::Rooks(std::shared_ptr <uint64_t> board_ptr, std::shared_ptr <uint64_t> empty_squares_ptr) : SlidingPieces(board_ptr, empty_squares_ptr) {
    rays.push_back(std::make_unique <attacks::NorthRay>());
    rays.push_back(std::make_unique <attacks::EastRay>());
    rays.push_back(std::make_unique <attacks::SouthRay>());
    rays.push_back(std::make_unique <attacks::WestRay>());
}

Bishops::Bishops(std::shared_ptr <uint64_t> board_ptr, std::shared_ptr <uint64_t> empty_squares_ptr) : SlidingPieces(board_ptr, empty_squares_ptr) {
    rays.push_back(std::make_unique <attacks::NorthWestRay>());
    rays.push_back(std::make_unique <attacks::NorthEastRay>());
    rays.push_back(std::make_unique <attacks::SouthWestRay>());
    rays.push_back(std::make_unique <attacks::SouthEastRay>());
}

Queens::Queens(std::shared_ptr <uint64_t> board_ptr, std::shared_ptr <uint64_t> empty_squares_ptr) : SlidingPieces(board_ptr, empty_squares_ptr) {
    rays.push_back(std::make_unique <attacks::NorthRay>());
    rays.push_back(std::make_unique <attacks::EastRay>());
    rays.push_back(std::make_unique <attacks::SouthRay>());
    rays.push_back(std::make_unique <attacks::WestRay>());
    rays.push_back(std::make_unique <attacks::NorthWestRay>());
    rays.push_back(std::make_unique <attacks::NorthEastRay>());
    rays.push_back(std::make_unique <attacks::SouthWestRay>());
    rays.push_back(std::make_unique <attacks::SouthEastRay>());
}

Move::Move(uint64_t source_squares, uint64_t target_squares) {
    sources = source_squares;
    targets = target_squares;
}

uint64_t Move::get_source() {
    return sources;
}

uint64_t Move::get_target() {
    return targets;
}


SerialMove::SerialMove(int source_square, uint64_t target_squares) : Move(source_square, target_squares){
    source = source_square;
    targets = target_squares;
}

const int SerialMove::get_source() {
    return source;
}

} // namespace pieces