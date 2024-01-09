#include <catch2/catch_test_macros.hpp>
#include <board.h>
#include <bitboard.h>
#include <pieces.h>

#include <string>

TEST_CASE("MoveTest: DoublePawnPushCreatesEpTargetSquare") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  std::string expected{ "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3 0 1" };
  yak::Board board(fen);
  auto move = yak::piece::makeDoublePush(8, 24);

  // Act
  board.makeMove(move);

  // Assert
  CHECK(board.toFen() == expected);
}

TEST_CASE("MoveTest: CanMakeEpCapture") {
  // Arrange
  std::string fen{ "8/8/8/8/Pp6/1P6/8/8 b KQkq a3 0 1" };
  std::string expected{ "8/8/8/8/8/pP6/8/8 w KQkq - 0 2" };
  yak::Board board(fen);
  auto move = yak::piece::make_ep_capture(25, 16);
  //move.set_en_passant(yak::bitboard::to_yak::bitboard(Square(24)));

  // Act
  board.makeMove(move);


  // Assert
  CHECK(board.toFen() == expected);
}

/*
TEST_CASE("MoveTest: CanConvertPawnMoveToAlgebraic") {
	// Arrange
	Move move(Square(25), Square(16), PieceType::PAWN);
	std::string expected = "a3";

	// Act
	std::string actual = move.to_algebraic();

	// Assert
	CHECK(actual == expected);
}
*/
TEST_CASE("MoveFactoryTest: CanCreatePawnPushMove") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  std::string expected{ "rnbqkbnr/pppppppp/8/8/8/P7/1PPPPPPP/RNBQKBNR b KQkq - 0 1" };
  yak::Board board(fen);

  // Act
  auto pawn_push = yak::piece::makeQuiet(8, 16);
  //Move pawn_push = mf.create_pawn_push(8, 16);

  // Assert
  board.makeMove(pawn_push);
  CHECK(board.toFen() == expected);
}

TEST_CASE("MoveFactoryTest: CanCreatePawnDoublePushMove") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  std::string expected{ "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3 0 1" };
  yak::Board board(fen);

  // Act
  yak::piece::Move pawn_push = yak::piece::makeDoublePush(8, 24);

  // Assert
  board.makeMove(pawn_push);
  CHECK(board.toFen() == expected);
}
TEST_CASE("BoardTest: BoardCanReturnCorrectPawnPositions") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
  auto expected = yak::bitboard::RANK_2 | yak::bitboard::RANK_7;

  // Act
  Bitboard actual = board.get_position(PieceType::PAWN);

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("BoardTest: BoardCanReturnCorrectKnightPositions") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
  Bitboard expected = (yak::bitboard::RANK_1 | yak::bitboard::RANK_8) & (yak::bitboard::FILE_B | yak::bitboard::FILE_G);

  // Act
  Bitboard actual = board.get_position(PieceType::KNIGHT);

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("BoardTest: BoardCanReturnCorrectBishopPositions") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
  Bitboard expected = (yak::bitboard::RANK_1 | yak::bitboard::RANK_8) & (yak::bitboard::FILE_C | yak::bitboard::FILE_F);

  // Act
  Bitboard actual = board.get_position(PieceType::BISHOP);

  // Assert
  CHECK(actual == expected);
}


TEST_CASE("BoardTest: BoardCanReturnCorrectRookPositions") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
  Bitboard expected = (yak::bitboard::RANK_1 | yak::bitboard::RANK_8) & (yak::bitboard::FILE_A | yak::bitboard::FILE_H);

  // Act
  Bitboard actual = board.get_position(PieceType::ROOK);

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("BoardTest: BoardCanReturnCorrectQueenPositions") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
  Bitboard expected = (yak::bitboard::RANK_1 | yak::bitboard::RANK_8) & yak::bitboard::FILE_D;

  // Act
  Bitboard actual = board.get_position(PieceType::QUEEN);

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("BoardTest: BoardCanReturnCorrectKingPositions") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
  Bitboard expected = (yak::bitboard::RANK_1 | yak::bitboard::RANK_8) & yak::bitboard::FILE_E;

  // Act
  Bitboard actual = board.get_position(PieceType::KING);

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("BoardTest: BoardCanReturnCorrectWhitePieces") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
  Bitboard expected = (yak::bitboard::RANK_1 | yak::bitboard::RANK_2);

  // Act
  Bitboard actual = board.get_position(PieceColour::WHITE);

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("BoardTest: BoardCanReturnCorrectBlackPieces") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
  Bitboard expected = (yak::bitboard::RANK_7 | yak::bitboard::RANK_8);

  // Act
  Bitboard actual = board.get_position(PieceColour::BLACK);

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("BoardTest: BoardCanReturnCorrectWhitePawnPositions") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
  Bitboard expected = yak::bitboard::RANK_2;

  // Act
  Bitboard actual = board.getPosition(PieceColour::WHITE, PieceType::PAWN);

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("BoardTest: BoardCanReturnCorrectBlackPawnPositions") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
  Bitboard expected = yak::bitboard::RANK_7;

  // Act
  Bitboard actual = board.getPosition(PieceColour::BLACK, PieceType::PAWN);

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("BoardTest: BoardCanReturnCorrectWhiteKnightPositions") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
  Bitboard expected = yak::bitboard::RANK_1 & (yak::bitboard::FILE_B | yak::bitboard::FILE_G);

  // Act
  Bitboard actual = board.getPosition(PieceColour::WHITE, PieceType::KNIGHT);

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("BoardTest: BoardCanReturnCorrectBlackKnightPositions") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
  Bitboard expected = yak::bitboard::RANK_8 & (yak::bitboard::FILE_B | yak::bitboard::FILE_G);

  // Act
  Bitboard actual = board.getPosition(PieceColour::BLACK, PieceType::KNIGHT);

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("BoardTest: BoardCanReturnCorrectWhiteBishopPositions") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
  Bitboard expected = yak::bitboard::RANK_1 & (yak::bitboard::FILE_C | yak::bitboard::FILE_F);

  // Act
  Bitboard actual = board.getPosition(PieceColour::WHITE, PieceType::BISHOP);

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("BoardTest: BoardCanReturnCorrectBlackBishopPositions") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
  Bitboard expected = yak::bitboard::RANK_8 & (yak::bitboard::FILE_C | yak::bitboard::FILE_F);

  // Act
  Bitboard actual = board.getPosition(PieceColour::BLACK, PieceType::BISHOP);

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("BoardTest: BoardCanReturnCorrectWhiteRookPositions") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
  Bitboard expected = yak::bitboard::RANK_1 & (yak::bitboard::FILE_A | yak::bitboard::FILE_H);

  // Act
  Bitboard actual = board.getPosition(PieceColour::WHITE, PieceType::ROOK);

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("BoardTest: BoardCanReturnCorrectBlackRookPositions") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
  Bitboard expected = yak::bitboard::RANK_8 & (yak::bitboard::FILE_A | yak::bitboard::FILE_H);

  // Act
  Bitboard actual = board.getPosition(PieceColour::BLACK, PieceType::ROOK);

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("BoardTest: BoardCanReturnCorrectWhiteQueenPositions") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
  Bitboard expected = yak::bitboard::RANK_1 & yak::bitboard::FILE_D;

  // Act
  Bitboard actual = board.getPosition(PieceColour::WHITE, PieceType::QUEEN);

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("BoardTest: BoardCanReturnCorrectBlackQueenPositions") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
  Bitboard expected = yak::bitboard::RANK_8 & yak::bitboard::FILE_D;

  // Act
  Bitboard actual = board.getPosition(PieceColour::BLACK, PieceType::QUEEN);

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("BoardTest: BoardCanReturnCorrectWhiteKingPositions") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
  Bitboard expected = yak::bitboard::RANK_1 & yak::bitboard::FILE_E;

  // Act
  Bitboard actual = board.getPosition(PieceColour::WHITE, PieceType::KING);

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("BoardTest: BoardCanReturnCorrectBlackKingPositions") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
  Bitboard expected = yak::bitboard::RANK_8 & yak::bitboard::FILE_E;

  // Act
  Bitboard actual = board.getPosition(PieceColour::BLACK, PieceType::KING);

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("BoardTest: CanReturnEmptySquares") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
  Bitboard expected = yak::bitboard::RANK_3 | yak::bitboard::RANK_4 | yak::bitboard::RANK_5 | yak::bitboard::RANK_6;

  // Act
  Bitboard actual = board.emptySquares();

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("BoardTest: EpSquareLoads") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3 0 1" };
  yak::Board board(fen);

  // Act
  std::string actual = board.toFen();

  // Assert
  CHECK(actual == fen);
}

TEST_CASE("BoardTest: DetectsCastlingRightsFromFenAll") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3 0 1" };
  yak::Board board(fen);

  // Assert
  CHECK(board.canKingSideCastle(PieceColour::BLACK));
  CHECK(board.canKingSideCastle(PieceColour::WHITE));
  CHECK(board.canQueenSideCastle(PieceColour::BLACK));
  CHECK(board.canQueenSideCastle(PieceColour::WHITE));
}

TEST_CASE("BoardTest: DetectsCastlingRightsFromFenNoKingSideWhite") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b Qkq a3 0 1" };
  yak::Board board(fen);

  // Assert
  CHECK(board.canKingSideCastle(PieceColour::BLACK));
  CHECK_FALSE(board.canKingSideCastle(PieceColour::WHITE));
  CHECK(board.canQueenSideCastle(PieceColour::BLACK));
  CHECK(board.canQueenSideCastle(PieceColour::WHITE));
}

TEST_CASE("BoardTest: CastlingRightsChangeWhenRookIsCaptured") {
  // Arrange
  std::string fen{ "rnbqkbnr/ppppppp1/8/8/8/8/PPPPPPP1/RNBQKBNR b KQkq - 0 1" };
  std::string expected{ "rnbqkbn1/ppppppp1/8/8/8/8/PPPPPPP1/RNBQKBNr w Qq - 0 2" };
  yak::Board board(fen);

  yak::piece::Move move = yak::piece::makeCapture(63, 7);
  //move.set_capture(PieceType::ROOK);

  board.makeMove(move);

  std::string actual = board.toFen();

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("BoardTest: UndoingMoveRestoresState") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);
  yak::piece::Move move = yak::piece::makeQuiet(yak::bitboard::squareIndex("e2"), yak::bitboard::squareIndex("e4"));
  board.makeMove(move);

  //std::cout << board.toFen() << std::endl;

  // Act
  board.undoMove();

  // Assert
  CHECK(board.toFen() == fen);
}

TEST_CASE("BoardTest: CalculatesAllAttackedSquares") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);

  // Act
  Bitboard white_actual = board.attacked_by(PieceColour::WHITE);
  Bitboard black_actual = board.attacked_by(PieceColour::BLACK);

  // Assert
  CHECK(white_actual == yak::bitboard::RANK_3);
  CHECK(black_actual == yak::bitboard::RANK_6);
}

TEST_CASE("BoardTest: CalculatesAllAttackedSquares2") {
  // Arrange
  std::string fen{ "r1bqkbnr/pppp1ppp/2n5/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1" };
  yak::Board board(fen);

  // Pawns
  Bitboard white_expected = yak::bitboard::toBitboard("a3");
  white_expected |= yak::bitboard::toBitboard("b3");
  white_expected |= yak::bitboard::toBitboard("c3");
  white_expected |= yak::bitboard::toBitboard("d3");
  white_expected |= yak::bitboard::toBitboard("e3");
  white_expected |= yak::bitboard::toBitboard("g3");
  white_expected |= yak::bitboard::toBitboard("h3");
  white_expected |= yak::bitboard::toBitboard("f5");

  // Knights
  white_expected |= yak::bitboard::toBitboard("d4");
  white_expected |= yak::bitboard::toBitboard("e5");
  white_expected |= yak::bitboard::toBitboard("g5");
  white_expected |= yak::bitboard::toBitboard("h4");
  white_expected |= yak::bitboard::toBitboard("g1");

  // Bishop
  white_expected |= yak::bitboard::toBitboard("a6");
  white_expected |= yak::bitboard::toBitboard("b5");
  white_expected |= yak::bitboard::toBitboard("d3");
  white_expected |= yak::bitboard::toBitboard("e2");
  white_expected |= yak::bitboard::toBitboard("f1");
  white_expected |= yak::bitboard::toBitboard("b3");
  white_expected |= yak::bitboard::toBitboard("d5");
  white_expected |= yak::bitboard::toBitboard("e6");
  white_expected |= yak::bitboard::toBitboard("f7");

  // King
  white_expected |= yak::bitboard::toBitboard("e2") | yak::bitboard::toBitboard("f1");

  // Act
  Bitboard white_actual = board.attacked_by(PieceColour::WHITE);
  Bitboard black_actual = board.attacked_by(PieceColour::BLACK);
  yak::bitboard::print_board(white_actual);
  yak::bitboard::print_board(white_expected);
  // Assert
  CHECK(white_actual == white_expected);
}

TEST_CASE("BoardTest: CanDetectCheck") {
  // Arrange
  std::string fen{ "rnbqkbnr/ppp2ppp/3p4/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1" };
  yak::Board board(fen);

  // Act
  bool actual = board.isCheck();

  // Assert
  CHECK(actual);
}

TEST_CASE("BoardTest: CanDetectNotCheck") {
  // Arrange
  std::string fen{ "rnbqkbnr/pp3ppp/2pp4/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1" };
  yak::Board board(fen);

  // Act
  bool actual = board.isCheck();

  // Assert
  CHECK_FALSE(actual);
}

TEST_CASE("BoardMoveGenerationTests: DetectsLegalCapturesWithWhitePawns") {

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
  std::vector<yak::piece::Move> moves = board.generateMoves();

  // Assert
  CHECK(moves.size() == 3);
}

TEST_CASE("BoardMoveGenerationTests: DetectsLegalCapturesWithBlackPawns") {

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
  std::vector<yak::piece::Move> moves = board.generateMoves();

  // Assert
  CHECK(moves.size() == 3);
}

TEST_CASE("BoardMoveGenerationTests: Kiwipete") {
  // Arrange
  std::string fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";
  yak::Board board(fen);

  // Act
  std::vector<yak::piece::Move> moves = board.generateMoves();

  // Assert
  CHECK(moves.size() == 48);
}
/*
TEST_CASE("BoardMoveGenerationTests: KiwipeteFast") {
	// Arrange
	std::string fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";
	Board board = Board(fen);
	yak::piece::Move move_list[330];
	yak::piece::SpecialisedRay<Direction::NORTH> n_ray;
	yak::piece::SpecialisedRay<Direction::EAST> e_ray;
	yak::piece::SpecialisedRay<Direction::SOUTH> s_ray;
	yak::piece::SpecialisedRay<Direction::WEST> w_ray;
	std::vector<yak::piece::Ray*> m_rookAtks{&n_ray, & e_ray, & s_ray, & w_ray};
	int move_counter{ 0 };

	// Act
	yak::piece::generate_pawn_moves(PieceColour::WHITE, &move_list[0], move_counter,
		board.get_position(PieceColour::WHITE, PieceType::PAWN),
		board.empty_squares(), board.get_position(PieceColour::BLACK));
	yak::piece::generate_sliding_piece_moves(m_rookAtks, &move_list[0], move_counter,
		board.get_position(PieceColour::WHITE, PieceType::ROOK),
		board.emptySquares(), board.get_position(PieceColour::BLACK));

	// Assert
	CHECK(move_counter == 13);
}
*/
TEST_CASE("BoardMoveGenerationTests: CanGenerateAndMakeEpCapture") {

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

  std::vector<yak::piece::Move> moves = board.generateMoves();

  // Act
  for (auto move : moves) {
    if (move.en_passant) {
      board.makeMove(move);
      break;
    }

  }

  // Assert
  CHECK(board.toFen() == expected);
}

TEST_CASE("BoardMoveGenerationTests: CanCastleKingSideWhiteOnlyKingAndRook") {
  // Arrange
  std::string fen{ "8/8/8/8/8/8/8/4K2R w K - 0 1" };
  std::string expected{ "8/8/8/8/8/8/8/5RK1 b - - 0 1" };
  yak::Board board(fen);

  std::vector<yak::piece::Move> moves = board.generateMoves();

  // Act
  for (auto move : moves) {
    if (move.castle != PieceType::NULL_PIECE) {
      board.makeMove(move);
      break;
    }

  }

  // Assert
  CHECK(board.toFen() == expected);
}


TEST_CASE("BoardMoveGenerationTests: CanCastleKingSideWhite") {
  // Arrange
  std::string fen{ "rnbqkbnr/ppp2ppp/3p4/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1" };
  std::string expected{ "rnbqkbnr/ppp2ppp/3p4/4p3/2B1P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 0 1" };
  yak::Board board(fen);

  std::vector<yak::piece::Move> moves = board.generateMoves();

  // Act
  for (auto move : moves) {
    if (move.castle != PieceType::NULL_PIECE) {
      board.makeMove(move);
      break;
    }

  }

  // Assert
  CHECK(board.toFen() == expected);
}

TEST_CASE("BoardMoveGenerationTests: CanCastleKingSideBlack") {
  // Arrange
  std::string fen{ "rnbqk2r/ppp2pbp/3p1np1/3Pp3/2B1P3/5N2/PPP2PPP/RNBQ1RK1 b kq - 0 1" };
  std::string expected{ "rnbq1rk1/ppp2pbp/3p1np1/3Pp3/2B1P3/5N2/PPP2PPP/RNBQ1RK1 w - - 0 2" };
  yak::Board board = yak::Board(fen);

  std::vector<yak::piece::Move> moves = board.generateMoves();

  // Act
  for (auto move : moves) {
    if (move.castle != PieceType::NULL_PIECE) {
      board.makeMove(move);
      break;
    }

  }

  // Assert
  CHECK(board.toFen() == expected);
}

TEST_CASE("BoardMoveGenerationTests: CanCastleQueenSideWhite") {
  // Arrange
  std::string fen{ "rnbqk2r/ppp2pbp/3p1np1/3Pp3/2B1P3/2N1B3/PPPQ1PPP/R3K1NR w KQkq - 0 1" };
  std::string expected{ "rnbqk2r/ppp2pbp/3p1np1/3Pp3/2B1P3/2N1B3/PPPQ1PPP/2KR2NR b kq - 0 1" };
  yak::Board board = yak::Board(fen);

  std::vector<yak::piece::Move> moves = board.generateMoves();

  // Act
  for (auto move : moves) {
    if (move.castle != PieceType::NULL_PIECE) {
      board.makeMove(move);
      break;
    }

  }

  // Assert
  CHECK(board.toFen() == expected);
}

TEST_CASE("BoardMoveGenerationTests: CanCastleQueenSideBlack") {
  // Arrange
  std::string fen{ "r3kbnr/ppp2ppp/3pb3/4p1q1/4Pn2/3P4/PPP2PPP/RNBQKBNR b KQkq - 0 1" };
  std::string expected{ "2kr1bnr/ppp2ppp/3pb3/4p1q1/4Pn2/3P4/PPP2PPP/RNBQKBNR w KQ - 0 2" };
  yak::Board board(fen);

  std::vector<yak::piece::Move> moves = board.generateMoves();

  // Act
  for (auto& move : moves) {
    if (move.castle != PieceType::NULL_PIECE) {
      board.makeMove(move);
      break;
    }

  }

  // Assert
  CHECK(board.toFen() == expected);
}

TEST_CASE("BoardMoveGenerationTests: PawnCanPromote") {
  // Arrange
  std::string fen{ "8/P7/8/8/8/8/8/8 w - - 0 1" };
  yak::Board board(fen);

  std::vector<yak::piece::Move> moves = board.generateMoves();

  // Act & Assert

  board.makeMove(moves[0]);
  CHECK(board.toFen() == "N7/8/8/8/8/8/8/8 b - - 0 1");
  board.undoMove();

  board.makeMove(moves[1]);
  CHECK(board.toFen() == "B7/8/8/8/8/8/8/8 b - - 0 1");
  board.undoMove();

  board.makeMove(moves[2]);
  CHECK(board.toFen() == "R7/8/8/8/8/8/8/8 b - - 0 1");
  board.undoMove();

  board.makeMove(moves[3]);
  CHECK(board.toFen() == "Q7/8/8/8/8/8/8/8 b - - 0 1");
  board.undoMove();
}

TEST_CASE("MoveTest: FirstMoveOfTheGameHas20LegalMoves") {
  // Arrange
  std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
  yak::Board board(fen);

  // Act
  std::vector<yak::piece::Move> moves = board.generateMoves();

  // Assert
  CHECK(moves.size() == 20);
}
TEST_CASE("BoardMoveGenerationTests: DetectsAllMovesInPosition") {

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
  std::vector<yak::piece::Move> moves = board.generateMoves();

  // Assert
  CHECK(moves.size() == 27);

}

TEST_CASE("BoardMoveGenerationTests: DetectsAllMovesInPosition2") {

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
  std::vector<yak::piece::Move> moves = board.generateMoves();

  // Assert
  CHECK(moves.size() == 36);

}

TEST_CASE("BoardMoveGenerationTests: DetectsAllMovesInPosition3") {

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
  std::vector<yak::piece::Move> moves = board.generateMoves();

  // Assert
  CHECK(moves.size() == 41);

}

TEST_CASE("BoardMoveGenerationTests: DetectsLegalMovesWhenInCheck") {

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
  std::vector<yak::piece::Move> moves = board.generateMoves();

  for (auto move : moves)
    std::cout << move.to_algebraic() << std::endl;

  // Assert
  CHECK(moves.size() == 6); // The only legal moves are the six that remove the check: c6, Nc6, Nd7, Bd7, Qc7, Ke7

}

TEST_CASE("BoardMoveGenerationTests: DetectsASituationThatIsNOTCheckmate") {

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
  bool actual = board.isCheckmate();

  // Assert
  CHECK_FALSE(actual);

}

TEST_CASE("BoardMoveGenerationTests: DetectsASituationThatIsCheckmate") {

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
  bool actual = board.isCheckmate();

  // Assert
  CHECK(actual);

}

TEST_CASE("BoardMoveGenerationTests: CannotCastleOutOfCheck") {

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

  std::vector<yak::piece::Move> moves = board.generateMoves();

  // Assert
  for (auto& move : moves) {
    CHECK(move.castle == PieceType::NULL_PIECE);
  }


}
TEST_CASE("CastlingRightsTests: CanBeCreatedFromFenAll") {
  // Arrange
  std::string fen = "KQkq";
  yak::CastlingRights castlingRights(fen);

  // Assert
  CHECK(castlingRights.kingSide(PieceColour::WHITE));
  CHECK(castlingRights.kingSide(PieceColour::BLACK));
  CHECK(castlingRights.queenSide(PieceColour::WHITE));
  CHECK(castlingRights.queenSide(PieceColour::BLACK));
}

TEST_CASE("CastlingRightsTests: CanBeCreatedFromFenNone") {
  // Arrange
  std::string fen = "-";
  yak::CastlingRights castlingRights(fen);

  // Assert
  CHECK_FALSE(castlingRights.kingSide(PieceColour::WHITE));
  CHECK_FALSE(castlingRights.kingSide(PieceColour::BLACK));
  CHECK_FALSE(castlingRights.queenSide(PieceColour::WHITE));
  CHECK_FALSE(castlingRights.queenSide(PieceColour::BLACK));
}

TEST_CASE("CastlingRightsTests: CanBeCreatedFromFenOnlyWhite") {
  // Arrange
  std::string fen = "KQ";
  yak::CastlingRights castlingRights(fen);

  // Assert
  CHECK(castlingRights.kingSide(PieceColour::WHITE));
  CHECK_FALSE(castlingRights.kingSide(PieceColour::BLACK));
  CHECK(castlingRights.queenSide(PieceColour::WHITE));
  CHECK_FALSE(castlingRights.queenSide(PieceColour::BLACK));
}

TEST_CASE("CastlingRightsTests: CanBeCreatedFromFenOnlyBlack") {
  // Arrange
  std::string fen = "kq";
  yak::CastlingRights castlingRights(fen);

  // Assert
  CHECK_FALSE(castlingRights.kingSide(PieceColour::WHITE));
  CHECK(castlingRights.kingSide(PieceColour::BLACK));
  CHECK_FALSE(castlingRights.queenSide(PieceColour::WHITE));
  CHECK(castlingRights.queenSide(PieceColour::BLACK));
}

TEST_CASE("CastlingRightsTests: CanBeCreatedFromFenOnlyKings") {
  // Arrange
  std::string fen = "Kk";
  yak::CastlingRights castlingRights(fen);

  // Assert
  CHECK(castlingRights.kingSide(PieceColour::WHITE));
  CHECK(castlingRights.kingSide(PieceColour::BLACK));
  CHECK_FALSE(castlingRights.queenSide(PieceColour::WHITE));
  CHECK_FALSE(castlingRights.queenSide(PieceColour::BLACK));
}

TEST_CASE("CastlingRightsTests: CanBeCreatedFromFenOnlyQueens") {
  // Arrange
  std::string fen = "Qq";
  yak::CastlingRights castlingRights(fen);

  // Assert
  CHECK_FALSE(castlingRights.kingSide(PieceColour::WHITE));
  CHECK_FALSE(castlingRights.kingSide(PieceColour::BLACK));
  CHECK(castlingRights.queenSide(PieceColour::WHITE));
  CHECK(castlingRights.queenSide(PieceColour::BLACK));
}

TEST_CASE("CastlingRightsTests: KingMoveRemovesRightToCastleForWhite") {
  // Arrange
  std::string fen = "KQkq";
  yak::CastlingRights castlingRights(fen);
  yak::piece::Move move = yak::piece::makeQuiet(E1, E1);

  // Act
  castlingRights.update(move, PieceColour::WHITE);

  // Assert
  CHECK(castlingRights.fen() == "kq");
}

TEST_CASE("CastlingRightsTests: KingMoveRemovesRightToCastleForBlack") {
  // Arrange
  std::string fen = "KQkq";
  yak::CastlingRights castling_rights(fen);
  yak::piece::Move move = yak::piece::makeQuiet(E8, E1);

  // Act
  castling_rights.update(move, PieceColour::BLACK);

  // Assert
  CHECK(castling_rights.fen() == "KQ");
}

TEST_CASE("CastlingRightsTests: H1RookMoveRemovesRightToCastleForWhite") {
  // Arrange
  std::string fen = "KQkq";
  yak::CastlingRights castling_rights(fen);
  yak::piece::Move move = yak::piece::makeQuiet(yak::bitboard::squareIndex("h1"), yak::bitboard::squareIndex("e1"));

  // Act
  castling_rights.update(move, PieceColour::WHITE);

  // Assert
  CHECK(castling_rights.fen() == "Qkq");
}

TEST_CASE("CastlingRightsTests: A8RookMoveRemovesRightToCastleForBlack") {
  // Arrange
  std::string fen = "KQkq";
  yak::CastlingRights castling_rights(fen);
  yak::piece::Move move = yak::piece::makeQuiet(yak::bitboard::squareIndex("a8"), yak::bitboard::squareIndex("e1"));

  // Act
  castling_rights.update(move, PieceColour::BLACK);

  // Assert
  CHECK(castling_rights.fen() == "KQk");
}
