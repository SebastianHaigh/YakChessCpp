#include "bitboard.h"
#include "pieces.h"
#include "attackmaps.h"
#include <iostream>
#include <stdio.h>

using namespace bitboard;

int main(int, char**) {
    Bitboard this_board = RANK_7;
    Bitboard empty_squares = NOT_RANK_7;
    Bitboard black_rook_bb = RANK_3 & FILE_D;
    this_board |= black_rook_bb;
    empty_squares = ~this_board;

    std::shared_ptr<Bitboard> white_pawns = std::make_shared<Bitboard> (RANK_2);
    std::shared_ptr<Bitboard> black_rook_ptr = std::make_shared<Bitboard> (black_rook_bb);
    std::shared_ptr<Bitboard> empty_squares_ptr = std::make_shared<Bitboard> (empty_squares);

    pieces::Rooks Rooks = pieces::Rooks(black_rook_ptr, empty_squares_ptr);
    pieces::Bishops Bishops = pieces::Bishops(black_rook_ptr, empty_squares_ptr);
    pieces::Queens Queens = pieces::Queens(black_rook_ptr, empty_squares_ptr);
    pieces::BlackPawns WhitePawns = pieces::BlackPawns(white_pawns, empty_squares_ptr);

    std::stack<pieces::SerialMove> rook_moves = Rooks.quite_moves();
    while (!rook_moves.empty()) {
        pieces::SerialMove move = rook_moves.top();
        print_board(move.get_target());
        rook_moves.pop();
    }
    std::stack <pieces::SerialMove> bishop_moves = Bishops.quite_moves();
    while (!bishop_moves.empty()) {
        pieces::SerialMove move = bishop_moves.top();
        print_board(move.get_target());
        bishop_moves.pop();
    }
    std::stack <pieces::SerialMove> queen_moves = Queens.quite_moves();
    while (!queen_moves.empty()) {
        pieces::SerialMove move = queen_moves.top();
        print_board(move.get_target());
        queen_moves.pop();
    }
    Bitboard pawn_targets = 0;
    std::stack<pieces::Move> pawn_moves = WhitePawns.get_all_moves();
    while (!pawn_moves.empty()) {
        pieces::Move move = pawn_moves.top();
        pawn_targets |= move.get_target();
        pawn_moves.pop();
    }
    print_board(pawn_targets);
}
