#ifndef YAK_BOARD_H_
#define YAK_BOARD_H_

#include <iostream>
#include <string>

#include "pieces.h"
#include "bitboard.h"

/*
 * \brief Class for encoding move information 
 */
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
	/*
	 * \brief Constuctor for castling moves
	 * \param[in] castle_side - The side on which to caste (KING/QUEEN)
	 */
	Move(PieceType castle_side) : castle(castle_side) {};

	/*
	 * \brief Constructor for moves that are not castling
	 * \param[in] from - the square to move from
	 * \param[in] to - the square to move to
	 * \param[in] type - the type of piece to be moved
	 */
	Move(Square from, Square to, PieceType type) : from(from), to(to), piece_to_move(type) {};

	/*
	 * \brief Check if this move is a capture.
	 * \return true if this is a capture, false otherwise
	 */
	bool is_capture() { return piece_to_capture != PieceType::NULL_PIECE; }

	/*
	 * \brief Check if this move is a promotion.
	 * \return true if this is a promotion, false otherwise.
	 */
	bool is_promotion() { return promotion_piece != PieceType::NULL_PIECE; }

	/*
	 * \brief Check if this move is en passant capture.
	 * \return true if this is en passant, false otherwise.
	 */
	bool is_en_passant() { return ep_target_piece != 0; }	

	/*
	 * \brief Check if this move is a double pawn push.
	 * \return true if this is a double pawn push, false otherwise.
	 */
	bool is_double_push() { return double_push; }

	/*
	 * \brief Check is this move is a castling move.
	 * \return true if this is a castling move, false otherwise.
	 */
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


/*
 * \brief Encodes and maintains castling rights for the game state.
 */
class CastlingRights {
private:
	bool black[2]{ 0 };
	bool white[2]{ 0 };
public:
	/* \brief Default constructor. */
	CastlingRights() {};

	/* 
	 * \brief Construct castling rights from fen.
	 * \param[in] fen - std::string containing the catling rights in fen format
	 */
	CastlingRights(std::string fen);

	/* 
	 * \brief Check if a side can castle in the king side.
	 * \param[in] colour - the colour of the side to check the rights for.
	 * \return true if colour can castle, false otherwise.
	 */
	bool king_side(PieceColour colour);

	/*
	 * \brief Check if a side can castle in the queen side.
	 * \param[in] colour - the colour of the side to check the rights for.
	 * \return true if colour can castle, false otherwise.
	 */
	bool queen_side(PieceColour colour);

	/*
	 * \brief Update castling rights based on a move.
	 * \param[in] move - a move that may update the rights.
	 * \param[in] side - the side that is making the move.
	 */
	void update(Move move, PieceColour side);

	/* 
	 * \brief Get the current castling rights in fen format.
	 * \return std::string containing the fen of the current castling rights.
     */
	std::string fen();
};


/*
 * \brief Holds all of the games state execpt for piece postions.
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
		/* \brief Default constructor */
		GameState(): side(PieceColour::WHITE) {}

		/* 
		 * \brief Construct from fen.
		 * \param[in] fen - the fen string of the game state to be initialised.
		 */
		GameState(std::string fen) { parse_fen(fen); }

		/*
		 * \brief Return the current game state in FEN notation.
		 * \return std::string in FEN notation.
		 */
		std::string to_fen();

		/*
		 * \brief Update the game state based on a given move.
		 * \param[in] move - The move to be made.
		 */
		void update(Move move);

		void update(faster::Move move);

		/*
		 * \brief Check if a side can castle on the king side.
		 * \param[in] colour - The colour of the side to check.
		 * \return true if the side can castle on the king side, false otherwise.
		 */
		bool can_king_side_castle(PieceColour colour);

		/*
		 * \brief Check if the current side to move can king side castle.
		 * \return true if the side to move can king side castle, false otherwise.
		 */
		bool can_king_side_castle();

		/*
		 * \brief Check if a side can castle on the queen side.
		 * \param[in] colour - The colour of the side to check.
		 * \return true if the side can castle on the queen side, false otherwise.
		 */
		bool can_queen_side_castle(PieceColour colour);

		/*
		 * \brief Check if the current side to move can queen side castle.
		 * \return true if the side to move can queen side castle, false otherwise.
		 */
		bool can_queen_side_castle();

		/* \brief Change the side to move. */
		void toggle_side_to_move();


		inline PieceColour side_to_move() const { return side; }
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

	std::vector<Move> generate_pawn_moves_and_captures();
};

namespace faster {
	class Board {
	private:
		// Piece positions
		Bitboard piece_type_bitboard[6];
		Bitboard colour_bitboard[2];

		// Game state
		GameState current_state;
		GameState previous_state;

		// Attack maps for move generation
		attacks::KingAttacks king_atks;
		attacks::KnightAttacks knight_atks;
		RookAttackMap rook_atks;
		BishopAttackMap bishop_atks;
		QueenAttackMap queen_atks;

		// Move storage
		Move previous_move;
		Move move_list[330];
		int move_counter{ 0 };

		PieceColour side_to_move = PieceColour::WHITE;

		void parse_fen(const std::string& fen);
		std::string rank_to_fen(Rank rank);
		void process_move(Move move);


		Bitboard piece_attacks(PieceType type, PieceColour colour);
		Bitboard all_attacks(PieceColour colour);
		bool target_is_promotion(Square square) { return (square < 8 || square > 55) ? 1 : 0; }

		std::vector<Move> generate_piece_moves_and_captures(PieceType type, std::vector<Move> moves);
		std::vector<Move> generate_castling_moves(std::vector<Move> moves);

	public:
		Board() : piece_type_bitboard{ 0 }, colour_bitboard{ 0 } {}
		Board(const std::string& fen) : piece_type_bitboard{ 0 }, colour_bitboard{ 0 } { parse_fen(fen); }

		PieceType get_piece_type_on(Square square) const 
		{
			Bitboard square_bb = bitboard::to_bitboard(square);
			for (int i = 0; i < 6; i++)
				if (square_bb & piece_type_bitboard[i]) return static_cast<PieceType>(i);

		};

		PieceColour get_piece_colour_on(Square square) const;

		Bitboard occupied_squares() const;
		Bitboard empty_squares() const;

		std::vector<Move> generate_moves() {
			Move temp_move_list[330];
			int temp_move_counter{ 0 };
			PieceColour side = current_state.side_to_move();
			generate_pawn_moves(side, &temp_move_list[0], temp_move_counter, get_position(side, PieceType::PAWN),
				empty_squares(), get_position(current_state.side_not_to_move()));
			jumping_piece_moves(&knight_atks, &temp_move_list[temp_move_counter], temp_move_counter, get_position(side, PieceType::KNIGHT),
				empty_squares(), get_position(current_state.side_not_to_move()));
			jumping_piece_moves(&king_atks, &temp_move_list[temp_move_counter], temp_move_counter, get_position(side, PieceType::KING),
				empty_squares(), get_position(current_state.side_not_to_move()));
			generate_sliding_piece_moves(&bishop_atks, &temp_move_list[temp_move_counter], temp_move_counter, get_position(side, PieceType::BISHOP),
				empty_squares(), get_position(current_state.side_not_to_move()));
			generate_sliding_piece_moves(&rook_atks, &temp_move_list[temp_move_counter], temp_move_counter, get_position(side, PieceType::ROOK),
				empty_squares(), get_position(current_state.side_not_to_move()));
			generate_sliding_piece_moves(&queen_atks, &temp_move_list[temp_move_counter], temp_move_counter, get_position(side, PieceType::QUEEN),
				empty_squares(), get_position(current_state.side_not_to_move()));

			std::vector<Move> legal_moves;
			for (int i = 0; i < temp_move_counter; i++)
			{
				make_move(temp_move_list[i]);
				if (!is_check(previous_state.side_to_move())) legal_moves.push_back(temp_move_list[i]);
				undo_move();
			}
			return legal_moves;
		}

		void make_move(PieceType type, PieceColour colour, Square from, Square to);
		void make_move(Move move) 
		{
			process_move(move);

			previous_state = current_state;
			current_state.update(move);
			current_state.toggle_side_to_move();
			previous_move = move;
		};

		/*
		 * Undo the last move that was made
		 */
		void undo_move();

		bool is_check() const;
		bool is_check(PieceColour colour) const;
		bool is_checkmate() const;

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

		std::vector<Move> generate_pawn_moves_and_captures();

	};

}


#endif // YAK_BOARD_H_
