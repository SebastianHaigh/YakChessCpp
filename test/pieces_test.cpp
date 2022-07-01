#include <gtest/gtest.h>

#include <memory>
#include <vector>
#include <utility>

#include "../src/pieces.h"
#include "../src/bitboard.h"


TEST(BlackPawnsTest, CanGenerateSinglePushesFromUnobstructedStartingPosition) {
    // Assemble
    Bitboard pawn_bitboard = bitboard::RANK_7;
    Bitboard empty_squares = ~pawn_bitboard;

    // Act

    // Assert

    //EXPECT_EQ(single_push_moves.size(), 8);
}

TEST(BlackPawnsTest, DoublePushOnlyAllowedFromRank7) {
    // Assemble
    auto pawn_bitboard = bitboard::RANK_7;
    auto empty_squares = ~pawn_bitboard;
    auto pawns = pieces::BlackPawns();


    // Act
    auto moves = pawns.double_push(pawn_bitboard, empty_squares);


    // Assert
    EXPECT_EQ(bitboard::rank_index(moves[0].first), 6); // If the source square has rank index of 6 it means that square is on the 7th rank.
    EXPECT_EQ(bitboard::rank_index(moves[1].first), 6);
    EXPECT_EQ(bitboard::rank_index(moves[2].first), 6);
    EXPECT_EQ(bitboard::rank_index(moves[3].first), 6);
    EXPECT_EQ(bitboard::rank_index(moves[4].first), 6);
    EXPECT_EQ(bitboard::rank_index(moves[5].first), 6);
    EXPECT_EQ(bitboard::rank_index(moves[6].first), 6);
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
    auto extra_piece = bitboard::RANK_6 & bitboard::FILE_C;
    auto empty_squares = ~(pawn_bitboard ^ extra_piece);
    auto pawns = pieces::BlackPawns();

    // Act
    auto moves = pawns.single_push(pawn_bitboard, empty_squares);

    // Assert
    EXPECT_EQ(moves.size(), 7);
}

TEST(BlackPawnsTest, WestAttackSources)
{
    // Arrange
    Bitboard target = bitboard::to_bitboard("c6");
    Bitboard expected = bitboard::to_bitboard("d7");

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
    pieces::WhitePawns pawns = pieces::WhitePawns();
    Bitboard pawn_bitboard = bitboard::RANK_2;
    Bitboard empty_squares = ~pawn_bitboard;

    // Act
    auto single_push_moves = pawns.single_push(pawn_bitboard, empty_squares);

    // Assert
    EXPECT_EQ(single_push_moves.size(), 8);
}

TEST(WhitePawnTests, CanGenerateDoublePushesFromUnobstructedStartingPosition) {
    // Assemble
    pieces::WhitePawns pawns = pieces::WhitePawns();
    Bitboard pawn_bitboard = bitboard::RANK_2;
    Bitboard empty_squares = ~pawn_bitboard;

    // Act
    auto single_push_moves = pawns.double_push(pawn_bitboard, empty_squares);

    // Assert
    EXPECT_EQ(single_push_moves.size(), 8);
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
    auto pawns = pieces::WhitePawns();
    auto pawn_bitboard = bitboard::RANK_2;
    auto extra_piece = bitboard::RANK_3 & bitboard::FILE_C;
    auto empty_squares = ~(pawn_bitboard ^ extra_piece);

    // Act
    auto moves = pawns.single_push(pawn_bitboard, empty_squares);

    // Assert
    EXPECT_EQ(moves.size(), 7);

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
    auto pawns = pieces::WhitePawns();
    auto pawn_bitboard = bitboard::RANK_2;
    auto extra_piece_1 = bitboard::RANK_3 & bitboard::FILE_C;
    auto extra_piece_2 = bitboard::RANK_4 & bitboard::FILE_E;
    auto empty_squares = ~(pawn_bitboard ^ extra_piece_1 ^ extra_piece_2);

    // Act
    auto moves = pawns.double_push(pawn_bitboard, empty_squares);

    // Assert
    EXPECT_EQ(moves.size(), 6);

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
    pieces::WhitePawns pawns = pieces::WhitePawns();
    Bitboard pawn_bitboard = bitboard::RANK_2;
    Bitboard opponent_pieces = bitboard::RANK_3 & bitboard::FILE_C;
    

    // Act
    auto west_moves = pawns.west_captures(pawn_bitboard, opponent_pieces);
    auto east_moves = pawns.east_captures(pawn_bitboard, opponent_pieces);

    // Assert
    EXPECT_EQ(west_moves.size(), 1);
    EXPECT_EQ(east_moves.size(), 1);
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

