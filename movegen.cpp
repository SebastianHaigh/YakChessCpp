#include "movegen.h"

#include <memory>
#include <stack>

namespace move_generation {

std::vector<Move> MoveGenerator::generate_pseudo_legal_moves(Board& board) {
    if (board.to_move() == PieceColour::WHITE) {
        return pseudo_legal_moves(board, PieceColour::WHITE, PieceColour::BLACK);
    }
    else {
        return pseudo_legal_moves(board, PieceColour::BLACK, PieceColour::WHITE);
    }
    
}

std::vector<Move> MoveGenerator::pseudo_legal_moves(Board& board, PieceColour friendly, PieceColour enemy) {
    std::vector<Move> moves;
    
    std::unique_ptr<pieces::Pawns> pawns = factory.make_pawns(friendly);
    Bitboard pawns_bitboard = board.get_position(friendly, PieceType::PAWN);

    //auto single_pushes = pawns->single_push(pawns_bitboard, board.empty_squares());
    //auto double_pushes = pawns->double_push(pawns_bitboard, board.empty_squares());
    auto west_captures = pawns->west_captures(pawns_bitboard, board.get_position(enemy));
    auto east_captures = pawns->east_captures(pawns_bitboard, board.get_position(enemy));
    auto west_ep_captures = pawns->west_captures(pawns_bitboard, board.ep_target());
    auto east_ep_captures = pawns->east_captures(pawns_bitboard, board.ep_target());

    //moves = process_quiet_moves(board, single_pushes, moves);
    //moves = process_quiet_moves(board, double_pushes, moves);
    //moves = process_captures(board, west_captures, moves);
   // moves = process_captures(board, east_captures, moves);
    //moves = process_ep_captures(board, west_ep_captures, moves, friendly);
   // moves = process_ep_captures(board, east_ep_captures, moves, friendly);

    moves = piece_moves_and_captures(board, PieceType::KNIGHT, friendly, enemy, moves);
    moves = piece_moves_and_captures(board, PieceType::BISHOP, friendly, enemy, moves);
    moves = piece_moves_and_captures(board, PieceType::ROOK, friendly, enemy, moves);
    moves = piece_moves_and_captures(board, PieceType::QUEEN, friendly, enemy, moves);
    moves = piece_moves_and_captures(board, PieceType::KING, friendly, enemy, moves);
    return moves;
}

std::vector<Move> MoveGenerator::piece_moves_and_captures(Board& board, PieceType type, PieceColour friendly, PieceColour enemy, std::vector<Move> move_stack) {
    std::unique_ptr<pieces::Piece> piece = factory.make_piece(type);

    std::vector<std::pair<Square, Square>> quiet_moves = piece->quiet_moves(board.get_position(friendly, type), board.empty_squares());
    std::vector<std::pair<Square, Square>> captures = piece->captures(board.get_position(friendly, type), ~board.empty_squares(), board.get_position(enemy));

    for (auto move : quiet_moves) {
        move_stack.push_back(Move(move.first, move.second, board.get_piece_type_on(move.first)));
    }

    for (auto capture : captures) {
        Move this_capture = Move(capture.first, capture.second, board.get_piece_type_on(capture.first));
        this_capture.set_capture(board.get_piece_type_on(capture.second));

        move_stack.push_back(this_capture);
    }
    return move_stack;
}

std::vector<Move> MoveGenerator::process_quiet_moves(
    Board& board, pieces::PawnTargets targets, 
    std::vector<Move> moves) {
    auto serialised_sources = bitboard::scan_forward(targets.get_source());
    auto serialised_targets = bitboard::scan_forward(targets.get_target());

    auto serialised_moves = zip(serialised_sources, serialised_targets);

    for (auto move : serialised_moves) {
        if (target_is_promotion(move.second)) {
            Move this_move = Move(move.first, move.second, board.get_piece_type_on(move.first));
            this_move.set_promotion(PieceType::KNIGHT);
            moves.push_back(this_move);

            this_move.set_promotion(PieceType::BISHOP);
            moves.push_back(this_move);

            this_move.set_promotion(PieceType::ROOK);
            moves.push_back(this_move);

            this_move.set_promotion(PieceType::QUEEN);
            moves.push_back(this_move);
        }
        else {
            Move this_move = Move(move.first, move.second, board.get_piece_type_on(move.first));
            moves.push_back(this_move);
        }

    }

    return moves;
}

std::vector<Move> MoveGenerator::process_captures(Board& board, pieces::PawnTargets targets,
    std::vector<Move> moves) {
    auto serialised_sources = bitboard::scan_forward(targets.get_source());
    auto serialised_targets = bitboard::scan_forward(targets.get_target());

    auto serialised_captures = zip(serialised_sources, serialised_targets);

    for (auto capture : serialised_captures) {
        PieceType piece_to_move = board.get_piece_type_on(capture.first);
        PieceType op_piece = board.get_piece_type_on(capture.second);
        if (op_piece != PieceType::NULL_PIECE && piece_to_move != PieceType::NULL_PIECE) {
            auto move = Move(capture.first, capture.second, piece_to_move);
            move.set_capture(op_piece);
            moves.push_back(move);
        }
    }

    return moves;
}

std::vector<Move> MoveGenerator::process_ep_captures(Board& board, pieces::PawnTargets targets, std::vector<Move> moves, PieceColour move_colour) {
    std::vector<Square> serialised_sources = bitboard::scan_forward(targets.get_source());
    Bitboard target_piece_bitboard(0);
    // Ep target squares can only be on the 3rd and 6th ranks, if the target is on the 3rd rank then the piece to be captured is white, otherwise it is black.
    if (move_colour == PieceColour::BLACK) {
        // Black captures white
        target_piece_bitboard = bitboard::north_one(board.ep_target());
    }
    else {
        // White captures black
        target_piece_bitboard = bitboard::south_one(board.ep_target());
    }
    for (auto source : serialised_sources) {
        auto move = Move(source, board.ep_target_square(), PieceType::PAWN);
        move.set_en_passant(target_piece_bitboard);
        moves.push_back(move);
    }
    return moves;
}

std::vector<std::pair<Square, Square>> MoveGenerator::zip(std::vector<Square> sources, std::vector<Square> targets) {
    std::vector<std::pair<Square, Square>> zipped;
    for (int i = 0; i < sources.size(); i++) {
        zipped.push_back(std::make_pair(sources[i], targets[i]));
    }
    return zipped;
}

} // namespace move_generation