#ifndef YAK_BOARD_H_
#define YAK_BOARD_H_

#include <iostream>
#include <string>

#include "pieces.h"
#include "bitboard.h"

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
	void update(const faster::Move& move, PieceColour side);

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
		//void update(Move move);

		void update(const faster::Move& move);

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

		template<PieceType T>
		bool can_castle() 
		{ 
			return (T == PieceType::KING) ? can_king_side_castle() :
				(T == PieceType::QUEEN) ? can_queen_side_castle() : false;
		}

		/* \brief Change the side to move. */
		void toggle_side_to_move();


		inline PieceColour side_to_move() const { return side; }
		PieceColour side_not_to_move() { return pieces::other_colour(side); }
		Square ep_target_square() { return ep_square; }
		Bitboard ep_target();

};

template<PieceType T, PieceColour C> struct KingCastleTarget { static constexpr Bitboard value{ 0 }; };
template<> struct KingCastleTarget<PieceType::KING, PieceColour::WHITE> { static constexpr Bitboard value = bitboard::static_bitboard<6>::value; };
template<> struct KingCastleTarget<PieceType::KING, PieceColour::BLACK> { static constexpr Bitboard value = bitboard::static_bitboard<62>::value; };
template<> struct KingCastleTarget<PieceType::QUEEN, PieceColour::WHITE> { static constexpr Bitboard value = bitboard::static_bitboard<2>::value; };
template<> struct KingCastleTarget<PieceType::QUEEN, PieceColour::BLACK> { static constexpr Bitboard value = bitboard::static_bitboard<58>::value; };

template<PieceType T, PieceColour C> struct RookCastleSource { static constexpr Bitboard value{ 0 }; };
template<> struct RookCastleSource<PieceType::KING, PieceColour::WHITE> { static constexpr Bitboard value = bitboard::static_bitboard<7>::value; };
template<> struct RookCastleSource<PieceType::KING, PieceColour::BLACK> { static constexpr Bitboard value = bitboard::static_bitboard<63>::value; };
template<> struct RookCastleSource<PieceType::QUEEN, PieceColour::WHITE> { static constexpr Bitboard value = bitboard::static_bitboard<0>::value; };
template<> struct RookCastleSource<PieceType::QUEEN, PieceColour::BLACK> { static constexpr Bitboard value = bitboard::static_bitboard<56>::value; };

template<PieceType T, PieceColour C> struct RookCastleTarget { static constexpr Bitboard value{ 0 }; };
template<> struct RookCastleTarget<PieceType::KING, PieceColour::WHITE> { static constexpr Bitboard value = bitboard::static_bitboard<5>::value; };
template<> struct RookCastleTarget<PieceType::KING, PieceColour::BLACK> { static constexpr Bitboard value = bitboard::static_bitboard<61>::value; };
template<> struct RookCastleTarget<PieceType::QUEEN, PieceColour::WHITE> { static constexpr Bitboard value = bitboard::static_bitboard<3>::value; };
template<> struct RookCastleTarget<PieceType::QUEEN, PieceColour::BLACK> { static constexpr Bitboard value = bitboard::static_bitboard<59>::value; };

template<PieceColour C> struct OppositeColour { static constexpr PieceColour value = PieceColour::NULL_COLOUR; };
template<> struct OppositeColour<PieceColour::WHITE> { static constexpr PieceColour value = PieceColour::BLACK; };
template<> struct OppositeColour<PieceColour::BLACK> { static constexpr PieceColour value = PieceColour::WHITE; };



class Board {

private:

	Bitboard piece_type_bitboard[6];
	Bitboard colour_bitboard[2];

	/* \brief The current GameState */
	GameState current_state;

	/* \brief A record of the GameState before the last move, for undoing moves. */
	GameState previous_state;

	attacks::KnightAttacks knight_atks; /* \brief Attack map for Knights */
	attacks::KingAttacks king_atks;		/* \brief Attack map for Kings */
	faster::RookMap rook_atks;			/* \brief Attack map for Rooks */
	faster::BishopMap bishop_atks;		/* \brief Attack map for Bishops */
	faster::QueenMap queen_atks;		/* \brief Attack map for Queens */
	
	/* \brief The last move to have been made on this board. */
	faster::Move previous_move_f; 

	/* \brief The piece captured on the last move, NULL_PIECE if last move was not a capture. */
	PieceType previous_captured_piece = PieceType::NULL_PIECE; 

	/*
	 * \brief Executes a move on the underlying board representation.
	 * \tparam C - Colour of the side on which to execute the move.
	 * \param[in] move - The move to be executed.
	 * \param[in] undo - If true this will process an undo move.
	 */
	template<PieceColour C>
	void process_move(const faster::Move& move, bool undo);

	/*
	 * \brief Executes a castling move on the underlying board representation.
	 * \tparam C - Colour of the side on which to execute the move.
	 * \tparam T - The side of the board on which to castle.
	 * \param[in] move - The move to be executed.
	 */
	template<PieceType T, PieceColour C>
	void process_castle(const faster::Move& move);

	template<PieceColour C>
	void process_ep(const faster::Move& move);

	/*
	 * \brief Returns all of the squares attacked by pawns of a given colour.
	 * \param[in] colour - The coloyr of the pawns.
	 * \return The bitboard of all the attacked squares.
	 */
	Bitboard pawn_attacks(PieceColour colour);

	/*
	 * \brief Returns all of the squares attacked by a particular type of piece.
	 * \param[in] type - The type of piece.
	 * \param[in] colour - The colour of the piece.
	 * \return A bitboard of all the attacked squares.
	 */
	Bitboard piece_attacks(PieceType type, PieceColour colour);
	
	/*
	 * \brief Returns all of the squares attacked by a single side.
	 * \param[in] colour - The colour of the side.
	 * \return A bitboard of all the attacked squares.
	 */
	Bitboard all_attacks(PieceColour colour);


	std::vector<faster::Move> generate_castling_moves(std::vector<faster::Move> moves);

	void parse_fen(const std::string& fen);
	std::string rank_to_fen(Rank rank);


public: 
	Board() : piece_type_bitboard{ 0 }, colour_bitboard{ 0 }, previous_move_f(faster::Move()) {}
	Board(const std::string& fen) : piece_type_bitboard{ 0 }, colour_bitboard{ 0 }, previous_move_f(faster::Move()) { parse_fen(fen); }

	PieceType get_piece_type_on(Square square);
	PieceColour get_piece_colour_on(Square square);

	Bitboard occupied_squares();
	Bitboard empty_squares();

	std::vector<faster::Move> generate_moves();
	void make_move(PieceType type, PieceColour colour, Square from, Square to);
	//void make_move(Move move);
	void make_move(const faster::Move& move);
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


template<PieceColour C>
void Board::process_move(const faster::Move& move, bool undo) {

	// If the move to be processed is a castle then we can handle this here and then exit.
	if (move.castle == PieceType::KING)
		return process_castle<PieceType::KING, C>(move);
	else if (move.castle == PieceType::QUEEN)
		return process_castle<PieceType::QUEEN, C>(move);

	if (move.en_passant) process_ep<C>(move);

	// Bitboards that will be used to update the board representation
	Bitboard to_bitboard = bitboard::to_bitboard(move.to);
	Bitboard from_bitboard = bitboard::to_bitboard(move.from);
	Bitboard from_to_bitboard = to_bitboard ^ from_bitboard;

	// Determine the piece type that is to be moved.
	int piece_to_move = 0;
	Bitboard piece_check_bb = undo ? to_bitboard : from_bitboard;
	while (!(piece_check_bb & piece_type_bitboard[piece_to_move]))
		piece_to_move++;

	int piece_to_capture = 0;

	if (move.en_passant)
	{
		piece_to_capture = static_cast<int>(PieceType::PAWN);
	}
	else
	{
		if (move.capture && !undo) {
			// If this is a make rather than unmake we need to determine the type of the piece to capture.
			while (!(to_bitboard & piece_type_bitboard[piece_to_capture]))
				piece_to_capture++;
			previous_captured_piece = PieceType(piece_to_capture);
		}
		else if (move.capture && undo) {
			piece_to_capture = static_cast<int>(previous_captured_piece);
		}
	}

	int colour_to_move = static_cast<int>(C);
	int opposing_colour = static_cast<int>(OppositeColour<C>::value);

	

	// Make the basic move.
	piece_type_bitboard[piece_to_move] ^= from_to_bitboard;
	colour_bitboard[colour_to_move] ^= from_to_bitboard;
	
	// If the move is a capture, remove the captured piece.
	if (move.capture)
	{
		Bitboard capture_square = to_bitboard;
		if (move.en_passant) capture_square = current_state.ep_target();

		piece_type_bitboard[piece_to_capture] ^= capture_square;
		colour_bitboard[opposing_colour] ^= capture_square;	
	}

	// If the move is a promotion, swap the moved piece for a piece of the promoted type.
	if (move.promotion != PieceType::NULL_PIECE) {
		piece_type_bitboard[static_cast<int>(PieceType::PAWN)] ^= to_bitboard;
		colour_bitboard[colour_to_move] ^= to_bitboard;
		piece_type_bitboard[static_cast<int>(move.promotion)] ^= to_bitboard;
		colour_bitboard[colour_to_move] ^= to_bitboard;
	}

	if (undo) {
		previous_captured_piece = PieceType::NULL_PIECE;
	}
	
}

template<PieceColour C>
void Board::process_ep(const faster::Move& move) {
	Bitboard to_bitboard = bitboard::to_bitboard(move.to);
	Bitboard from_bitboard = bitboard::to_bitboard(move.from);
	Bitboard from_to_bitboard = to_bitboard ^ from_bitboard;

	piece_type_bitboard[static_cast<int>(PieceType::PAWN)] ^= from_to_bitboard;
	colour_bitboard[static_cast<int>(C)] ^= from_to_bitboard;

	// get the ep target square from move.to square.
	// This is better than using the Boards ep_square because 
	// it will give the same result when the move is undo'd.
	Bitboard capture_square = faster::pawn_single_push_source<C>(to_bitboard);
	piece_type_bitboard[static_cast<int>(PieceType::PAWN)] ^= capture_square;
	colour_bitboard[static_cast<int>(OppositeColour<C>::value)] ^= capture_square;

}

template<PieceType T, PieceColour C>
void Board::process_castle(const faster::Move& move) {

	if (current_state.can_castle<T>())
	{
		// Move the king
		Bitboard from_to_bitboard = get_position(C, PieceType::KING) ^ KingCastleTarget<T, C>::value;
		piece_type_bitboard[static_cast<int>(PieceType::KING)] ^= from_to_bitboard;
		colour_bitboard[static_cast<int>(C)] ^= from_to_bitboard;

		// Move the rook
		from_to_bitboard = RookCastleSource<T, C>::value ^ RookCastleTarget<T, C>::value;
		piece_type_bitboard[static_cast<int>(PieceType::ROOK)] ^= from_to_bitboard;
		colour_bitboard[static_cast<int>(C)] ^= from_to_bitboard;
	}
}


namespace faster {

	/*
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
	*/
}


#endif // YAK_BOARD_H_
