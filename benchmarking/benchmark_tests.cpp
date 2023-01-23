#include "benchmarking.h"
#include "benchmark_tests.h"
#include "../src/pieces.h"
#include "../src/board.h"
#include "../src/attackmaps.h"
#include "../src/magic.h"

#include <vector>
#include <iostream>
#include <random>

void PAWN_MOVE_GENERATION() {

  yak::Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  {
    std::cout << "---------------------------" << std::endl;
    std::cout << "PAWN_MOVE_GENERATION" << std::endl;
    std::cout << "---------------------------" << std::endl;
    Timer timer;
    for (int i = 0; i < 1000; i++) {
      board.generateMoves();
    }

  }


  {
    yak::Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
    std::cout << "---------------------------" << std::endl;
    std::cout << "PAWN_MOVE_GENERATION" << std::endl;
    std::cout << "---------------------------" << std::endl;
    Timer timer;
    for (int i = 0; i < 1000; i++) {
      board.generateMoves();
    }

  }


}

void KNIGHT_MOVE_GENERATION() {




}

void ROOK_MOVE_GENERATION() {




  {
    std::cout << "---------------------------" << std::endl;
    std::cout << "GENERATE ALL MOVES METAFUNCTIONS" << std::endl;
    std::cout << "---------------------------" << std::endl;
    yak::piece::Move move_list[330];
    Bitboard pawns = yak::bitboard::RANK_2;
    Bitboard rooks = yak::bitboard::RANK_1 & (yak::bitboard::FILE_A | yak::bitboard::FILE_H);
    Bitboard knights = yak::bitboard::RANK_1 & (yak::bitboard::FILE_B | yak::bitboard::FILE_G);
    Bitboard bishops = yak::bitboard::RANK_1 & (yak::bitboard::FILE_C | yak::bitboard::FILE_F);
    Bitboard queens = yak::bitboard::RANK_1 & yak::bitboard::FILE_D;
    Bitboard kings = yak::bitboard::RANK_1 & yak::bitboard::FILE_E;
    Bitboard friendly = yak::bitboard::RANK_1 | yak::bitboard::RANK_2;
    Bitboard occupied = friendly;
    Bitboard empty = ~occupied;
    int move_counter = 0;

    yak::attackmap::RookMap rook_atks;
    yak::attackmap::BishopMap bishop_atks;
    yak::attackmap::QueenMap queen_atks;
    Timer timer;

    for (int i = 0; i < 1000; i++) {
      move_counter = 0;
      yak::piece::generate_pawn_moves<PieceColour::WHITE>(&move_list[0], move_counter, pawns, empty, 0);
      yak::piece::generatePieceMoves<PieceType::KNIGHT>(&move_list[0], move_counter, knights, empty, 0);
      yak::piece::generatePieceMoves<PieceType::KING>(&move_list[0], move_counter, knights, empty, 0);
      yak::piece::generatePieceMoves<PieceType::BISHOP>(&move_list[0], move_counter, knights, empty, 0);
      yak::piece::generatePieceMoves<PieceType::ROOK>(&move_list[0], move_counter, knights, empty, 0);
      yak::piece::generatePieceMoves<PieceType::QUEEN>(&move_list[0], move_counter, knights, empty, 0);
    }

    std::cout << move_counter << std::endl;

    //for (int i = 0; i < 50; i++) {
    //	std::cout << move_list[i].from << ", " << move_list[i].to << ", " << move_list[i].capture << std::endl;
    //}
  }



}
