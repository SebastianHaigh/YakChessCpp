#include <gtest/gtest.h>

#include <memory>
#include <vector>
#include <utility>

#include "../src/pieces.h"
#include "../src/bitboard.h"


TEST(BlackPawnsTest, CanGenerateSinglePushesFromUnobstructedStartingPosition) {
    // Assemble
    Bitboard pawns = bitboard::RANK_7;
    faster::Move move_list[20];
    int move_counter{ 0 };

    // Act
    faster::generate_pawn_single_pushes<PieceColour::BLACK, false>(&move_list[move_counter], move_counter, pawns, ~pawns);

    // Assert

    EXPECT_EQ(move_counter, 8);
}

TEST(BlackPawnsTest, DoublePushTargetIsRank5) {
    // Assemble
    Bitboard expected = bitboard::RANK_5;

    // Act
    Bitboard actual = faster::pawn_double_push_target<PieceColour::BLACK>();

    // Assert
    EXPECT_EQ(actual, expected);
}


TEST(BlackPawnsTest, DoublePushOnlyAllowedFromRank7) {
    // Assemble
    Bitboard pawns = bitboard::RANK_1;
    faster::Move move_list[20];
    int move_counter{ 0 };
    int actual[8]{ 0 };

    // Act
    for (int i = 0; i < 8; i++)
    {
        move_counter = 0;
        faster::generate_pawn_double_pushes<PieceColour::BLACK, false>(&move_list[move_counter], move_counter, pawns, ~pawns);
        actual[i] = move_counter;
        pawns = bitboard::shift<Direction::NORTH>(pawns);
    }
    

    // Assert
    EXPECT_EQ(actual[0], 0); // RANK 1, No moves
    EXPECT_EQ(actual[1], 0); // RANK 2, No moves
    EXPECT_EQ(actual[2], 0); // RANK 3, No moves
    EXPECT_EQ(actual[3], 0); // RANK 4, No moves
    EXPECT_EQ(actual[4], 0); // RANK 5, No moves
    EXPECT_EQ(actual[5], 0); // RANK 6, No moves
    EXPECT_EQ(actual[6], 8); // RANK 7, Eight moves
    EXPECT_EQ(actual[7], 0); // RANK 8, No moves
}

TEST(BlackPawnsTest, FriendlyPieceBlocksPawnPushes) {
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
    auto pawn_bitboard = bitboard::RANK_7;
    auto extra_piece = bitboard::static_bitboard<C6>::value;
    auto empty_squares = ~(pawn_bitboard ^ extra_piece);
    faster::Move move_list[20];
    int move_counter{ 0 };

    // Act
    faster::generate_pawn_single_pushes<PieceColour::BLACK, false>(&move_list[move_counter], move_counter, pawn_bitboard, empty_squares);

    // Assert
    EXPECT_EQ(move_counter, 7);
}

TEST(BlackPawnsTest, WestAttackSources)
{
    // Arrange
    Bitboard target = bitboard::static_bitboard<C6>::value;
    Bitboard expected = bitboard::static_bitboard<D7>::value;

    // Act
    Bitboard actual = faster::pawn_west_attack_source<PieceColour::BLACK>(target);

    // Assert
    EXPECT_EQ(actual, expected);
}

TEST(BlackPawnsTest, WestAttackTarget)
{
    // Arrange
    Bitboard source = bitboard::to_bitboard("e6");
    Bitboard expected = bitboard::to_bitboard("d5");

    // Act
    Bitboard actual = faster::pawn_west_attack_target<PieceColour::BLACK>(source);

    // Assert
    EXPECT_EQ(actual, expected);
}

TEST(BlackPawnsTest, EastAttackSources)
{
    // Arrange
    Bitboard target = bitboard::to_bitboard("c6");
    Bitboard expected = bitboard::to_bitboard("b7");

    // Act
    Bitboard actual = faster::pawn_east_attack_source<PieceColour::BLACK>(target);

    // Assert
    EXPECT_EQ(actual, expected);
}

TEST(BlackPawnsTest, EastAttackTarget)
{
    // Arrange
    Bitboard source = bitboard::to_bitboard("e6");
    Bitboard expected = bitboard::to_bitboard("f5");

    // Act
    Bitboard actual = faster::pawn_east_attack_target<PieceColour::BLACK>(source);

    // Assert
    EXPECT_EQ(actual, expected);
}

TEST(WhitePawnsTest, WestAttackSources)
{
    // Arrange
    Bitboard target = bitboard::to_bitboard("c6");
    Bitboard expected = bitboard::to_bitboard("d5");

    // Act
    Bitboard actual = faster::pawn_west_attack_source<PieceColour::WHITE>(target);

    // Assert
    EXPECT_EQ(actual, expected);
}

TEST(WhitePawnsTest, WestAttackTarget)
{
    // Arrange
    Bitboard source = bitboard::to_bitboard("e6");
    Bitboard expected = bitboard::to_bitboard("d7");

    // Act
    Bitboard actual = faster::pawn_west_attack_target<PieceColour::WHITE>(source);

    // Assert
    EXPECT_EQ(actual, expected);
}

TEST(WhitePawnsTest, EastAttackSources)
{
    // Arrange
    Bitboard target = bitboard::to_bitboard("c6");
    Bitboard expected = bitboard::to_bitboard("b5");

    // Act
    Bitboard actual = faster::pawn_east_attack_source<PieceColour::WHITE>(target);

    // Assert
    EXPECT_EQ(actual, expected);
}

TEST(WhitePawnsTest, EastAttackTarget)
{
    // Arrange
    Bitboard source = bitboard::to_bitboard("e6");
    Bitboard expected = bitboard::to_bitboard("f7");

    // Act
    Bitboard actual = faster::pawn_east_attack_target<PieceColour::WHITE>(source);

    // Assert
    EXPECT_EQ(actual, expected);
}

TEST(BlackPawnsTest, OpponentPieceCanBeCaptured) {
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
    Bitboard pawn_bitboard = bitboard::RANK_7;
    Bitboard opponent_pieces = bitboard::RANK_6 & bitboard::FILE_C;

    faster::pawn_west_attack_source<PieceColour::BLACK>(opponent_pieces);
    // Act
   // auto west_moves = pawns.west_captures(pawn_bitboard, opponent_pieces);
    //auto east_moves = pawns.east_captures(pawn_bitboard, opponent_pieces);

    // Assert
    //EXPECT_EQ(west_moves.size(), 1);
    //EXPECT_EQ(east_moves.size(), 1);
}

TEST(WhitePawnTests, CanGenerateSinglePushesFromUnobstructedStartingPosition) {
    // Assemble
    Bitboard pawns = bitboard::RANK_2;
    faster::Move move_list[20];
    int move_counter{ 0 };
    
    // Act
    faster::generate_pawn_single_pushes<PieceColour::WHITE, false>(&move_list[move_counter], move_counter, pawns, ~pawns);

    // Assert

    EXPECT_EQ(move_counter, 8);
}

TEST(WhitePawnTests, CanGenerateDoublePushesFromUnobstructedStartingPosition) {
    // Assemble
    Bitboard pawns = bitboard::RANK_2;
    faster::Move move_list[20];
    int move_counter{ 0 };

    // Act
    faster::generate_pawn_single_pushes<PieceColour::WHITE, false>(&move_list[move_counter], move_counter, pawns, ~pawns);

    // Assert

    EXPECT_EQ(move_counter, 8);
}

TEST(WhitePawnTests, PieceObstructsSinglePush) {
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
    Bitboard pawns = bitboard::RANK_2;
    Bitboard extra_piece = bitboard::static_bitboard<C3>::value;
    Bitboard empty_squares = ~(pawns ^ extra_piece);
    faster::Move move_list[20];
    int move_counter{ 0 };

    // Act
    faster::generate_pawn_single_pushes<PieceColour::WHITE, false>(&move_list[move_counter], move_counter, pawns, empty_squares);

    // Assert
    EXPECT_EQ(move_counter, 7);

}

TEST(WhitePawnTests, PiecesObstructsDoublePush) {
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
    Bitboard pawns = bitboard::RANK_2;
    Bitboard extra_pieces = bitboard::static_bitboard<C3, E4>::value;
    Bitboard empty_squares = ~(pawns ^ extra_pieces);
    faster::Move move_list[20];
    int move_counter{ 0 };

    // Act
    faster::generate_pawn_double_pushes<PieceColour::WHITE, false>(&move_list[move_counter], move_counter, pawns, empty_squares);

    // Assert
    EXPECT_EQ(move_counter, 6);

}

TEST(WhitePawnTests, OpponentPieceCanBeCaptured) {
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
    Bitboard pawns = bitboard::RANK_2;
    Bitboard opponent_pieces = bitboard::static_bitboard<C3>::value;
    faster::Move move_list[20];
    int move_counter{ 0 };

    // Act
    faster::generate_pawn_east_captures<PieceColour::WHITE, false>(&move_list[move_counter], move_counter, pawns, opponent_pieces);
    faster::generate_pawn_west_captures<PieceColour::WHITE, false>(&move_list[move_counter], move_counter, pawns, opponent_pieces);

    // Assert
    EXPECT_EQ(move_counter, 2);
}

TEST(KnightTests, KnightOnA1AttacksTheCorrectSquares) {
    // Arrange
    attacks::KnightAttacks knight_attacks;
    Square starting_square{0};
    faster::Move move_list[50];
    int move_counter{ 0 };
    Bitboard empty_squares = bitboard::UNIVERSAL;
    Bitboard opponent_pieces = bitboard::EMPTY;

    // Act
    faster::jumping_piece_moves(&knight_attacks, &move_list[0], move_counter, bitboard::to_bitboard(starting_square), empty_squares, opponent_pieces);
  
    // Assert
    EXPECT_EQ(move_counter, 2);
    EXPECT_EQ(move_list[0].to, bitboard::square_index("c2"));
    EXPECT_EQ(move_list[1].to, bitboard::square_index("b3"));
}

TEST(KnightTests, KnightOnD4AttacksTheCorrectSquares) {
    // Arrange
    attacks::KnightAttacks knight_attacks;
    Square starting_square = bitboard::square_index("d4");
    faster::Move move_list[50];
    int move_counter{ 0 };
    Bitboard empty_squares = bitboard::UNIVERSAL;
    Bitboard opponent_pieces = bitboard::EMPTY;

    // Act
    faster::jumping_piece_moves(&knight_attacks, &move_list[0], move_counter, bitboard::to_bitboard(starting_square), empty_squares, opponent_pieces);

    // Assert
    EXPECT_EQ(move_counter, 8);
    EXPECT_EQ(move_list[0].to, bitboard::square_index("c2"));
    EXPECT_EQ(move_list[1].to, bitboard::square_index("e2"));
    EXPECT_EQ(move_list[2].to, bitboard::square_index("b3"));
    EXPECT_EQ(move_list[3].to, bitboard::square_index("f3"));
    EXPECT_EQ(move_list[4].to, bitboard::square_index("b5"));
    EXPECT_EQ(move_list[5].to, bitboard::square_index("f5"));
    EXPECT_EQ(move_list[6].to, bitboard::square_index("c6"));
    EXPECT_EQ(move_list[7].to, bitboard::square_index("e6"));
}



TEST(KingTests, KnightOnA1AttacksTheCorrectSquares) {
    // Arrange
    attacks::KingAttacks king_attacks;
    Square starting_square{ 0 };
    faster::Move move_list[50];
    int move_counter{ 0 };
    Bitboard empty_squares = bitboard::UNIVERSAL;
    Bitboard opponent_pieces = bitboard::EMPTY;

    // Act
    faster::jumping_piece_moves(&king_attacks, &move_list[0], move_counter, bitboard::to_bitboard(starting_square), empty_squares, opponent_pieces);

    // Assert
    EXPECT_EQ(move_counter, 3);
    EXPECT_EQ(move_list[0].to, bitboard::square_index("b1"));
    EXPECT_EQ(move_list[1].to, bitboard::square_index("a2"));
    EXPECT_EQ(move_list[2].to, bitboard::square_index("b2"));
}

TEST(KingTests, KnightOnD4AttacksTheCorrectSquares) {
    // Arrange
    attacks::KingAttacks king_attacks;
    Square starting_square = bitboard::square_index("d4");
    faster::Move move_list[50];
    int move_counter{ 0 };
    Bitboard empty_squares = bitboard::UNIVERSAL;
    Bitboard opponent_pieces = bitboard::EMPTY;

    // Act
    faster::jumping_piece_moves(&king_attacks, &move_list[0], move_counter, bitboard::to_bitboard(starting_square), empty_squares, opponent_pieces);

    // Assert
    EXPECT_EQ(move_counter, 8);
    EXPECT_EQ(move_list[0].to, bitboard::square_index("c3"));
    EXPECT_EQ(move_list[1].to, bitboard::square_index("d3"));
    EXPECT_EQ(move_list[2].to, bitboard::square_index("e3"));
    EXPECT_EQ(move_list[3].to, bitboard::square_index("c4"));
    EXPECT_EQ(move_list[4].to, bitboard::square_index("e4"));
    EXPECT_EQ(move_list[5].to, bitboard::square_index("c5"));
    EXPECT_EQ(move_list[6].to, bitboard::square_index("d5"));
    EXPECT_EQ(move_list[7].to, bitboard::square_index("e5"));
}

TEST(RookTests, RookAttacksProperly) {
    // Arrange
    faster::RookMap atk_map;
    faster::Move list[100];
    int move_counter{ 0 };
    Bitboard rook_bb = Bitboard{ 1 };
    Bitboard obstruction = bitboard::to_bitboard(bitboard::square_index("a5"));

    // Act
    faster::generate_sliding_piece_moves(atk_map, &list[0], move_counter, rook_bb, ~obstruction, obstruction);

    // Assert
    EXPECT_EQ(move_counter, 11);
}

TEST(QueenTests, CanCalculateQueenAttackBitboard) {
    // Arrange
    faster::RookMap atk_map;
    faster::Move list[100];
    int move_counter{ 0 };
    Bitboard queen_bb = Bitboard{ 1 };
    Bitboard opponent_pieces = bitboard::to_bitboard(bitboard::square_index("a8"));
    Bitboard friendly_pieces = queen_bb | bitboard::to_bitboard(bitboard::square_index("h8"));
    Bitboard occupied = opponent_pieces | friendly_pieces;
    Bitboard expected = (bitboard::FILE_A | bitboard::RANK_1 | bitboard::DIAG_A1_H8) & ~queen_bb;

    // Act
    Bitboard actual = faster::piece_attacks<PieceType::QUEEN>(queen_bb, occupied);

    // Assert
    EXPECT_EQ(actual, expected);
}

TEST(BishopTests, CanCalculateBishopAttackBitboardWithTwoBishops) {
    // Arrange
    Bitboard bishop_bb = bitboard::to_bitboard("a1") | bitboard::to_bitboard("a8");
    Bitboard opponent_pieces = bitboard::to_bitboard("a8");
    Bitboard friendly_pieces = bishop_bb | bitboard::to_bitboard("h8");
    Bitboard occupied = opponent_pieces | friendly_pieces;
    Bitboard expected = (bitboard::DIAG_A8_H1 | bitboard::DIAG_A1_H8) & bitboard::NOT_FILE_A;

    // Act
    Bitboard actual = faster::piece_attacks<PieceType::BISHOP>(bishop_bb, occupied);

    // Assert
    EXPECT_EQ(actual, expected);
}



TEST(FunctionTests, FenCharToPieceTypeTestForBlackPieces) {
    // Arrange
    std::string fen_chars{ "pnbrqkx" };
    PieceType expected[7]{ PieceType::PAWN, PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK, PieceType::QUEEN, PieceType::KING, PieceType::NULL_PIECE };
    PieceType actual[7]{ PieceType::NULL_PIECE };

    // Act
    for (int i = 0; i < 7; i++) {
        actual[i] = pieces::fen_char_to_piece_type(fen_chars[i]);
    }

    // Assert
    for (int i = 0; i < 7; i++) {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST(FunctionTests, FenCharToPieceTypeTestForWhitePieces) {
    // Arrange
    std::string fen_chars{ "PNBRQKX" };
    PieceType expected[7]{ PieceType::PAWN, PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK, PieceType::QUEEN, PieceType::KING, PieceType::NULL_PIECE };
    PieceType actual[7]{ PieceType::NULL_PIECE };

    // Act
    for (int i = 0; i < 7; i++) {
        actual[i] = pieces::fen_char_to_piece_type(fen_chars[i]);
    }

    // Assert
    for (int i = 0; i < 7; i++) {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST(FunctionTests, FenCharToPieceColourTestForBlackPieces) {
    // Arrange
    std::string fen_chars{ "pnbrqkx" };
    std::vector<PieceColour> expected(6, PieceColour::BLACK);
    expected.push_back(PieceColour::NULL_COLOUR);
    std::vector<PieceColour> actual(7, PieceColour::NULL_COLOUR);

    // Act
    for (int i = 0; i < 7; i++) {
        actual[i] = pieces::fen_char_to_piece_colour(fen_chars[i]);
    }

    // Assert
    for (int i = 0; i < 7; i++) {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

TEST(FunctionTests, FenCharToPieceColourTestForWhitePieces) {
    // Arrange
    std::string fen_chars{ "PNBRQKX" }; 
    std::vector<PieceColour> expected(6, PieceColour::WHITE);
    expected.push_back(PieceColour::NULL_COLOUR);
    PieceColour actual[7]{ PieceColour::NULL_COLOUR };

    // Act
    for (int i = 0; i < 7; i++) {
        actual[i] = pieces::fen_char_to_piece_colour(fen_chars[i]);
    }

    // Assert
    for (int i = 0; i < 7; i++) {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

