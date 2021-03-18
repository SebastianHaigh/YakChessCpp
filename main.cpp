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

    std::shared_ptr <Bitboard> black_rook_ptr = std::make_shared <Bitboard> (black_rook_bb);
    std::shared_ptr <Bitboard> empty_squares_ptr = std::make_shared <Bitboard> (empty_squares);

    pieces::Rooks Rooks = pieces::Rooks(black_rook_ptr, empty_squares_ptr);
    pieces::Bishops Bishops = pieces::Bishops(black_rook_ptr, empty_squares_ptr);
    pieces::Queens Queens = pieces::Queens(black_rook_ptr, empty_squares_ptr);

    std::stack <pieces::SerialMove> rook_moves = Rooks.quite_moves();
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
}
