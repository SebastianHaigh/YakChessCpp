#include "board.h"

/*
std::string Move::to_algebraic() {
	if(piece_to_move == PieceType::PAWN) {
		std::string output = "";
		if (is_capture()) {
			output += bitboard::to_algebraic(from)[0] + "x" + bitboard::to_algebraic(to);
		}
		else {
			output += bitboard::to_algebraic(to);
		}
		if (is_promotion()) {
			output += "=" + pieces::white_piece_type_to_fen_char(promotion_piece);
		}
		return output;
	}
	else if (piece_to_move != PieceType::PAWN && piece_to_capture == PieceType::NULL_PIECE) {
		return pieces::white_piece_type_to_fen_char(piece_to_move) + bitboard::to_algebraic(to);
	}
	else if (piece_to_move != PieceType::PAWN && piece_to_capture != PieceType::NULL_PIECE) {
		std::string algebraic_move{ pieces::white_piece_type_to_fen_char(piece_to_move) };
		return algebraic_move + "x" + bitboard::to_algebraic(to);
	}
	else if (castle == PieceType::KING) {
		return "OO";
	}
	else if (castle == PieceType::QUEEN) {
		return "OOO";
	}
	else {
		return "XX";
	}
}
*/

void Board::parse_fen(const std::string& fen) {
	auto end_of_piece_placement = fen.find_first_of(" ");
	
	Square current_square{ 56 };

	for (int i = 0; i < end_of_piece_placement; i++) {
		if (fen[i] == '/') {
			// Advance to next rank
			current_square -= 16;
		}
		else if (isdigit(fen[i])) {
			// Advance squares (digits are empty squares)
			current_square += (fen[i] - '0');
		}
		else {
			PieceType piece_type = pieces::fen_char_to_piece_type(fen[i]);
			PieceColour piece_colour = pieces::fen_char_to_piece_colour(fen[i]);
			
			bitboard::set_square(piece_type_bitboard[static_cast<int>(piece_type)], current_square);
			bitboard::set_square(colour_bitboard[static_cast<int>(piece_colour)], current_square);
			current_square++;
		}
	}
	current_state = GameState(fen.substr(end_of_piece_placement + 1));
}

PieceType Board::get_piece_type_on(Square square) {
	Bitboard square_bb = bitboard::to_bitboard(square);
	for (int i = 0; i < 6; i++) {
		if ((square_bb & piece_type_bitboard[i]) > 0) {
			return static_cast<PieceType>(i);
		}
	}
	return PieceType::NULL_PIECE;
}

PieceColour Board::get_piece_colour_on(Square square) {
	Bitboard square_bb = bitboard::to_bitboard(square);
	for (int i = 0; i < 2; i++) {
		if ((square_bb & colour_bitboard[i]) > 0) {
			return static_cast<PieceColour>(i);
		}
	}
	return PieceColour::NULL_COLOUR;
}

Bitboard Board::occupied_squares() {
	return colour_bitboard[0] | colour_bitboard[1];
}

Bitboard Board::empty_squares() {
	return ~occupied_squares();
}

std::vector<faster::Move> Board::generate_moves() {
	faster::Move psuedo_legal_move_list[330];
	faster::Move en_passant_move;
	int move_counter{ 0 };

	PieceColour this_side = current_state.side_to_move();
	PieceColour other_side = current_state.side_not_to_move();

	if (this_side == PieceColour::WHITE) {
		faster::generate_pawn_moves<PieceColour::WHITE>(&psuedo_legal_move_list[move_counter], move_counter,
			get_position(this_side, PieceType::PAWN), empty_squares(), get_position(other_side));

		faster::generate_ep_captures<PieceColour::WHITE>(&en_passant_move, move_counter,
			get_position(this_side, PieceType::PAWN), ep_target());
	}
	else {
		faster::generate_pawn_moves<PieceColour::BLACK>(&psuedo_legal_move_list[move_counter], move_counter,
			get_position(this_side, PieceType::PAWN), empty_squares(), get_position(other_side));

		faster::generate_ep_captures<PieceColour::BLACK>(&psuedo_legal_move_list[move_counter], move_counter,
			get_position(this_side, PieceType::PAWN), ep_target());
	}

	faster::generate_piece_moves<PieceType::KNIGHT>(&psuedo_legal_move_list[move_counter], move_counter,
		get_position(this_side, PieceType::KNIGHT), empty_squares(), get_position(other_side));

	faster::generate_piece_moves<PieceType::KING>(&psuedo_legal_move_list[move_counter], move_counter,
		get_position(this_side, PieceType::KING), empty_squares(), get_position(other_side));

	faster::generate_piece_moves<PieceType::BISHOP>(&psuedo_legal_move_list[move_counter], move_counter,
		get_position(this_side, PieceType::BISHOP), empty_squares(), get_position(other_side));

	faster::generate_piece_moves<PieceType::ROOK>(&psuedo_legal_move_list[move_counter], move_counter,
		get_position(this_side, PieceType::ROOK), empty_squares(), get_position(other_side));

	faster::generate_piece_moves<PieceType::QUEEN>(&psuedo_legal_move_list[move_counter], move_counter,
		get_position(this_side, PieceType::QUEEN), empty_squares(), get_position(other_side));
	
	std::vector<faster::Move> legal_moves;
	for (int i = 0; i < move_counter; i++) {
		
		make_move(psuedo_legal_move_list[i]);
		if (!is_check(previous_state.side_to_move())) {
			legal_moves.push_back(psuedo_legal_move_list[i]);
		}
		undo_move();
	}

	legal_moves = generate_castling_moves(legal_moves);

	return legal_moves;
}

std::vector<faster::Move> Board::generate_castling_moves(std::vector<faster::Move> moves) {
	
	Bitboard squares_attacked_by_enemy = attacked_by(current_state.side_not_to_move());

	Bitboard king = get_position(current_state.side_to_move(), PieceType::KING);
	
	if (current_state.can_king_side_castle() && !is_check()) {
		Bitboard king_path = bitboard::east_one(king) | bitboard::east_one(bitboard::east_one(king));
		if ((king_path & occupied_squares()) == 0 && (king_path & squares_attacked_by_enemy) == 0) {
			moves.push_back(faster::make_kingside_castle());
			
		}
	} 
	
	if (current_state.can_queen_side_castle() && !is_check()) {
		Bitboard king_path = bitboard::west_one(king) | bitboard::west_one(bitboard::west_one(king));
		Bitboard rook_path = king_path | bitboard::west_one(king_path);
		if ((rook_path & occupied_squares()) == 0 && (king_path & squares_attacked_by_enemy) == 0) {
			moves.push_back(faster::make_queenside_castle());
		}

	}
	return moves;
}

void Board::make_move(PieceType type, PieceColour colour, Square from, Square to) {
	Bitboard from_to_bitboard = bitboard::to_bitboard(from) ^ bitboard::to_bitboard(to);
	piece_type_bitboard[static_cast<int>(type)] ^= from_to_bitboard;
	colour_bitboard[static_cast<int>(colour)] ^= from_to_bitboard;
}

/*
void Board::make_move(Move move) {
	process_move(move);
	
	previous_state = current_state;
	current_state.update(move);
	current_state.toggle_side_to_move();
	previous_move = move;

}
*/
void Board::make_move(const faster::Move& move) {
	if (current_state.side_to_move() == PieceColour::WHITE) 
		process_move<PieceColour::WHITE>(move, false);
	else
		process_move<PieceColour::BLACK>(move, false);
	
	previous_state = current_state;
	current_state.update(move);
	current_state.toggle_side_to_move();
	previous_move_f = move;
}

void Board::undo_move() {
	if (previous_move_f.from != previous_move_f.to) {
		current_state.toggle_side_to_move();
		if (current_state.side_to_move() == PieceColour::WHITE)
			process_move<PieceColour::WHITE>(previous_move_f, true);
		else
			process_move<PieceColour::BLACK>(previous_move_f, true);
		current_state = previous_state;
	}
}

bool Board::is_check() {
	Bitboard king = get_position(current_state.side_to_move(), PieceType::KING);
	Bitboard attackers = attacked_by(pieces::other_colour(current_state.side_to_move()));
	return (king & attackers) > 0;
}

bool Board::is_check(PieceColour colour) {
	Bitboard king = get_position(colour , PieceType::KING);
	Bitboard attackers = attacked_by(pieces::other_colour(colour));
	return (king & attackers) > 0;
}

bool Board::is_checkmate() {
	if (is_check()) {
		std::vector<faster::Move> moves = generate_moves();
		if (moves.empty()) {
			return true;
		}
		else {
			return false;
		}
	}
	return false;
}

/*
void Board::process_move(Move move) {
	// Process capture or quiet move
	if (move.is_capture() && move.is_en_passant()) {
		// reset the half move counter
		Bitboard from_to_bitboard = move.from_bitboard() ^ move.to_bitboard();
		piece_type_bitboard[static_cast<int>(move.get_moved_piece())] ^= from_to_bitboard;
		colour_bitboard[static_cast<int>(current_state.side_to_move())] ^= from_to_bitboard;
		piece_type_bitboard[static_cast<int>(move.get_moved_piece())] ^= move.ep_capture_square_bb();
		colour_bitboard[static_cast<int>(pieces::other_colour(current_state.side_to_move()))] ^= move.ep_capture_square_bb();
	}
	else if (move.is_capture() && !move.is_en_passant()) {
		//reset the half move counter
		Bitboard from_to_bitboard = move.from_bitboard() ^ move.to_bitboard();
		piece_type_bitboard[static_cast<int>(move.get_moved_piece())] ^= from_to_bitboard;
		colour_bitboard[static_cast<int>(current_state.side_to_move())] ^= from_to_bitboard;
		piece_type_bitboard[static_cast<int>(move.get_captured_piece())] ^= move.to_bitboard();
		colour_bitboard[static_cast<int>(pieces::other_colour(current_state.side_to_move()))] ^= move.to_bitboard();

	}
	else if (move.is_castle()) {
		if (move.get_castle_side() == PieceType::KING && current_state.can_king_side_castle()) {
			Bitboard from_bitboard = get_position(current_state.side_to_move(), PieceType::KING);
			Bitboard to_bitboard = bitboard::east_one(bitboard::east_one(from_bitboard));
			Bitboard from_to_bitboard = from_bitboard ^ to_bitboard;
			bitboard::print_board(from_to_bitboard);
			piece_type_bitboard[static_cast<int>(PieceType::KING)] ^= from_to_bitboard;
			colour_bitboard[static_cast<int>(current_state.side_to_move())] ^= from_to_bitboard;

			from_bitboard = bitboard::FILE_H & get_position(current_state.side_to_move(), PieceType::ROOK);
			to_bitboard = bitboard::west_one(bitboard::west_one(from_bitboard));
			from_to_bitboard = from_bitboard ^ to_bitboard;
			piece_type_bitboard[static_cast<int>(PieceType::ROOK)] ^= from_to_bitboard;
			colour_bitboard[static_cast<int>(current_state.side_to_move())] ^= from_to_bitboard;
		}
		else if (move.get_castle_side() == PieceType::QUEEN && current_state.can_queen_side_castle()) {
			Bitboard from_bitboard = get_position(current_state.side_to_move(), PieceType::KING);
			Bitboard to_bitboard = bitboard::west_one(bitboard::west_one(from_bitboard));
			Bitboard from_to_bitboard = from_bitboard ^ to_bitboard;
			bitboard::print_board(from_to_bitboard);
			piece_type_bitboard[static_cast<int>(PieceType::KING)] ^= from_to_bitboard;
			colour_bitboard[static_cast<int>(current_state.side_to_move())] ^= from_to_bitboard;

			from_bitboard = bitboard::FILE_A & get_position(current_state.side_to_move(), PieceType::ROOK);
			to_bitboard = bitboard::east_one(bitboard::east_one(bitboard::east_one(from_bitboard)));
			from_to_bitboard = from_bitboard ^ to_bitboard;
			piece_type_bitboard[static_cast<int>(PieceType::ROOK)] ^= from_to_bitboard;
			colour_bitboard[static_cast<int>(current_state.side_to_move())] ^= from_to_bitboard;
		}
	}
	else {
		// must be quiet move
		Bitboard from_to_bitboard = move.from_bitboard() ^ move.to_bitboard();
		piece_type_bitboard[static_cast<int>(move.get_moved_piece())] ^= from_to_bitboard;
		colour_bitboard[static_cast<int>(current_state.side_to_move())] ^= from_to_bitboard;
	}

	if (move.is_promotion()) {
		piece_type_bitboard[static_cast<int>(PieceType::PAWN)] ^= move.to_bitboard();
		colour_bitboard[static_cast<int>(current_state.side_to_move())] ^= move.to_bitboard();
		piece_type_bitboard[static_cast<int>(move.get_promotion_piece())] ^= move.to_bitboard();
		colour_bitboard[static_cast<int>(current_state.side_to_move())] ^= move.to_bitboard();

	}
}
*/

//void Board::set_ep_target(Square square) {
//	ep_square = square;
//	has_ep_target = true;
//}

std::string Board::to_fen() {
	std::string fen{ "" };
	for (int iRank = 7; iRank >= 0; iRank--) {
		fen += rank_to_fen((Rank) iRank);
		if (iRank != 0) {
			// No slash after last rank
			fen += "/";
		}
	}
	fen += " ";
	fen += current_state.to_fen();
	return fen;
}

bool Board::can_king_side_castle(PieceColour colour) { 
	return current_state.can_king_side_castle(colour);
}

bool Board::can_queen_side_castle(PieceColour colour) { 
	return current_state.can_queen_side_castle(colour);
}



std::string Board::rank_to_fen(Rank rank) {
	std::string fen{ "" };
	int empty_sum = 0;
	for (File iFile = 0; iFile < 8; iFile++) {
		PieceType type = get_piece_type_on(bitboard::square_index(iFile, rank));
		if (type != PieceType::NULL_PIECE) {
			PieceColour colour = get_piece_colour_on(bitboard::square_index(iFile, rank));
			if (empty_sum != 0) {
				fen += std::to_string(empty_sum);
				empty_sum = 0;
			}
			fen += pieces::piece_to_fen_char(type, colour);
		}
		else {
			empty_sum++;
		}
	}
	if (empty_sum != 0) {
		fen += std::to_string(empty_sum);
	}
	
	return fen;
}

Bitboard Board::pawn_attacks(PieceColour colour) {
	//std::unique_ptr<pieces::Pawns> pawns = factory.make_pawns(colour);
	Bitboard pawns_bitboard = get_position(colour, PieceType::PAWN);

	return (colour == PieceColour::BLACK) ? faster::pawn_attacks<PieceColour::BLACK>(pawns_bitboard) : faster::pawn_attacks<PieceColour::WHITE>(pawns_bitboard);
}


Bitboard Board::piece_attacks(PieceType type, PieceColour colour) {
	//std::unique_ptr<pieces::Piece> piece = factory.make_piece(type);
	//Bitboard piece_bitboard = get_position(colour, type);
	switch (type) {
		case PieceType::PAWN: return pawn_attacks(colour);
		case PieceType::KNIGHT: return faster::piece_attacks<PieceType::KNIGHT>(get_position(colour, type), occupied_squares());
		case PieceType::BISHOP: return faster::piece_attacks<PieceType::BISHOP>(get_position(colour, type), occupied_squares());
		case PieceType::ROOK: return faster::piece_attacks<PieceType::ROOK>(get_position(colour, type), occupied_squares());
		case PieceType::QUEEN: return faster::piece_attacks<PieceType::QUEEN>(get_position(colour, type), occupied_squares());
		case PieceType::KING: return faster::piece_attacks<PieceType::KING>(get_position(colour, type), occupied_squares());
		default: return Bitboard{ 0 };
	}
}

Bitboard Board::all_attacks(PieceColour colour) {
	Bitboard attacked_squares{ 0 };
	Bitboard friendly_pieces = get_position(colour);
	attacked_squares |= (pawn_attacks(colour) & ~friendly_pieces);
	attacked_squares |= (piece_attacks(PieceType::KNIGHT, colour) & ~friendly_pieces);
	attacked_squares |= (piece_attacks(PieceType::BISHOP, colour) & ~friendly_pieces);
	attacked_squares |= (piece_attacks(PieceType::ROOK, colour) & ~friendly_pieces);
	attacked_squares |= (piece_attacks(PieceType::QUEEN, colour) & ~friendly_pieces);
	attacked_squares |= (piece_attacks(PieceType::KING, colour) & ~friendly_pieces);
	return attacked_squares;
}


CastlingRights::CastlingRights(std::string fen) {
	if (fen.empty() || (fen.size() == 1 && fen[0] == '-')) {
		return;
	}
	else {
		for (auto character : fen) {
			switch (character) {
				case 'K':
					white[0] = true;
					break;
				case 'Q':
					white[1] = true;
					break;
				case 'k':
					black[0] = true;
					break;
				case 'q':
					black[1] = true;
					break;
			}
		}
		
	}
}

bool CastlingRights::king_side(PieceColour colour) {
	if (colour == PieceColour::BLACK) {
		return black[0];
	}
	else {
		return white[0];
	}
}

bool CastlingRights::queen_side(PieceColour colour) {
	if (colour == PieceColour::BLACK) {
		return black[1];
	}
	else {
		return white[1];
	}
}

void CastlingRights::update(const faster::Move& move, PieceColour side) {
	if (move.castle != PieceType::NULL_PIECE) {
		if (side == PieceColour::WHITE) {
			white[0] = false;
			white[1] = false;
		}
		else {
			black[0] = false;
			black[1] = false;
		}
		return;
	}
	white[0] = !(move.from == 7 || move.to == 7 || move.from == 4) && white[0];
	white[1] = !(move.from == 0 || move.to == 0 || move.from == 4) && white[1];
	black[0] = !(move.from == 63 || move.to == 63 || move.from == 60) && black[0];
	black[1] = !(move.from == 56 || move.to == 56 || move.from == 60) && black[1];
	
}

std::string CastlingRights::fen() {
	std::string fen_string = "";
	fen_string += white[0] ? "K" : "";
	fen_string += white[1] ? "Q" : "";
	fen_string += black[0] ? "k" : "";
	fen_string += black[1] ? "q" : "";
	if (fen_string.empty()) {
		fen_string += "-";
	}
	return fen_string;
}

void GameState::parse_fen(std::string fen) {
	auto start_of_castling_rights = fen.find_first_of(" ");
	auto start_of_ep_target = fen.find_first_of(" ", start_of_castling_rights + 1);
	auto start_of_move_number = fen.find_first_of(" ", start_of_ep_target + 1);
	if (fen[0] == 'w') {
		side = PieceColour::WHITE;
	}
	else if (fen[0] == 'b') {
		side = PieceColour::BLACK;
	}

	if (fen[start_of_ep_target + 1] == '-') {
		has_ep_target = false;
	}

	else {
		has_ep_target = true;
		ep_square = bitboard::square_index(fen.substr(start_of_ep_target + 1, 2));
	}
	int length_castling_rights = start_of_ep_target - start_of_castling_rights;
	castling_rights = CastlingRights(fen.substr(start_of_castling_rights + 1, length_castling_rights));
}

std::string GameState::to_fen() {
	std::string fen = "";
	if (side == PieceColour::WHITE) {
		fen += 'w';
	}
	else {
		fen += 'b';
	}

	fen += " " + castling_rights.fen() + " ";

	fen += has_ep_target ? bitboard::to_algebraic(ep_square) : "-";

	fen += " 0 ";
	fen += std::to_string(move_clock);
	return fen;
}
/*
void GameState::update(Move move) {
	if (move.is_double_push()) {
		has_ep_target = true;
		if (side == PieceColour::WHITE) {
			ep_square = move.to_square() - 8;
		}
		else {
			ep_square = move.to_square() + 8;
		}
	}
	else {
		has_ep_target = false;
	}

	castling_rights.update(move, side);
	if (side == PieceColour::BLACK) {
		move_clock++;
	}
}
*/

void GameState::update(const faster::Move& move) {
	if (move.double_push) {
		has_ep_target = true;
		if (side == PieceColour::WHITE) {
			ep_square = move.to - 8;
		}
		else {
			ep_square = move.to + 8;
		}
	}
	else {
		has_ep_target = false;
	}

	castling_rights.update(move, side);
	if (side == PieceColour::BLACK) {
		move_clock++;
	}
}

bool GameState::can_king_side_castle(PieceColour colour) {
	return castling_rights.king_side(colour);
}

bool GameState::can_king_side_castle() {
	return castling_rights.king_side(side);
}

bool GameState::can_queen_side_castle(PieceColour colour) {
	return castling_rights.queen_side(colour);
}

bool GameState::can_queen_side_castle() {
	return castling_rights.queen_side(side);
}

void GameState::toggle_side_to_move() {
	if (side == PieceColour::BLACK) {
		side = PieceColour::WHITE;
	}
	else {
		side = PieceColour::BLACK;
	}

}

Bitboard GameState::ep_target() {
	if (has_ep_target) {
		return bitboard::to_bitboard(ep_square);
	}
	else {
		return Bitboard(0);
	}

}
