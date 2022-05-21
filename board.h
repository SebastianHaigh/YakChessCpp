#ifndef YAK_BOARD_H_
#define YAK_BOARD_H_

#include <iostream>
#include <string>

#include "pieces.h"
#include "bitboard.h"

class Move {
private:
	Square from;
	Square to;
	PieceType piece_to_move;
	PieceType piece_to_capture = PieceType::NULL_PIECE;
	PieceType promotion_piece = PieceType::NULL_PIECE;
	Bitboard ep_target_piece = Bitboard(0);
	PieceType castle = PieceType::NULL_PIECE;
	bool double_push = false;

public:
	Move(PieceType castle_side) : castle(castle_side) {};
	Move(Square from, Square to, PieceType type) : from(from), to(to), piece_to_move(type) {};

	bool is_capture() { return piece_to_capture != PieceType::NULL_PIECE; }
	bool is_promotion() { return promotion_piece != PieceType::NULL_PIECE; }
	bool is_en_passant() { return ep_target_piece != 0; }	
	bool is_double_push() { return double_push; }
	bool is_castle() { return castle != PieceType::NULL_PIECE; }

	void set_double_push() { double_push = true; }
	void set_capture(PieceType type) { piece_to_capture = type; }
	void set_en_passant(Bitboard target_piece_location) { 
		ep_target_piece = target_piece_location; 
		piece_to_capture = PieceType::PAWN;
	}
	void set_promotion(PieceType type) { promotion_piece = type; }

	PieceType get_moved_piece() { return piece_to_move; }
	PieceType get_captured_piece() { return piece_to_capture; }
	PieceType get_castle_side() { return castle; }
	PieceType get_promotion_piece() { return promotion_piece; }

	Square to_square() { return to; };
	Square from_square() { return from; };

	Bitboard to_bitboard() { return bitboard::to_bitboard(to); }
	Bitboard from_bitboard() { return bitboard::to_bitboard(from); }
	Bitboard ep_capture_square_bb() { return ep_target_piece; }
	std::string to_algebraic();
};

class MoveFactory {
public:
	MoveFactory() {}
	/**
	* Creates double and single pawn pushes. 
	*
	* If the target square is on the 1st or 8th rank this will automatically be 
	* turned into a promotion, with the default promotion piece being queen.
	*/
	Move create_pawn_push(Square from, Square to);

	/**
	Explicit creation of a pawn promotion witht eh specified promotion piece. 
	*/
	Move create_pawn_push(Square from, Square to, PieceType promotion_type);

	/**
	* Creates a pawn capture of a piece.
	* 
	* If the target square is on the 1st or 8th rank this will automatically be 
	* turned into a promotion, with the default promotion piece being queen.
	*/
	Move create_pawn_capture(Square from, Square to, PieceType captured_piece);

	/**
	* Creates an en passant pawn capture, where the pawn to be captured is on 
	* the square ep_target.
	*/
	Move create_pawn_capture(Square from, Square to, Square ep_target);

	/*
	Creates a pawn capture promotion.

	If the target square is on the 1st or 8th rank this will automatically be
	turned into a promotion, with the default promotion piece being queen.
	*/
	Move create_pawn_capture(Square from, Square to, PieceType captured_piece, PieceType promotion_type);


	Move create_quiet_move(Square from, Square to);
	Move create_capture(Square form, Square to);
};

class CastlingRights {
private:
	bool black[2]{ 0 };
	bool white[2]{ 0 };
public:
	CastlingRights() {};
	CastlingRights(std::string fen);
	bool king_side(PieceColour colour);
	bool queen_side(PieceColour colour);
	void update(Move move, PieceColour side);
	std::string fen();
};


/**
 * Holds all of the games state execpt for piece postions 
 */
class GameState {
	private:
		CastlingRights castling_rights;
		PieceColour side;
		int move_clock = 1;
		bool has_ep_target = false;
		Square ep_square = 0;
		void parse_fen(std::string fen);
		
	public:
		GameState(): side(PieceColour::WHITE) {}
		GameState(std::string fen) { parse_fen(fen); }
		std::string to_fen();
		void update(Move move);
		bool can_king_side_castle(PieceColour colour);
		bool can_king_side_castle();

		bool can_queen_side_castle(PieceColour colour);
		bool can_queen_side_castle();

		void toggle_side_to_move();
		PieceColour side_to_move() { return side; }
		PieceColour side_not_to_move() { return pieces::other_colour(side); }
		Square ep_target_square() { return ep_square; }
		Bitboard ep_target();

};


class Board {

private:
	Bitboard piece_type_bitboard[6];
	Bitboard colour_bitboard[2];
	GameState current_state;
	GameState previous_state;

	pieces::StandardPieceFactory factory; // Generates the rules for piece movement
	void parse_fen(const std::string& fen);
	std::string rank_to_fen(Rank rank);
	Move previous_move;

	void process_move(Move move);

	std::vector<Move> move_list;
	Bitboard pawn_attacks(PieceColour colour);
	Bitboard piece_attacks(PieceType type, PieceColour colour);
	Bitboard all_attacks(PieceColour colour);

	bool target_is_promotion(Square square) { return (square < 8 || square > 55) ? 1 : 0; }

	std::vector<Move> generate_pawn_moves_and_captures();
	std::vector<Move> generate_piece_moves_and_captures(PieceType type, std::vector<Move> moves);
	std::vector<Move> generate_castling_moves(std::vector<Move> moves);


public: 
	Board() : piece_type_bitboard{ 0 }, colour_bitboard{ 0 }, previous_move(Move(0, 0, PieceType::NULL_PIECE)) {}
	Board(const std::string& fen) : piece_type_bitboard{ 0 }, colour_bitboard{ 0 }, previous_move(Move(0, 0, PieceType::NULL_PIECE)) { parse_fen(fen); }

	PieceType get_piece_type_on(Square square);
	PieceColour get_piece_colour_on(Square square);

	Bitboard occupied_squares();
	Bitboard empty_squares();

	std::vector<Move> generate_moves();
	void make_move(PieceType type, PieceColour colour, Square from, Square to);
	void make_move(Move move);
	void undo_move();

	bool is_check();
	bool is_check(PieceColour colour);
	bool is_checkmate();

	std::string to_fen();

	Bitboard ep_target() { return current_state.ep_target(); }
	Square ep_target_square() { return current_state.ep_target_square(); }

	PieceColour to_move() { return current_state.side_to_move(); }

	Bitboard attacked_by(PieceColour colour) { return all_attacks(colour); }

	bool can_king_side_castle(PieceColour colour);
	bool can_queen_side_castle(PieceColour colour);

	Bitboard get_position(PieceType type) { return piece_type_bitboard[static_cast<int>(type)]; }
	Bitboard get_position(PieceColour colour) { return colour_bitboard[static_cast<int>(colour)]; }
	Bitboard get_position(PieceColour colour, PieceType type) { return get_position(type) & get_position(colour); }
};




#endif // YAK_BOARD_H_
