#include "benchmarking.h"
#include "benchmark_tests.h"
#include "../src/pieces.h"
#include "../src/board.h"
#include "../src/attackmaps.h"

#include <vector>
#include <iostream>
#include <random>

namespace yak {

void PAWN_MOVE_GENERATION() {

  Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
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
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
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
    Move move_list[330];
    Bitboard pawns = bitboard::RANK_2;
    Bitboard rooks = bitboard::RANK_1 & (bitboard::FILE_A | bitboard::FILE_H);
    Bitboard knights = bitboard::RANK_1 & (bitboard::FILE_B | bitboard::FILE_G);
    Bitboard bishops = bitboard::RANK_1 & (bitboard::FILE_C | bitboard::FILE_F);
    Bitboard queens = bitboard::RANK_1 & bitboard::FILE_D;
    Bitboard kings = bitboard::RANK_1 & bitboard::FILE_E;
    Bitboard friendly = bitboard::RANK_1 | bitboard::RANK_2;
    Bitboard occupied = friendly;
    Bitboard empty = ~occupied;
    int moveCounter = 0;

    attackmap::RookMap rook_atks;
    attackmap::BishopMap bishop_atks;
    attackmap::QueenMap queen_atks;
    Timer timer;

    for (int i = 0; i < 1000; i++) {
      moveCounter = 0;
      /* pawns::generatePawnMoves<PieceColour::WHITE>(&move_list[0], moveCounter, pawns, empty, 0); */
      /* moveCounter += piece::generatePieceMoves<PieceType::KNIGHT>(&move_list[0], knights, empty, 0); */
      /* moveCounter += piece::generatePieceMoves<PieceType::KING>(&move_list[0], kings, empty, 0); */
      /* moveCounter += piece::generatePieceMoves<PieceType::BISHOP>(&move_list[0], bishops, empty, 0); */
      /* moveCounter += piece::generatePieceMoves<PieceType::ROOK>(&move_list[0], rooks, empty, 0); */
      /* moveCounter += piece::generatePieceMoves<PieceType::QUEEN>(&move_list[0], queens, empty, 0); */
    }
    std::cout << moveCounter << std::endl;
  }
}

} // namespace yak
