#include <catch2/catch_test_macros.hpp>

#include <memory>
#include <vector>
#include <utility>

#include <pieces.h>
#include <bitboard.h>

TEST_CASE("BlackPawnsTest: CanGenerateSinglePushesFromUnobstructedStartingPosition") {
    // Assemble
    Bitboard pawns = yak::bitboard::RANK_7;
    yak::piece::Move move_list[20];
    int move_counter{ 0 };

    // Act
    yak::piece::generate_pawn_single_pushes<PieceColour::BLACK, false>(&move_list[move_counter], move_counter, pawns, ~pawns);

    // Assert
    CHECK(move_counter == 8);
}

TEST_CASE("BlackPawnsTest: DoublePushTargetIsRank5") {
    // Assemble
    Bitboard expected = yak::bitboard::RANK_5;

    // Act
    Bitboard actual = yak::piece::pawnDoublePushTarget<PieceColour::BLACK>();

    // Assert
    CHECK(actual == expected);
}

TEST_CASE("BlackPawnsTest: DoublePushOnlyAllowedFromRank7") {
    // Assemble
    Bitboard pawns = yak::bitboard::RANK_1;
    yak::piece::Move move_list[20];
    int move_counter{ 0 };
    int actual[8]{ 0 };

    // Act
    for (int i = 0; i < 8; i++)
    {
        move_counter = 0;
      yak::piece::generatePawnDoublePushes<PieceColour::BLACK, false>(&move_list[move_counter],
                                                                      move_counter,
                                                                      pawns,
                                                                      ~pawns);
        actual[i] = move_counter;
        pawns = yak::bitboard::shift<Direction::NORTH>(pawns);
    }


    // Assert
    CHECK(actual[0] == 0); // RANK 1, No moves
    CHECK(actual[1] == 0); // RANK 2, No moves
    CHECK(actual[2] == 0); // RANK 3, No moves
    CHECK(actual[3] == 0); // RANK 4, No moves
    CHECK(actual[4] == 0); // RANK 5, No moves
    CHECK(actual[5] == 0); // RANK 6, No moves
    CHECK(actual[6] == 8); // RANK 7, Eight moves
    CHECK(actual[7] == 0); // RANK 8, No moves
}

TEST_CASE("BlackPawnsTest: FriendlyPieceBlocksPawnPushes") {
    // Tests if an friendly piece can block pawn pushes.

    //   Test Board           Sources           Targets
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // p p p p p p p p    1 1 . 1 1 1 1 1    . . . . . . . .
    // . . b . . . . .    . . . . . . . .    1 1 . 1 1 1 1 1
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .

    // Assemble
    auto pawn_bitboard = yak::bitboard::RANK_7;
    auto extra_piece = yak::bitboard::static_bitboard<C6>::value;
    auto empty_squares = ~(pawn_bitboard ^ extra_piece);
    yak::piece::Move move_list[20];
    int move_counter{ 0 };

    // Act
    yak::piece::generate_pawn_single_pushes<PieceColour::BLACK, false>(&move_list[move_counter], move_counter, pawn_bitboard, empty_squares);

    // Assert
    CHECK(move_counter == 7);
}

TEST_CASE("BlackPawnsTest: WestAttackSources")
{
    // Arrange
    Bitboard target = yak::bitboard::static_bitboard<C6>::value;
    Bitboard expected = yak::bitboard::static_bitboard<D7>::value;

    // Act
    Bitboard actual = yak::piece::pawnWestAttackSource<PieceColour::BLACK>(target);

    // Assert
    CHECK(actual == expected);
}

TEST_CASE("BlackPawnsTest: WestAttackTarget")
{
    // Arrange
    Bitboard source = yak::bitboard::toBitboard("e6");
    Bitboard expected = yak::bitboard::toBitboard("d5");

    // Act
    Bitboard actual = yak::piece::pawnWestAttackTarget<PieceColour::BLACK>(source);

    // Assert
    CHECK(actual == expected);
}

TEST_CASE("BlackPawnsTest: EastAttackSources")
{
    // Arrange
    Bitboard target = yak::bitboard::toBitboard("c6");
    Bitboard expected = yak::bitboard::toBitboard("b7");

    // Act
    Bitboard actual = yak::piece::pawn_east_attack_source<PieceColour::BLACK>(target);

    // Assert
    CHECK(actual == expected);
}

TEST_CASE("BlackPawnsTest: EastAttackTarget")
{
    // Arrange
    Bitboard source = yak::bitboard::toBitboard("e6");
    Bitboard expected = yak::bitboard::toBitboard("f5");

    // Act
    Bitboard actual = yak::piece::pawn_east_attack_target<PieceColour::BLACK>(source);

    // Assert
    CHECK(actual == expected);
}

TEST_CASE("WhitePawnsTest: WestAttackSources")
{
    // Arrange
    Bitboard target = yak::bitboard::toBitboard("c6");
    Bitboard expected = yak::bitboard::toBitboard("d5");

    // Act
    Bitboard actual = yak::piece::pawnWestAttackSource<PieceColour::WHITE>(target);

    // Assert
    CHECK(actual == expected);
}

TEST_CASE("WhitePawnsTest: WestAttackTarget")
{
    // Arrange
    Bitboard source = yak::bitboard::toBitboard("e6");
    Bitboard expected = yak::bitboard::toBitboard("d7");

    // Act
    Bitboard actual = yak::piece::pawnWestAttackTarget<PieceColour::WHITE>(source);

    // Assert
    CHECK(actual == expected);
}

TEST_CASE("WhitePawnsTest: EastAttackSources")
{
    // Arrange
    Bitboard target = yak::bitboard::toBitboard("c6");
    Bitboard expected = yak::bitboard::toBitboard("b5");

    // Act
    Bitboard actual = yak::piece::pawn_east_attack_source<PieceColour::WHITE>(target);

    // Assert
    CHECK(actual == expected);
}

TEST_CASE("WhitePawnsTest: EastAttackTarget")
{
    // Arrange
    Bitboard source = yak::bitboard::toBitboard("e6");
    Bitboard expected = yak::bitboard::toBitboard("f7");

    // Act
    Bitboard actual = yak::piece::pawn_east_attack_target<PieceColour::WHITE>(source);

    // Assert
    CHECK(actual == expected);
}

TEST_CASE("BlackPawnsTest: OpponentPieceCanBeCaptured") {
    // Tests if the white pawns on their starting squares can return valid
    // capture moves for the situation on the board shown below.

    //   Test Board           Sources           Targets
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // p p p p p p p p    . 1 . 1 . . . .    . . . . . . . .
    // . . B . . . . .    . . . . . . . .    . . 1 . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .

    // Assemble
    Bitboard pawn_bitboard = yak::bitboard::RANK_7;
    Bitboard opponent_pieces = yak::bitboard::RANK_6 & yak::bitboard::FILE_C;

  yak::piece::pawnWestAttackSource<PieceColour::BLACK>(opponent_pieces);
    // Act
   // auto west_moves = pawns.west_captures(pawn_bitboard, opponent_pieces);
    //auto east_moves = pawns.east_captures(pawn_bitboard, opponent_pieces);

    // Assert
    //CHECK(west_moves.size() == 1);
    //CHECK(east_moves.size() == 1);
}

TEST_CASE("WhitePawnTests: CanGenerateSinglePushesFromUnobstructedStartingPosition") {
    // Assemble
    Bitboard pawns = yak::bitboard::RANK_2;
    yak::piece::Move move_list[20];
    int move_counter{ 0 };

    // Act
    yak::piece::generate_pawn_single_pushes<PieceColour::WHITE, false>(&move_list[move_counter], move_counter, pawns, ~pawns);

    // Assert

    CHECK(move_counter == 8);
}

TEST_CASE("WhitePawnTests: CanGenerateDoublePushesFromUnobstructedStartingPosition") {
    // Assemble
    Bitboard pawns = yak::bitboard::RANK_2;
    yak::piece::Move move_list[20];
    int move_counter{ 0 };

    // Act
    yak::piece::generate_pawn_single_pushes<PieceColour::WHITE, false>(&move_list[move_counter], move_counter, pawns, ~pawns);

    // Assert

    CHECK(move_counter == 8);
}

TEST_CASE("WhitePawnTests: PieceObstructsSinglePush") {
    // Tests if a piece can block pawn pushes.

    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . B . . . . .
    // P P P P P P P P
    // . . . . . . . .

    // Arrange
    Bitboard pawns = yak::bitboard::RANK_2;
    Bitboard extra_piece = yak::bitboard::static_bitboard<C3>::value;
    Bitboard empty_squares = ~(pawns ^ extra_piece);
    yak::piece::Move move_list[20];
    int move_counter{ 0 };

    // Act
    yak::piece::generate_pawn_single_pushes<PieceColour::WHITE, false>(&move_list[move_counter], move_counter, pawns, empty_squares);

    // Assert
    CHECK(move_counter == 7);

}

TEST_CASE("WhitePawnTests: PiecesObstructsDoublePush") {
    // Tests if pieces can block pawn double pushes.

    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . R . . .
    // . . B . . . . .
    // P P P P P P P P
    // . . . . . . . .

    // Arrange
    Bitboard pawns = yak::bitboard::RANK_2;
    Bitboard extra_pieces = yak::bitboard::static_bitboard<C3, E4>::value;
    Bitboard empty_squares = ~(pawns ^ extra_pieces);
    yak::piece::Move move_list[20];
    int move_counter{ 0 };

    // Act
  yak::piece::generatePawnDoublePushes<PieceColour::WHITE, false>(&move_list[move_counter],
                                                                  move_counter,
                                                                  pawns,
                                                                  empty_squares);

    // Assert
    CHECK(move_counter == 6);

}

TEST_CASE("WhitePawnTests: OpponentPieceCanBeCaptured") {
    // Tests if the white pawns on their starting squares can return valid
    // capture moves for the situation on the board shown below.

    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . b . . . . .
    // P P P P P P P P
    // . . . . . . . .

    // Assemble
    Bitboard pawns = yak::bitboard::RANK_2;
    Bitboard opponent_pieces = yak::bitboard::static_bitboard<C3>::value;
    yak::piece::Move move_list[20];
    int move_counter{ 0 };

    // Act
    yak::piece::generate_pawn_east_captures<PieceColour::WHITE, false>(&move_list[move_counter], move_counter, pawns, opponent_pieces);
  yak::piece::generatePawnWestCaptures<PieceColour::WHITE, false>(&move_list[move_counter],
                                                                  move_counter,
                                                                  pawns,
                                                                  opponent_pieces);

    // Assert
    CHECK(move_counter == 2);
}
TEST_CASE("KnightTests: KnightOnA1AttacksTheCorrectSquares") {
    // Arrange
    yak::piece::Move move_list[50];
    int move_counter{ 0 };
    Bitboard knights = yak::bitboard::static_bitboard<A1>::value;
    Bitboard empty_squares = yak::bitboard::UNIVERSAL;
    Bitboard opponent_pieces = yak::bitboard::EMPTY;

    // Act
    yak::piece::generatePieceMoves<PieceType::KNIGHT>(&move_list[0], move_counter, knights, empty_squares, opponent_pieces);

    // Assert
    CHECK(move_counter == 2);
    CHECK(move_list[0].to == C2);
    CHECK(move_list[1].to == B3);
}

TEST_CASE("KnightTests: KnightOnD4AttacksTheCorrectSquares") {
    // Arrange
    yak::piece::Move move_list[50];
    int move_counter{ 0 };
    Bitboard knights = yak::bitboard::static_bitboard<D4>::value;
    Bitboard empty_squares = yak::bitboard::UNIVERSAL;
    Bitboard opponent_pieces = yak::bitboard::EMPTY;

    // Act
    yak::piece::generatePieceMoves<PieceType::KNIGHT>(&move_list[0], move_counter, knights, empty_squares, opponent_pieces);

    // Assert
    CHECK(move_counter == 8);
    CHECK(move_list[0].to == C2);
    CHECK(move_list[1].to == E2);
    CHECK(move_list[2].to == B3);
    CHECK(move_list[3].to == F3);
    CHECK(move_list[4].to == B5);
    CHECK(move_list[5].to == F5);
    CHECK(move_list[6].to == C6);
    CHECK(move_list[7].to == E6);
}

TEST_CASE("KingTests: KnightOnA1AttacksTheCorrectSquares") {
    // Arrange
    Square starting_square{ A1 };
    yak::piece::Move move_list[50];
    int move_counter{ 0 };
    Bitboard empty_squares = yak::bitboard::UNIVERSAL;
    Bitboard opponent_pieces = yak::bitboard::EMPTY;

    // Act
    yak::piece::generatePieceMoves<PieceType::KING>(&move_list[0], move_counter,
                                                      yak::bitboard::toBitboard(starting_square), empty_squares, opponent_pieces);

    // Assert
    CHECK(move_counter == 3);
    CHECK(move_list[0].to == B1);
    CHECK(move_list[1].to == A2);
    CHECK(move_list[2].to == B2);
}

TEST_CASE("KingTests: KnightOnD4AttacksTheCorrectSquares") {
    // Arrange
    Square starting_square{ D4 };
    yak::piece::Move move_list[50];
    int move_counter{ 0 };
    Bitboard empty_squares = yak::bitboard::UNIVERSAL;
    Bitboard opponent_pieces = yak::bitboard::EMPTY;

    // Act
    yak::piece::generatePieceMoves<PieceType::KING>(&move_list[0],
                                                    move_counter,
                                                    yak::bitboard::toBitboard(starting_square),
                                                    empty_squares,
                                                    opponent_pieces);

    // Assert
    CHECK(move_counter == 8);
    CHECK(move_list[0].to == C3);
    CHECK(move_list[1].to == D3);
    CHECK(move_list[2].to == E3);
    CHECK(move_list[3].to == C4);
    CHECK(move_list[4].to == E4);
    CHECK(move_list[5].to == C5);
    CHECK(move_list[6].to == D5);
    CHECK(move_list[7].to == E5);
}

TEST_CASE("RookTests: RookAttacksProperly") {
    // Arrange
    yak::attackmap::RookMap atk_map;
    yak::piece::Move list[100];
    int move_counter{ 0 };
    Bitboard rook_bb = yak::bitboard::static_bitboard<A1>::value;
    Bitboard opponentPieces = yak::bitboard::static_bitboard<A5>::value;
    auto emptySquares = ~(rook_bb | opponentPieces);

    // Act
    yak::piece::generatePieceMoves<PieceType::ROOK>(&list[0],
                                                    move_counter,
                                                    rook_bb,
                                                    emptySquares,
                                                    opponentPieces);

    // Assert
    CHECK(move_counter == 11);
}

TEST_CASE("QueenTests: CanCalculateQueenAttackBitboard") {
    // Arrange
    yak::attackmap::RookMap atk_map;
    yak::piece::Move list[100];
    int move_counter{ 0 };
    Bitboard queen_bb = yak::bitboard::static_bitboard<A1>::value;
    Bitboard opponent_pieces = yak::bitboard::static_bitboard<A8>::value;
    Bitboard friendly_pieces = queen_bb | yak::bitboard::static_bitboard<H8>::value;
    Bitboard occupied = opponent_pieces | friendly_pieces;
    Bitboard expected = (yak::bitboard::FILE_A | yak::bitboard::RANK_1 | yak::bitboard::DIAG_A1_H8) & ~queen_bb;

    // Act
    Bitboard actual = yak::piece::pieceAttacks<PieceType::QUEEN>(queen_bb, occupied);

    // Assert
    CHECK(actual == expected);
}

TEST_CASE("BishopTests: CanCalculateBishopAttackBitboardWithTwoBishops") {
    // Arrange
    Bitboard bishop_bb = yak::bitboard::static_bitboard<A1, A8>::value;
    Bitboard opponent_pieces = yak::bitboard::static_bitboard<A8>::value;
    Bitboard friendly_pieces = bishop_bb | yak::bitboard::static_bitboard<H8>::value;
    Bitboard occupied = opponent_pieces | friendly_pieces;
    Bitboard expected = (yak::bitboard::DIAG_A8_H1 | yak::bitboard::DIAG_A1_H8) & yak::bitboard::NOT_FILE_A;

    // Act
    Bitboard actual = yak::piece::pieceAttacks<PieceType::BISHOP>(bishop_bb, occupied);

    // Assert
    CHECK(actual == expected);
}
TEST_CASE("FunctionTests: FenCharToPieceTypeTestForBlackPieces") {
    // Arrange
    std::string fen_chars{ "pnbrqkx" };
    PieceType expected[7]{ PieceType::PAWN, PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK, PieceType::QUEEN, PieceType::KING, PieceType::NULL_PIECE };
    PieceType actual[7]{ PieceType::NULL_PIECE };

    // Act
    for (int i = 0; i < 7; i++) {
        actual[i] = pieces::fenCharToPieceType(fen_chars[i]);
    }

    // Assert
    for (int i = 0; i < 7; i++) {
        CHECK(actual[i] == expected[i]);
    }
}

TEST_CASE("FunctionTests: FenCharToPieceTypeTestForWhitePieces") {
    // Arrange
    std::string fen_chars{ "PNBRQKX" };
    PieceType expected[7]{ PieceType::PAWN, PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK, PieceType::QUEEN, PieceType::KING, PieceType::NULL_PIECE };
    PieceType actual[7]{ PieceType::NULL_PIECE };

    // Act
    for (int i = 0; i < 7; i++) {
        actual[i] = pieces::fenCharToPieceType(fen_chars[i]);
    }

    // Assert
    for (int i = 0; i < 7; i++) {
        CHECK(actual[i] == expected[i]);
    }
}

TEST_CASE("FunctionTests: FenCharToPieceColourTestForBlackPieces") {
    // Arrange
    std::string fen_chars{ "pnbrqkx" };
    std::vector<PieceColour> expected(6, PieceColour::BLACK);
    expected.push_back(PieceColour::NULL_COLOUR);
    std::vector<PieceColour> actual(7, PieceColour::NULL_COLOUR);

    // Act
    for (int i = 0; i < 7; i++) {
        actual[i] = pieces::fenCharToPieceColour(fen_chars[i]);
    }

    // Assert
    for (int i = 0; i < 7; i++) {
        CHECK(actual[i] == expected[i]);
    }
}

TEST_CASE("FunctionTests: FenCharToPieceColourTestForWhitePieces") {
    // Arrange
    std::string fen_chars{ "PNBRQKX" };
    std::vector<PieceColour> expected(6, PieceColour::WHITE);
    expected.push_back(PieceColour::NULL_COLOUR);
    PieceColour actual[7]{ PieceColour::NULL_COLOUR };

    // Act
    for (int i = 0; i < 7; i++) {
        actual[i] = pieces::fenCharToPieceColour(fen_chars[i]);
    }

    // Assert
    for (int i = 0; i < 7; i++) {
        CHECK(actual[i] == expected[i]);
    }
}

