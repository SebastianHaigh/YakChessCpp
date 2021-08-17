#include "bitboard.h"
#include "pieces.h"
#include "attackmaps.h"
#include <iostream>
#include <stdio.h>

using namespace bitboard;

int main(int, char**) {
    std::shared_ptr<Bitboard> black_pawns = std::make_shared<Bitboard> (RANK_7);
    std::shared_ptr<Bitboard> white_pawns = std::make_shared<Bitboard> (RANK_2);
    std::shared_ptr<Bitboard> black_pieces = std::make_shared<Bitboard> (*black_pawns);
    std::shared_ptr<Bitboard> white_pieces = std::make_shared<Bitboard> (*white_pawns);
    auto occupied_squares = *black_pawns ^ *white_pawns;
    std::shared_ptr<Bitboard> empty_squares_ptr = std::make_shared<Bitboard> (~occupied_squares);

    auto WhitePieces = std::make_shared<pieces::White>(white_pieces, black_pieces);
    auto WhitePawns = pieces::Pawns(empty_squares_ptr, WhitePieces);
    WhitePawns.standard_starting_position();

    auto pawn_moves = WhitePawns.single_pushes();

    print_board(pawn_moves.get_target());
}
