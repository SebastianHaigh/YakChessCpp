#include "bitboard.h"
#include "pieces.h"
#include "attackmaps.h"
#include <iostream>
#include <stdio.h>
#include <chrono>
#include "board.h"
#include <random>

using namespace bitboard;

int main(int, char**) {

    std::shared_ptr<Bitboard> black_pawns = std::make_shared<Bitboard> (RANK_7);
    std::shared_ptr<Bitboard> white_pawns = std::make_shared<Bitboard> (RANK_2);
    std::shared_ptr<Bitboard> black_pieces = std::make_shared<Bitboard> (*black_pawns);
    std::shared_ptr<Bitboard> white_pieces = std::make_shared<Bitboard> (*white_pawns);
    auto occupied_squares = *black_pawns ^ *white_pawns;
    std::shared_ptr<Bitboard> empty_squares_ptr = std::make_shared<Bitboard> (~occupied_squares);
    auto pawns = pieces::WhitePawns();

    auto pawn_moves = pawns.single_push(RANK_2, *empty_squares_ptr);

    //print_board(pawn_moves.get_target());

    
    
    CastlingRights cr("KQkq");
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int>  distr(0, 5);
    std::vector<PieceType> a;
    for (int i = 0; i < 1000000; i++) {
        a.push_back((PieceType) distr(generator));
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 1000000; i++) {
        Move move(56, 0, a[i]);
        cr.update(move, PieceColour::WHITE);
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    // To get the value of duration use the count()
    // member function on the duration object
    std::cout << duration.count() << std::endl;
}
