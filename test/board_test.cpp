#include <gtest/gtest.h>
#include "../src/board.h"
#include "../src/bitboard.h"
#include "../src/pieces.h"

#include <string>

TEST(MoveTest, DoublePawnPushCreatesEpTargetSquare) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
	std::string expected{ "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3 0 1" };
  yak::Board board(fen);
	auto move = yak::piece::make_double_push(8, 24);

	// Act
	board.make_move(move);

	// Assert
	EXPECT_EQ(board.to_fen(), expected);

}

TEST(MoveTest, CanMakeEpCapture) {
	// Arrange
	std::string fen{ "8/8/8/8/Pp6/1P6/8/8 b KQkq a3 0 1" };
	std::string expected{ "8/8/8/8/8/pP6/8/8 w KQkq - 0 2" };
	yak::Board board(fen);
	auto move = yak::piece::make_ep_capture(25, 16);
	//move.set_en_passant(yak::bitboard::to_yak::bitboard(Square(24)));
	
	// Act
	board.make_move(move);


	// Assert
	EXPECT_EQ(board.to_fen(), expected);
}

/*
TEST(MoveTest, CanConvertPawnMoveToAlgebraic) {
	// Arrange
	Move move(Square(25), Square(16), PieceType::PAWN);
	std::string expected = "a3";

	// Act
	std::string actual = move.to_algebraic();

	// Assert
	EXPECT_EQ(actual, expected);
}
*/
TEST(MoveFactoryTest, CanCreatePawnPushMove) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
	std::string expected{ "rnbqkbnr/pppppppp/8/8/8/P7/1PPPPPPP/RNBQKBNR b KQkq - 0 1" };
	yak::Board board(fen);

	// Act
	auto pawn_push = yak::piece::make_quiet(8, 16);
	//Move pawn_push = mf.create_pawn_push(8, 16);

	// Assert
	board.make_move(pawn_push);
	EXPECT_EQ(board.to_fen(), expected);
}

TEST(MoveFactoryTest, CanCreatePawnDoublePushMove) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
	std::string expected{ "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3 0 1" };
	yak::Board board(fen);

	// Act
	yak::piece::Move pawn_push = yak::piece::make_double_push(8, 24);

	// Assert
	board.make_move(pawn_push);
	EXPECT_EQ(board.to_fen(), expected);
}
TEST(BoardTest, BoardCanReturnCorrectPawnPositions) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
	yak::Board board(fen);
	auto expected = yak::bitboard::RANK_2 | yak::bitboard::RANK_7;

	// Act
	Bitboard actual = board.get_position(PieceType::PAWN);

	// Assert
	EXPECT_EQ(actual, expected);
}

TEST(BoardTest, BoardCanReturnCorrectKnightPositions) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
	yak::Board board(fen);
	Bitboard expected = (yak::bitboard::RANK_1 | yak::bitboard::RANK_8) & (yak::bitboard::FILE_B | yak::bitboard::FILE_G);

	// Act
	Bitboard actual = board.get_position(PieceType::KNIGHT);

	// Assert
	EXPECT_EQ(actual, expected);
}

TEST(BoardTest, BoardCanReturnCorrectBishopPositions) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
	yak::Board board(fen);
	Bitboard expected = (yak::bitboard::RANK_1 | yak::bitboard::RANK_8) & (yak::bitboard::FILE_C | yak::bitboard::FILE_F);

	// Act
	Bitboard actual = board.get_position(PieceType::BISHOP);

	// Assert
	EXPECT_EQ(actual, expected);
}


TEST(BoardTest, BoardCanReturnCorrectRookPositions) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
	yak::Board board(fen);
	Bitboard expected = (yak::bitboard::RANK_1 | yak::bitboard::RANK_8) & (yak::bitboard::FILE_A | yak::bitboard::FILE_H);

	// Act
	Bitboard actual = board.get_position(PieceType::ROOK);

	// Assert
	EXPECT_EQ(actual, expected);
}

TEST(BoardTest, BoardCanReturnCorrectQueenPositions) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
	Bitboard expected = (yak::bitboard::RANK_1 | yak::bitboard::RANK_8) & yak::bitboard::FILE_D;

	// Act
	Bitboard actual = board.get_position(PieceType::QUEEN);

	// Assert
	EXPECT_EQ(actual, expected);
}

TEST(BoardTest, BoardCanReturnCorrectKingPositions) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
	Bitboard expected = (yak::bitboard::RANK_1 | yak::bitboard::RANK_8) & yak::bitboard::FILE_E;

	// Act
	Bitboard actual = board.get_position(PieceType::KING);

	// Assert
	EXPECT_EQ(actual, expected);
}

TEST(BoardTest, BoardCanReturnCorrectWhitePieces) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
	Bitboard expected = (yak::bitboard::RANK_1 | yak::bitboard::RANK_2);

	// Act
	Bitboard actual = board.get_position(PieceColour::WHITE);

	// Assert
	EXPECT_EQ(actual, expected);
}

TEST(BoardTest, BoardCanReturnCorrectBlackPieces) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
	Bitboard expected = (yak::bitboard::RANK_7 | yak::bitboard::RANK_8);

	// Act
	Bitboard actual = board.get_position(PieceColour::BLACK);

	// Assert
	EXPECT_EQ(actual, expected);
}

TEST(BoardTest, BoardCanReturnCorrectWhitePawnPositions) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
	Bitboard expected = yak::bitboard::RANK_2;

	// Act
	Bitboard actual = board.get_position(PieceColour::WHITE, PieceType::PAWN);

	// Assert
	EXPECT_EQ(actual, expected);
}

TEST(BoardTest, BoardCanReturnCorrectBlackPawnPositions) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
	Bitboard expected = yak::bitboard::RANK_7;

	// Act
	Bitboard actual = board.get_position(PieceColour::BLACK, PieceType::PAWN);

	// Assert
	EXPECT_EQ(actual, expected);
}

TEST(BoardTest, BoardCanReturnCorrectWhiteKnightPositions) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
	Bitboard expected = yak::bitboard::RANK_1 & (yak::bitboard::FILE_B | yak::bitboard::FILE_G);

	// Act
	Bitboard actual = board.get_position(PieceColour::WHITE, PieceType::KNIGHT);

	// Assert
	EXPECT_EQ(actual, expected);
}

TEST(BoardTest, BoardCanReturnCorrectBlackKnightPositions) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
	Bitboard expected = yak::bitboard::RANK_8 & (yak::bitboard::FILE_B | yak::bitboard::FILE_G);

	// Act
	Bitboard actual = board.get_position(PieceColour::BLACK, PieceType::KNIGHT);

	// Assert
	EXPECT_EQ(actual, expected);
}

TEST(BoardTest, BoardCanReturnCorrectWhiteBishopPositions) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
	Bitboard expected = yak::bitboard::RANK_1 & (yak::bitboard::FILE_C | yak::bitboard::FILE_F);

	// Act
	Bitboard actual = board.get_position(PieceColour::WHITE, PieceType::BISHOP);

	// Assert
	EXPECT_EQ(actual, expected);
}

TEST(BoardTest, BoardCanReturnCorrectBlackBishopPositions) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
	Bitboard expected = yak::bitboard::RANK_8 & (yak::bitboard::FILE_C | yak::bitboard::FILE_F);

	// Act
	Bitboard actual = board.get_position(PieceColour::BLACK, PieceType::BISHOP);

	// Assert
	EXPECT_EQ(actual, expected);
}

TEST(BoardTest, BoardCanReturnCorrectWhiteRookPositions) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
	Bitboard expected = yak::bitboard::RANK_1 & (yak::bitboard::FILE_A | yak::bitboard::FILE_H);

	// Act
	Bitboard actual = board.get_position(PieceColour::WHITE, PieceType::ROOK);

	// Assert
	EXPECT_EQ(actual, expected);
}

TEST(BoardTest, BoardCanReturnCorrectBlackRookPositions) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
	Bitboard expected = yak::bitboard::RANK_8 & (yak::bitboard::FILE_A | yak::bitboard::FILE_H);

	// Act
	Bitboard actual = board.get_position(PieceColour::BLACK, PieceType::ROOK);

	// Assert
	EXPECT_EQ(actual, expected);
}

TEST(BoardTest, BoardCanReturnCorrectWhiteQueenPositions) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
	Bitboard expected = yak::bitboard::RANK_1 & yak::bitboard::FILE_D;

	// Act
	Bitboard actual = board.get_position(PieceColour::WHITE, PieceType::QUEEN);

	// Assert
	EXPECT_EQ(actual, expected);
}

TEST(BoardTest, BoardCanReturnCorrectBlackQueenPositions) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
	Bitboard expected = yak::bitboard::RANK_8 & yak::bitboard::FILE_D;

	// Act
	Bitboard actual = board.get_position(PieceColour::BLACK, PieceType::QUEEN);

	// Assert
	EXPECT_EQ(actual, expected);
}

TEST(BoardTest, BoardCanReturnCorrectWhiteKingPositions) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
	Bitboard expected = yak::bitboard::RANK_1 & yak::bitboard::FILE_E;

	// Act
	Bitboard actual = board.get_position(PieceColour::WHITE, PieceType::KING);

	// Assert
	EXPECT_EQ(actual, expected);
}

TEST(BoardTest, BoardCanReturnCorrectBlackKingPositions) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
	Bitboard expected = yak::bitboard::RANK_8 & yak::bitboard::FILE_E;

	// Act
	Bitboard actual = board.get_position(PieceColour::BLACK, PieceType::KING);

	// Assert
	EXPECT_EQ(actual, expected);
}

TEST(BoardTest, CanReturnEmptySquares) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
	Bitboard expected = yak::bitboard::RANK_3 | yak::bitboard::RANK_4 | yak::bitboard::RANK_5 | yak::bitboard::RANK_6;

	// Act
	Bitboard actual = board.empty_squares();

	// Assert
	EXPECT_EQ(actual, expected);
}

TEST(BoardTest, EpSquareLoads) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3 0 1" };
  yak::Board board(fen);

	// Act
	std::string actual = board.to_fen();

	// Assert
	EXPECT_EQ(actual, fen);
}

TEST(BoardTest, DetectsCastlingRightsFromFenAll) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3 0 1" };
  yak::Board board(fen);

	// Assert
	EXPECT_TRUE(board.can_king_side_castle(PieceColour::BLACK));
	EXPECT_TRUE(board.can_king_side_castle(PieceColour::WHITE));
	EXPECT_TRUE(board.can_queen_side_castle(PieceColour::BLACK));
	EXPECT_TRUE(board.can_queen_side_castle(PieceColour::WHITE));
}

TEST(BoardTest, DetectsCastlingRightsFromFenNoKingSideWhite) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b Qkq a3 0 1" };
  yak::Board board(fen);

	// Assert
	EXPECT_TRUE(board.can_king_side_castle(PieceColour::BLACK));
	EXPECT_FALSE(board.can_king_side_castle(PieceColour::WHITE));
	EXPECT_TRUE(board.can_queen_side_castle(PieceColour::BLACK));
	EXPECT_TRUE(board.can_queen_side_castle(PieceColour::WHITE));
}

TEST(BoardTest, CastlingRightsChangeWhenRookIsCaptured) {
	// Arrange
	std::string fen{ "rnbqkbnr/ppppppp1/8/8/8/8/PPPPPPP1/RNBQKBNR b KQkq - 0 1" };
	std::string expected{ "rnbqkbn1/ppppppp1/8/8/8/8/PPPPPPP1/RNBQKBNr w Qq - 0 2" };
  yak::Board board(fen);

	yak::piece::Move move = yak::piece::make_capture(63, 7);
	//move.set_capture(PieceType::ROOK);

	board.make_move(move);

	std::string actual = board.to_fen();

	// Assert
	EXPECT_EQ(actual, expected);
}

TEST(BoardTest, UndoingMoveRestoresState) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
	yak::piece::Move move = yak::piece::make_quiet(yak::bitboard::square_index("e2"), yak::bitboard::square_index("e4"));
	board.make_move(move);

	//std::cout << board.to_fen() << std::endl;

	// Act
	board.undo_move();

	// Assert
	EXPECT_EQ(board.to_fen(), fen);
}

TEST(BoardTest, CalculatesAllAttackedSquares) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);

	// Act
	Bitboard white_actual = board.attacked_by(PieceColour::WHITE);
	Bitboard black_actual = board.attacked_by(PieceColour::BLACK);

	// Assert
	EXPECT_EQ(white_actual, yak::bitboard::RANK_3);
	EXPECT_EQ(black_actual, yak::bitboard::RANK_6);
}

TEST(BoardTest, CalculatesAllAttackedSquares2) {
	// Arrange
	std::string fen{ "r1bqkbnr/pppp1ppp/2n5/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1" };
  yak::Board board(fen);
		
		// Pawns
		Bitboard white_expected = yak::bitboard::to_bitboard("a3");
		white_expected |= yak::bitboard::to_bitboard("b3");
		white_expected |= yak::bitboard::to_bitboard("c3");
		white_expected |= yak::bitboard::to_bitboard("d3");
		white_expected |= yak::bitboard::to_bitboard("e3");
		white_expected |= yak::bitboard::to_bitboard("g3");
		white_expected |= yak::bitboard::to_bitboard("h3");
		white_expected |= yak::bitboard::to_bitboard("f5");

		// Knights
		white_expected |= yak::bitboard::to_bitboard("d4");
		white_expected |= yak::bitboard::to_bitboard("e5");
		white_expected |= yak::bitboard::to_bitboard("g5");
		white_expected |= yak::bitboard::to_bitboard("h4");
		white_expected |= yak::bitboard::to_bitboard("g1");

		// Bishop
		white_expected |= yak::bitboard::to_bitboard("a6");
		white_expected |= yak::bitboard::to_bitboard("b5");
		white_expected |= yak::bitboard::to_bitboard("d3");
		white_expected |= yak::bitboard::to_bitboard("e2");
		white_expected |= yak::bitboard::to_bitboard("f1");
		white_expected |= yak::bitboard::to_bitboard("b3");
		white_expected |= yak::bitboard::to_bitboard("d5");
		white_expected |= yak::bitboard::to_bitboard("e6");
		white_expected |= yak::bitboard::to_bitboard("f7");

		// King
		white_expected |= yak::bitboard::to_bitboard("e2") | yak::bitboard::to_bitboard("f1");

	// Act
	Bitboard white_actual = board.attacked_by(PieceColour::WHITE);
	Bitboard black_actual = board.attacked_by(PieceColour::BLACK);
  yak::bitboard::print_board(white_actual);
  yak::bitboard::print_board(white_expected);
	// Assert
	EXPECT_EQ(white_actual, white_expected);
}

TEST(BoardTest, CanDetectCheck) {
	// Arrange
	std::string fen{ "rnbqkbnr/ppp2ppp/3p4/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1" };
  yak::Board board(fen);

	// Act
	bool actual = board.is_check();

	// Assert
	EXPECT_TRUE(actual);
}

TEST(BoardTest, CanDetectNotCheck) {
	// Arrange
	std::string fen{ "rnbqkbnr/pp3ppp/2pp4/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1" };
  yak::Board board(fen);

	// Act
	bool actual = board.is_check();

	// Assert
	EXPECT_FALSE(actual);
}

TEST(BoardMoveGenerationTests, DetectsLegalCapturesWithWhitePawns) {

	//   Test Board       W Pawn Targets           Targets
	// . . . . . . . .    . . . . . . . .    . . . . . . . . 
	// . . . . . . . .    . . . . . . . .    . . . . . . . . 
	// . . . . . . . .    . . . . . . . .    1 1 . 1 1 1 1 1 
	// . . . . . . . .    . . . . . . . .    . . . . . . . . 
	// . . . . . . . .    . 1 . . . . . .    . . . . . . . . 
	// . . p . . . . .    . 1 1 . . . . .    . . . . . . . . 
	// . P . . . . . .    . . . . . . . .    . . . . . . . . 
	// . . . . . . . .    . . . . . . . .    . . . . . . . . 


	// Arrange
	std::string fen = "8/8/8/8/8/2p5/1P6/8 w - - 0 1";
  yak::Board board(fen);

	// Act
	std::vector<yak::piece::Move> moves = board.generate_moves();

	// Assert
	EXPECT_EQ(moves.size(), 3);
}

TEST(BoardMoveGenerationTests, DetectsLegalCapturesWithBlackPawns) {

	//   Test Board       W Pawn Targets           Targets
	// . . . . . . . .    . . . . . . . .    . . . . . . . . 
	// . P . . . . . .    . . . . . . . .    . . . . . . . . 
	// . . p . . . . .    . 1 1 . . . . .    1 1 . 1 1 1 1 1 
	// . . . . . . . .    . 1 . . . . . .    . . . . . . . . 
	// . . . . . . . .    . . . . . . . .    . . . . . . . . 
	// . . . . . . . .    . . . . . . . .    . . . . . . . . 
	// . . . . . . . .    . . . . . . . .    . . . . . . . . 
	// . . . . . . . .    . . . . . . . .    . . . . . . . . 


	// Arrange
	std::string fen = "8/1p6/2P5/8/8/8/8/8 b - - 0 1";
  yak::Board board(fen);

	// Act
	std::vector<yak::piece::Move> moves = board.generate_moves();

	// Assert
	EXPECT_EQ(moves.size(), 3);
}

TEST(BoardMoveGenerationTests, Kiwipete) {
	// Arrange
	std::string fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";
  yak::Board board(fen);

	// Act
	std::vector<yak::piece::Move> moves = board.generate_moves();

	// Assert
	EXPECT_EQ(moves.size(), 48);
}
/*
TEST(BoardMoveGenerationTests, KiwipeteFast) {
	// Arrange
	std::string fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";
	Board board = Board(fen);
	yak::piece::Move move_list[330];
	yak::piece::SpecialisedRay<Direction::NORTH> n_ray;
	yak::piece::SpecialisedRay<Direction::EAST> e_ray;
	yak::piece::SpecialisedRay<Direction::SOUTH> s_ray;
	yak::piece::SpecialisedRay<Direction::WEST> w_ray;
	std::vector<yak::piece::Ray*> rook_atks{&n_ray, & e_ray, & s_ray, & w_ray};
	int move_counter{ 0 };

	// Act
	yak::piece::generate_pawn_moves(PieceColour::WHITE, &move_list[0], move_counter,
		board.get_position(PieceColour::WHITE, PieceType::PAWN),
		board.empty_squares(), board.get_position(PieceColour::BLACK));
	yak::piece::generate_sliding_piece_moves(rook_atks, &move_list[0], move_counter,
		board.get_position(PieceColour::WHITE, PieceType::ROOK),
		board.empty_squares(), board.get_position(PieceColour::BLACK));

	// Assert
	EXPECT_EQ(move_counter, 13);
}
*/
TEST(BoardMoveGenerationTests, CanGenerateAndMakeEpCapture) {

	//  Before Capture     After Capture
	// . . . . . . . .    . . . . . . . . 
	// . . . . . . . .    . . . . . . . .
	// . . . . . . . .    . . . . . . . .
	// . . . . . . . .    . . . . . . . .
	// P p . . . . . .    . . . . . . . .
	// . P . . . . . .    p P . . . . . .
	// . . . . . . . .    . . . . . . . .
	// . . . . . . . .    . . . . . . . .
	
	// Arrange
	std::string fen{ "8/8/8/8/Pp6/1P6/8/8 b - a3 0 1" };
	std::string expected{ "8/8/8/8/8/pP6/8/8 w - - 0 2" };
  yak::Board board(fen);

	std::vector<yak::piece::Move> moves = board.generate_moves();

	// Act
	for (auto move : moves) {
		if (move.en_passant) {
			board.make_move(move);
			break;
		}
		
	}

	// Assert
	EXPECT_EQ(board.to_fen(), expected);
}

TEST(BoardMoveGenerationTests, CanCastleKingSideWhiteOnlyKingAndRook) {
	// Arrange
	std::string fen{ "8/8/8/8/8/8/8/4K2R w K - 0 1" };
	std::string expected{ "8/8/8/8/8/8/8/5RK1 b - - 0 1" };
  yak::Board board(fen);

	std::vector<yak::piece::Move> moves = board.generate_moves();

	// Act
	for (auto move : moves) {
		if (move.castle != PieceType::NULL_PIECE) {
			board.make_move(move);
			break;
		}

	}

	// Assert
	EXPECT_EQ(board.to_fen(), expected);
}


TEST(BoardMoveGenerationTests, CanCastleKingSideWhite) {
	// Arrange
	std::string fen{ "rnbqkbnr/ppp2ppp/3p4/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1" };
	std::string expected{ "rnbqkbnr/ppp2ppp/3p4/4p3/2B1P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 0 1" };
  yak::Board board(fen);

	std::vector<yak::piece::Move> moves = board.generate_moves();

	// Act
	for (auto move : moves) {
		if (move.castle != PieceType::NULL_PIECE) {
			board.make_move(move);
			break;
		}

	}

	// Assert
	EXPECT_EQ(board.to_fen(), expected);
}

TEST(BoardMoveGenerationTests, CanCastleKingSideBlack) {
	// Arrange
	std::string fen{ "rnbqk2r/ppp2pbp/3p1np1/3Pp3/2B1P3/5N2/PPP2PPP/RNBQ1RK1 b kq - 0 1" };
	std::string expected{ "rnbq1rk1/ppp2pbp/3p1np1/3Pp3/2B1P3/5N2/PPP2PPP/RNBQ1RK1 w - - 0 2" };
  yak::Board board = yak::Board(fen);

	std::vector<yak::piece::Move> moves = board.generate_moves();

	// Act
	for (auto move : moves) {
		if (move.castle != PieceType::NULL_PIECE) {
			board.make_move(move);
			break;
		}

	}

	// Assert
	EXPECT_EQ(board.to_fen(), expected);
}

TEST(BoardMoveGenerationTests, CanCastleQueenSideWhite) {
	// Arrange
	std::string fen{ "rnbqk2r/ppp2pbp/3p1np1/3Pp3/2B1P3/2N1B3/PPPQ1PPP/R3K1NR w KQkq - 0 1" };
	std::string expected{ "rnbqk2r/ppp2pbp/3p1np1/3Pp3/2B1P3/2N1B3/PPPQ1PPP/2KR2NR b kq - 0 1" };
  yak::Board board = yak::Board(fen);

	std::vector<yak::piece::Move> moves = board.generate_moves();

	// Act
	for (auto move : moves) {
		if (move.castle != PieceType::NULL_PIECE) {
			board.make_move(move);
			break;
		}

	}

	// Assert
	EXPECT_EQ(board.to_fen(), expected);
}

TEST(BoardMoveGenerationTests, CanCastleQueenSideBlack) {
	// Arrange
	std::string fen{ "r3kbnr/ppp2ppp/3pb3/4p1q1/4Pn2/3P4/PPP2PPP/RNBQKBNR b KQkq - 0 1" };
	std::string expected{ "2kr1bnr/ppp2ppp/3pb3/4p1q1/4Pn2/3P4/PPP2PPP/RNBQKBNR w KQ - 0 2" };
  yak::Board board(fen);

	std::vector<yak::piece::Move> moves = board.generate_moves();

	// Act
	for (auto& move : moves) {
		if (move.castle != PieceType::NULL_PIECE) {
			board.make_move(move);
			break;
		}

	}

	// Assert
	EXPECT_EQ(board.to_fen(), expected);
}

TEST(BoardMoveGenerationTests, PawnCanPromote) {
	// Arrange
	std::string fen{ "8/P7/8/8/8/8/8/8 w - - 0 1" };
  yak::Board board(fen);

	std::vector<yak::piece::Move> moves = board.generate_moves();

	// Act & Assert
	
	board.make_move(moves[0]);
	EXPECT_EQ(board.to_fen(), "N7/8/8/8/8/8/8/8 b - - 0 1");
	board.undo_move();

	board.make_move(moves[1]);
	EXPECT_EQ(board.to_fen(), "B7/8/8/8/8/8/8/8 b - - 0 1");
	board.undo_move();

	board.make_move(moves[2]);
	EXPECT_EQ(board.to_fen(), "R7/8/8/8/8/8/8/8 b - - 0 1");
	board.undo_move();

	board.make_move(moves[3]);
	EXPECT_EQ(board.to_fen(), "Q7/8/8/8/8/8/8/8 b - - 0 1");
	board.undo_move();
}

TEST(MoveTest, FirstMoveOfTheGameHas20LegalMoves) {
	// Arrange
	std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);

	// Act
	std::vector<yak::piece::Move> moves = board.generate_moves();

	// Assert
	EXPECT_EQ(moves.size(), 20);
}
TEST(BoardMoveGenerationTests, DetectsAllMovesInPosition) {

	//   Test Board    
	// r n b q k b n r  
	// p p p . . p p p  
	// . . . p . . . .   
	// . . . . p . . .   
	// . . . . P . . .  
	// . . . . . N . . 
	// P P P P . P P P 
	// R N B Q K B . R 


	// Arrange
	std::string fen = "rnbqkbnr/ppp2ppp/3p4/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 0 1";
  yak::Board board(fen);

	// Act
	std::vector<yak::piece::Move> moves = board.generate_moves();

	// Assert
	EXPECT_EQ(moves.size(), 27);

}

TEST(BoardMoveGenerationTests, DetectsAllMovesInPosition2) {

	//   Test Board       
	// r n b q k b n r 
	// p p p . . . p p 
	// . . . p . p . . 
	// . . . . p . . . 
	// . . . P P . . . 
	// . . . . . N . . 
	// P P P . . P P P 
	// R N B Q K B . R


	// Arrange
	std::string fen = "rnbqkbnr/ppp3pp/3p1p2/4p3/3PP3/5N2/PPP2PPP/RNBQKB1R w KQkq - 0 1";
  yak::Board board(fen);

	// Act
	std::vector<yak::piece::Move> moves = board.generate_moves();

	// Assert
	EXPECT_EQ(moves.size(), 36);

}

TEST(BoardMoveGenerationTests, DetectsAllMovesInPosition3) {

	//   Test Board    
	// r n b q k b n r
	// p p . . . . p p 
	// . . p p . p . .
	// . B . . p . . .
	// . . . P P . . .
	// . . . . . N . .
	// P P P . . P P P
	// R N B Q K . . R 


	// Arrange
	std::string fen = "rnbqkbnr/pp4pp/2pp1p2/1B2p3/3PP3/5N2/PPP2PPP/RNBQK2R w KQkq - 0 1";
  yak::Board board(fen);

	// Act
	std::vector<yak::piece::Move> moves = board.generate_moves();

	// Assert
	EXPECT_EQ(moves.size(), 41);

}

TEST(BoardMoveGenerationTests, DetectsLegalMovesWhenInCheck) {

	//   Test Board    
	// r n b q k b n r 
	// p p p . . p p p
	// . . . p . . . . 
	// . B . . p . . . 
	// . . . . P . . . 
	// . . . . . N . . 
	// P P P P . P P P
	// R N B Q K . . R  


	// Arrange
	std::string fen = "rnbqkbnr/ppp2ppp/3p4/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1";
  yak::Board board(fen);

	// Act
	std::vector<yak::piece::Move> moves = board.generate_moves();

	for (auto move : moves)
		std::cout << move.to_algebraic() << std::endl;

	// Assert
	EXPECT_EQ(moves.size(), 6); // The only legal moves are the six that remove the check: c6, Nc6, Nd7, Bd7, Qc7, Ke7

}

TEST(BoardMoveGenerationTests, DetectsASituationThatIsNOTCheckmate) {

	//   Test Board    
	// r n b q k b n r   
	// p p p . . p p p  
	// . . . p . . . .  
	// . B . . p . . .  
	// . . . . P . . .  
	// . . . . . N . . 
	// P P P P . P P P  
	// R N B Q K . . R 


	// Arrange
	std::string fen = "rnbqkbnr/ppp2ppp/3p4/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1";
  yak::Board board = yak::Board(fen);

	// Act
	bool actual = board.is_checkmate();

	// Assert
	EXPECT_FALSE(actual);

}

TEST(BoardMoveGenerationTests, DetectsASituationThatIsCheckmate) {

	//   Test Board    
	// r . . q . b k r   
	// p p p . . . p p  
	// . . n . B . . .  
	// . . . . p . . .  
	// . . . . . . . .  
	// . . . . . Q . . 
	// P P P P . P P P  
	// R N B   K . . R 

	// Arrange
	std::string fen = "r2q1bkr/ppp3pp/2n1B3/4p3/8/5Q2/PPPP1PPP/RNB1K2R b KQkq - 0 1";
  yak::Board board = yak::Board(fen);

	// Act
	bool actual = board.is_checkmate();

	// Assert
	EXPECT_TRUE(actual);

}

TEST(BoardMoveGenerationTests, CannotCastleOutOfCheck) {

	//   Test Board    
	// . . . . . . . .   
	// . . . . . . . .   
	// . . . . r . . .   
	// . . . . . . . .   
	// . . . . . . . .  
	// . . . . . . . .   
	// . . . . . . . .   
	// . . . . K . . R 

	// Arrange
	std::string fen = "8/8/4r3/8/8/8/8/4K2R w K - 0 1";
  yak::Board board = yak::Board(fen);

	std::vector<yak::piece::Move> moves = board.generate_moves();

	// Assert
	for (auto& move : moves) {
		EXPECT_TRUE(move.castle == PieceType::NULL_PIECE);
	}
	

}
TEST(CastlingRightsTests, CanBeCreatedFromFenAll) {
	// Arrange
	std::string fen = "KQkq";
	yak::CastlingRights castling_rights(fen);

	// Assert
	EXPECT_TRUE(castling_rights.king_side(PieceColour::WHITE));
	EXPECT_TRUE(castling_rights.king_side(PieceColour::BLACK));
	EXPECT_TRUE(castling_rights.queen_side(PieceColour::WHITE));
	EXPECT_TRUE(castling_rights.queen_side(PieceColour::BLACK));
}

TEST(CastlingRightsTests, CanBeCreatedFromFenNone) {
	// Arrange
	std::string fen = "-";
	yak::CastlingRights castling_rights(fen);

	// Assert
	EXPECT_FALSE(castling_rights.king_side(PieceColour::WHITE));
	EXPECT_FALSE(castling_rights.king_side(PieceColour::BLACK));
	EXPECT_FALSE(castling_rights.queen_side(PieceColour::WHITE));
	EXPECT_FALSE(castling_rights.queen_side(PieceColour::BLACK));
}

TEST(CastlingRightsTests, CanBeCreatedFromFenOnlyWhite) {
	// Arrange
	std::string fen = "KQ";
	yak::CastlingRights castling_rights(fen);

	// Assert
	EXPECT_TRUE(castling_rights.king_side(PieceColour::WHITE));
	EXPECT_FALSE(castling_rights.king_side(PieceColour::BLACK));
	EXPECT_TRUE(castling_rights.queen_side(PieceColour::WHITE));
	EXPECT_FALSE(castling_rights.queen_side(PieceColour::BLACK));
}

TEST(CastlingRightsTests, CanBeCreatedFromFenOnlyBlack) {
	// Arrange
	std::string fen = "kq";
	yak::CastlingRights castling_rights(fen);

	// Assert
	EXPECT_FALSE(castling_rights.king_side(PieceColour::WHITE));
	EXPECT_TRUE(castling_rights.king_side(PieceColour::BLACK));
	EXPECT_FALSE(castling_rights.queen_side(PieceColour::WHITE));
	EXPECT_TRUE(castling_rights.queen_side(PieceColour::BLACK));
}

TEST(CastlingRightsTests, CanBeCreatedFromFenOnlyKings) {
	// Arrange
	std::string fen = "Kk";
	yak::CastlingRights castling_rights(fen);

	// Assert
	EXPECT_TRUE(castling_rights.king_side(PieceColour::WHITE));
	EXPECT_TRUE(castling_rights.king_side(PieceColour::BLACK));
	EXPECT_FALSE(castling_rights.queen_side(PieceColour::WHITE));
	EXPECT_FALSE(castling_rights.queen_side(PieceColour::BLACK));
}

TEST(CastlingRightsTests, CanBeCreatedFromFenOnlyQueens) {
	// Arrange
	std::string fen = "Qq";
	yak::CastlingRights castling_rights(fen);

	// Assert
	EXPECT_FALSE(castling_rights.king_side(PieceColour::WHITE));
	EXPECT_FALSE(castling_rights.king_side(PieceColour::BLACK));
	EXPECT_TRUE(castling_rights.queen_side(PieceColour::WHITE));
	EXPECT_TRUE(castling_rights.queen_side(PieceColour::BLACK));
}

TEST(CastlingRightsTests, KingMoveRemovesRightToCastleForWhite) {
	// Arrange
	std::string fen = "KQkq";
	yak::CastlingRights castling_rights(fen);
	yak::piece::Move move = yak::piece::make_quiet(E1, E1);

	// Act
	castling_rights.update(move, PieceColour::WHITE);

	// Assert
	EXPECT_EQ(castling_rights.fen(), "kq");
}

TEST(CastlingRightsTests, KingMoveRemovesRightToCastleForBlack) {
	// Arrange
	std::string fen = "KQkq";
	yak::CastlingRights castling_rights(fen);
	yak::piece::Move move = yak::piece::make_quiet(E8, E1);

	// Act
	castling_rights.update(move, PieceColour::BLACK);

	// Assert
	EXPECT_EQ(castling_rights.fen(), "KQ");
}

TEST(CastlingRightsTests, H1RookMoveRemovesRightToCastleForWhite) {
	// Arrange
	std::string fen = "KQkq";
	yak::CastlingRights castling_rights(fen);
	yak::piece::Move move = yak::piece::make_quiet(yak::bitboard::square_index("h1"), yak::bitboard::square_index("e1"));

	// Act
	castling_rights.update(move, PieceColour::WHITE);

	// Assert
	EXPECT_EQ(castling_rights.fen(), "Qkq");
}

TEST(CastlingRightsTests, A8RookMoveRemovesRightToCastleForBlack) {
	// Arrange
	std::string fen = "KQkq";
	yak::CastlingRights castling_rights(fen);
	yak::piece::Move move = yak::piece::make_quiet(yak::bitboard::square_index("a8"), yak::bitboard::square_index("e1"));

	// Act
	castling_rights.update(move, PieceColour::BLACK);

	// Assert
	EXPECT_EQ(castling_rights.fen(), "KQk");
}