#include "benchmarking.h"
#include "benchmark_tests.h"
#include "../pieces.h"
#include "../board.h"
#include "../attackmaps.h"
#include "../magic.h"

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


	{
		std::cout << "---------------------------" << std::endl;
		std::cout << "PAWN_MOVE_GENERATION faster 2" << std::endl;
		std::cout << "---------------------------" << std::endl;
		faster::Move move_list[330];
		std::random_device rand_dev;
		std::mt19937 generator(rand_dev());
		std::uniform_int_distribution<int>  distr(0, 1);
		std::vector<PieceColour> a;
		int move_counter = 0;
		for (int i = 0; i < 1000; i++) {
			a.push_back((PieceColour)distr(generator));
		}
		Timer timer;
		
		for (int i = 0; i < 1000; i++) {
			faster::generate_pawn_moves(a[i], &move_list[0], move_counter, bitboard::RANK_2, bitboard::NOT_RANK_2, bitboard::RANK_3);
		}
		/*
		for (int i = 0; i < 30; i++) {
			std::cout << move_list[i].from << ", " << move_list[i].to << ", " << move_list[i].capture << std::endl;
		}
		*/
		
	}



}

void KNIGHT_MOVE_GENERATION() {
	pieces::StandardPieceFactory factory;

	{
		pieces::StandardPieceFactory factory;
		std::unique_ptr<pieces::Piece> knights = factory.make_piece(PieceType::KNIGHT);

		std::cout << "---------------------------" << std::endl;
		std::cout << "KNIGHT_MOVE_GENERATION" << std::endl;
		std::cout << "---------------------------" << std::endl;
		std::vector<std::pair<Square, Square>> moves;
		Timer timer;

		for (int i = 0; i < 1000; i++) {
			
			knights->quiet_moves(bitboard::RANK_2, bitboard::NOT_RANK_2);
			moves = knights->captures(bitboard::RANK_1, Bitboard(0), bitboard::RANK_3 | bitboard::RANK_4);
		}

		//for (auto move : moves) {
		//	std::cout << move.first << ", " << move.second << std::endl;
		//}

	}

	attacks::KnightAttacks atks;
	{
		std::cout << "---------------------------" << std::endl;
		std::cout << "KNIGHT_MOVE_GENERATION fast" << std::endl;
		std::cout << "---------------------------" << std::endl;
		faster::Move move_list[330];
		Bitboard enemy = bitboard::RANK_3 | bitboard::RANK_4;
		Bitboard friendly = bitboard::RANK_1;
		Bitboard occupied = enemy | friendly;
		Bitboard empty = ~occupied;
		int move_counter = 0;
		Timer timer;
		
		for (int i = 0; i < 1000; i++) {
			jumping_piece_moves(&atks, &move_list[0], move_counter, friendly, empty, enemy);
		}

		//for (int i = 0; i < 50; i++) {
		//	std::cout << move_list[i].from << ", " << move_list[i].to << ", " << move_list[i].capture << std::endl;
		//}
	}
}

void ROOK_MOVE_GENERATION() {
	pieces::StandardPieceFactory factory;

	{
		pieces::StandardPieceFactory factory;
		std::unique_ptr<pieces::Piece> rook = factory.make_piece(PieceType::ROOK);

		std::cout << "---------------------------" << std::endl;
		std::cout << "ROOK_MOVE_GENERATION" << std::endl;
		std::cout << "---------------------------" << std::endl;
		std::vector<std::pair<Square, Square>> moves;
		Timer timer;

		for (int i = 0; i < 1000; i++) {

			rook->quiet_moves(bitboard::RANK_2, bitboard::NOT_RANK_2);
			moves = rook->captures(bitboard::RANK_1, Bitboard(0), bitboard::RANK_3 | bitboard::RANK_4);
		}

		//for (auto move : moves) {
		//	std::cout << move.first << ", " << move.second << std::endl;
		//}

	}

	
	{
		std::cout << "---------------------------" << std::endl;
		std::cout << "ROOK_MOVE_GENERATION fast" << std::endl;
		std::cout << "---------------------------" << std::endl;
		faster::Move move_list[330];
		Bitboard enemy = bitboard::RANK_8;
		Bitboard friendly = bitboard::RANK_1;
		Bitboard occupied = enemy | friendly;
		Bitboard empty = ~occupied;
		int move_counter = 0;
		faster::SpecialisedRay<Direction::NORTH> n_ray;
		faster::SpecialisedRay<Direction::EAST> e_ray;
		faster::SpecialisedRay<Direction::SOUTH> s_ray;
		faster::SpecialisedRay<Direction::WEST> w_ray;
		std::vector<faster::Ray*> rook_atks = { &n_ray, &e_ray, &s_ray, &w_ray };
		Timer timer;

		for (int i = 0; i < 1000; i++) {
			generate_sliding_piece_moves(rook_atks, &move_list[0], move_counter, friendly, empty, enemy);
		}

		//for (int i = 0; i < 50; i++) {
		//	std::cout << move_list[i].from << ", " << move_list[i].to << ", " << move_list[i].capture << std::endl;
		//}
	}

	{
		std::cout << "---------------------------" << std::endl;
		std::cout << "ROOK_MOVE_GENERATION fast factory" << std::endl;
		std::cout << "---------------------------" << std::endl;
		faster::Move move_list[330];
		Bitboard enemy = bitboard::RANK_8;
		Bitboard friendly = bitboard::RANK_1;
		Bitboard occupied = enemy | friendly;
		Bitboard empty = ~occupied;
		int move_counter = 0;
		faster::RookAttackMap rook_atks;
		Timer timer;

		for (int i = 0; i < 1000; i++) {
			generate_sliding_piece_moves(&rook_atks, &move_list[0], move_counter, friendly, empty, enemy);
		}

		//for (int i = 0; i < 50; i++) {
		//	std::cout << move_list[i].from << ", " << move_list[i].to << ", " << move_list[i].capture << std::endl;
		//}
	}

	{
		std::cout << "---------------------------" << std::endl;
		std::cout << "GENERATE ALL MOVES" << std::endl;
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
		attacks::KnightAttacks knight_atks;
		attacks::KingAttacks king_atks;
		int move_counter = 0;
		faster::SpecialisedRay<Direction::NORTH> n_ray;
		faster::SpecialisedRay<Direction::EAST> e_ray;
		faster::SpecialisedRay<Direction::SOUTH> s_ray;
		faster::SpecialisedRay<Direction::WEST> w_ray;
		faster::SpecialisedRay<Direction::NORTH_EAST> ne_ray;
		faster::SpecialisedRay<Direction::SOUTH_EAST> se_ray;
		faster::SpecialisedRay<Direction::NORTH_WEST> nw_ray;
		faster::SpecialisedRay<Direction::SOUTH_WEST> sw_ray;

		std::vector<faster::Ray*> rook_atks = { &n_ray, &e_ray, &s_ray, &w_ray };
		std::vector<faster::Ray*> bishop_atks = { &ne_ray, &se_ray, &nw_ray, &sw_ray };
		std::vector<faster::Ray*> queen_atks = { &n_ray, &e_ray, &s_ray, &w_ray, &se_ray, &ne_ray, &nw_ray, &sw_ray };
		Timer timer;

		for (int i = 0; i < 1000; i++) {
			move_counter = 0;
			faster::generate_pawn_moves(PieceColour::WHITE, &move_list[0], move_counter, pawns, empty, 0);
			faster::jumping_piece_moves(&knight_atks, &move_list[0], move_counter, knights, empty, 0);
			faster::jumping_piece_moves(&king_atks, &move_list[0], move_counter, kings, empty, 0);
			faster::generate_sliding_piece_moves(rook_atks, &move_list[0], move_counter, rooks, empty, 0);
			faster::generate_sliding_piece_moves(bishop_atks, &move_list[0], move_counter, bishops, empty, 0);
			faster::generate_sliding_piece_moves(queen_atks, &move_list[0], move_counter, queens, empty, 0);
		}

		std::cout << move_counter << std::endl;

		//for (int i = 0; i < 50; i++) {
		//	std::cout << move_list[i].from << ", " << move_list[i].to << ", " << move_list[i].capture << std::endl;
		//}
	}

	{
		std::cout << "---------------------------" << std::endl;
		std::cout << "GENERATE ALL MOVES factory" << std::endl;
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
		attacks::KnightAttacks knight_atks;
		attacks::KingAttacks king_atks;
		int move_counter = 0;

		faster::RookAttackMap rook_atks;
		faster::BishopAttackMap bishop_atks;
		faster::QueenAttackMap queen_atks;
		Timer timer;

		for (int i = 0; i < 1000; i++) {
			move_counter = 0;
			faster::generate_pawn_moves(PieceColour::WHITE, &move_list[0], move_counter, pawns, empty, 0);
			faster::jumping_piece_moves(&knight_atks, &move_list[0], move_counter, knights, empty, 0);
			faster::jumping_piece_moves(&king_atks, &move_list[0], move_counter, kings, empty, 0);
			faster::generate_sliding_piece_moves(&rook_atks, &move_list[0], move_counter, rooks, empty, 0);
			faster::generate_sliding_piece_moves(&bishop_atks, &move_list[0], move_counter, bishops, empty, 0);
			faster::generate_sliding_piece_moves(&queen_atks, &move_list[0], move_counter, queens, empty, 0);
		}

		std::cout << move_counter << std::endl;

		//for (int i = 0; i < 50; i++) {
		//	std::cout << move_list[i].from << ", " << move_list[i].to << ", " << move_list[i].capture << std::endl;
		//}
	}

	
}
