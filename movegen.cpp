#include "movegen.h"

#include <memory>
#include <stack>

namespace move_generation {

void PseudoLegalMove::execute() {
    piece_to_move->make_move(source, target);
    move_made = 1;
}

void PseudoLegalMove::undo() {
    if (move_made) {
        piece_to_move->make_move(source, target);
        move_made = 0;
    }
}

void PseudoLegalCapture::execute() {
    piece_to_move->make_move(source, target);
    piece_to_capture->make_capture(target);
    move_made = 1;
}

void PseudoLegalCapture::undo() {
    if (move_made) {
        piece_to_capture->make_capture(target);
        piece_to_move->make_move(source, target);
        move_made = 0;
    }
}


std::stack<std::shared_ptr<Move>> MoveGenerator::generate_pseudo_legal_moves() {
    if (side_to_move) {
        return pseudo_legal_moves(white_pieces, black_pieces);
    } else {
        return pseudo_legal_moves(black_pieces, white_pieces);
    }
}

std::stack<std::shared_ptr<Move>> MoveGenerator::pseudo_legal_moves(pieces::ChessMen friendly_pieces, pieces::ChessMen opponent_pieces) {
    std::stack<std::shared_ptr<Move>> move_stack;
    std::shared_ptr<pieces::Pawns> pawns = friendly_pieces.get_pawns();

    auto single_pushes = pawns->single_pushes();
    auto double_pushes = pawns->double_pushes();
    auto west_captures = pawns->west_captures();
    auto east_captures = pawns->east_captures();

    move_stack = process_quiet_moves(pawns, single_pushes, move_stack);
    move_stack = process_quiet_moves(pawns, double_pushes, move_stack);
    move_stack = process_captures(pawns, west_captures, move_stack, opponent_pieces);
    move_stack = process_captures(pawns, east_captures, move_stack, opponent_pieces);

    return move_stack;
}

std::stack<std::shared_ptr<Move>> MoveGenerator::process_quiet_moves(
    std::shared_ptr<pieces::Piece> piece_to_move, pieces::PawnTargets targets, 
    std::stack<std::shared_ptr<Move>> move_stack) {
    auto serialised_sources = bitboard::scan_forward(targets.get_source());
    auto serialised_targets = bitboard::scan_forward(targets.get_target());

    while (!serialised_sources.empty()) {

        auto move = std::make_shared<PseudoLegalMove>(piece_to_move, 
            serialised_sources.top(), 
            serialised_targets.top());

        serialised_sources.pop();
        serialised_targets.pop();
        move_stack.push(move);
    }

    return move_stack;
}

std::stack<std::shared_ptr<Move>> MoveGenerator::process_captures(std::shared_ptr<pieces::Piece> piece_to_move, pieces::PawnTargets targets, 
    std::stack<std::shared_ptr<Move>> move_stack, pieces::ChessMen opponent_pieces) {
    auto serialised_sources = bitboard::scan_forward(targets.get_source());
    auto serialised_targets = bitboard::scan_forward(targets.get_target());

    while (!serialised_sources.empty()) {
        auto op_piece = opponent_pieces.get_piece(serialised_targets.top());
        if (!op_piece->is_null()) {
            auto capture = std::make_shared<PseudoLegalCapture>(
                piece_to_move, 
                serialised_sources.top(), 
                serialised_targets.top(), 
                op_piece);
            move_stack.push(capture);
        }

        serialised_sources.pop();
        serialised_targets.pop();
    }

    return move_stack;
}

} // namespace move_generation