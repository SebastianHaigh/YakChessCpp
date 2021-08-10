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

    pieces::WhitePawns WhitePawns = pieces::WhitePawns(white_pawns, empty_squares_ptr);

    Bitboard pawn_targets = 0;
    std::stack<pieces::SourceTargetPair> pawn_moves = WhitePawns.get_all_moves();
    while (!pawn_moves.empty()) {
        auto move = pawn_moves.top();
        pawn_moves.pop();
    }
    print_board(pawn_targets);
}
