#include "bitboard.h"
#include "pieces.h"
#include "attackmaps.h"
#include <iostream>
#include <stdio.h>

using namespace bitboard;

int main(int, char**) {
    uint64_t this_board = RANK_7;
    uint64_t empty_squares = NOT_RANK_7;
    uint64_t black_rook_bb = RANK_3 & FILE_D;
    this_board |= black_rook_bb;
    empty_squares = ~this_board;
    std::shared_ptr <uint64_t> black_rook_ptr = std::make_shared <uint64_t> (black_rook_bb);
    std::shared_ptr <uint64_t> empty_squares_ptr = std::make_shared <uint64_t> (empty_squares);
    pieces::Rooks Rooks = pieces::Rooks(black_rook_ptr, empty_squares_ptr);
    std::stack <pieces::SerialMove> rook_moves = Rooks.quite_moves();
    while (!rook_moves.empty()) {
        pieces::SerialMove move = rook_moves.top();
        print_board(move.get_target());
        rook_moves.pop();
    }
}
