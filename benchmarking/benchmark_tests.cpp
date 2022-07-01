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

	Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	{
		std::cout << "---------------------------" << std::endl;
		std::cout << "PAWN_MOVE_GENERATION" << std::endl;
		std::cout << "---------------------------" << std::endl;
		Timer timer;
		for (int i = 0; i < 1000; i++) {
			board.generate_moves();
		}
		
	}

	
	{
		Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
		std::cout << "---------------------------" << std::endl;
		std::cout << "PAWN_MOVE_GENERATION" << std::endl;
		std::cout << "---------------------------" << std::endl;
		Timer timer;
		for (int i = 0; i < 1000; i++) {
			board.generate_moves();
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
		faster::Move move_list[330];
		Bitboard pawns = bitboard::RANK_2;
		Bitboard rooks = bitboard::RANK_1 & (bitboard::FILE_A | bitboard::FILE_H);
		Bitboard knights = bitboard::RANK_1 & (bitboard::FILE_B | bitboard::FILE_G);
		Bitboard bishops = bitboard::RANK_1 & (bitboard::FILE_C | bitboard::FILE_F);
		Bitboard queens = bitboard::RANK_1 & bitboard::FILE_D;
		Bitboard kings = bitboard::RANK_1 & bitboard::FILE_E;
		Bitboard friendly = bitboard::RANK_1 | bitboard::RANK_2;
		Bitboard occupied = friendly;
		Bitboard empty = ~occupied;
		int move_counter = 0;

		faster::RookMap rook_atks;
		faster::BishopMap bishop_atks;
		faster::QueenMap queen_atks;
		Timer timer;

		for (int i = 0; i < 1000; i++) {
			move_counter = 0;
			faster::generate_pawn_moves<PieceColour::WHITE>(&move_list[0], move_counter, pawns, empty, 0);
			faster::generate_piece_moves<PieceType::KNIGHT>(&move_list[0], move_counter, knights, empty, 0);
			faster::generate_piece_moves<PieceType::KING>(&move_list[0], move_counter, knights, empty, 0);
			faster::generate_piece_moves<PieceType::BISHOP>(&move_list[0], move_counter, knights, empty, 0);
			faster::generate_piece_moves<PieceType::ROOK>(&move_list[0], move_counter, knights, empty, 0);
			faster::generate_piece_moves<PieceType::QUEEN>(&move_list[0], move_counter, knights, empty, 0);
		}

		std::cout << move_counter << std::endl;

		//for (int i = 0; i < 50; i++) {
		//	std::cout << move_list[i].from << ", " << move_list[i].to << ", " << move_list[i].capture << std::endl;
		//}
	}

	
	
}
