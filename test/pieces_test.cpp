#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "../pieces.h"
#include "../bitboard.h"

TEST(WhitePawnsTest, CanGenerateSinglePushesFromUnobstructedStartingPosition) {
    // Assemble
    auto pawns_bitboard = bitboard::RANK_2;
    auto empty_squares_ptr = std::make_shared<Bitboard>(~pawns_bitboard);
    auto black_pieces = std::make_shared<Bitboard> (0);
    auto white_pieces = std::make_shared<Bitboard> (pawns_bitboard);

    auto WhitePieces = std::make_shared<pieces::White>(white_pieces, black_pieces);
    auto pawns = pieces::Pawns(empty_squares_ptr, WhitePieces);
    pawns.standard_starting_position();

    // Act
    auto single_push_moves = pawns.single_pushes();

    // Assert
    EXPECT_EQ(single_push_moves.get_source(), bitboard::RANK_2);
    EXPECT_EQ(single_push_moves.get_target(), bitboard::RANK_3);
}

TEST(WhitePawnsTest, DoublePushOnlyAllowedFromRank2) {
    // Assemble
    auto pawns_bitboard = bitboard::RANK_2;
    auto empty_squares_ptr = std::make_shared<Bitboard>(~pawns_bitboard);
    auto black_pieces = std::make_shared<Bitboard> (0);
    auto white_pieces = std::make_shared<Bitboard> (pawns_bitboard);

    auto WhitePieces = std::make_shared<pieces::White>(white_pieces, black_pieces);
    auto pawns = pieces::Pawns(empty_squares_ptr, WhitePieces);
    pawns.standard_starting_position();

    std::vector<pieces::PawnTargets> moves;

    // Act
    for (int i = 2; i <= 8; i++) {
        moves.push_back(pawns.double_pushes());
        pawns_bitboard = bitboard::north_one(pawns_bitboard);
    }

    // Assert
    EXPECT_EQ(moves[0].get_source(), bitboard::RANK_2);
    EXPECT_EQ(moves[1].get_source(), bitboard::RANK_2);
    EXPECT_EQ(moves[2].get_source(), bitboard::RANK_2);
    EXPECT_EQ(moves[3].get_source(), bitboard::RANK_2);
    EXPECT_EQ(moves[4].get_source(), bitboard::RANK_2);
    EXPECT_EQ(moves[5].get_source(), bitboard::RANK_2);
    EXPECT_EQ(moves[6].get_source(), bitboard::RANK_2); 
}

TEST(WhitePawnsTest, OpponentPieceBlocksPawnPushes) {
    // Tests if an opponent piece can block pawn pushes

    // Assemble
    auto pawns_bitboard = bitboard::RANK_2;
    auto black_pieces = std::make_shared<Bitboard> (bitboard::RANK_3 & bitboard::FILE_C);
    auto white_pieces = std::make_shared<Bitboard> (pawns_bitboard);
    auto occupied_squares = *black_pieces ^ *white_pieces;
    auto empty_squares_ptr = std::make_shared<Bitboard>(~occupied_squares);

    auto WhitePieces = std::make_shared<pieces::White>(white_pieces, black_pieces);
    auto pawns = pieces::Pawns(empty_squares_ptr, WhitePieces);
    pawns.standard_starting_position();
    // Act
    auto moves = pawns.single_pushes();

    // Assert
    EXPECT_EQ(moves.get_source(), bitboard::RANK_2 & bitboard::NOT_FILE_C);
    EXPECT_EQ(moves.get_target(), bitboard::RANK_3 & bitboard::NOT_FILE_C);
}

TEST(WhitePawnsTest, FriendlyPieceBlocksPawnPushes) {
    // Tests if an friendly piece can block pawn pushes.

    // . . . . . . . . 
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . B . . . . .
    // P P P P P P P P
    // . . . . . . . .

    // Assemble
    auto pawns_bitboard = bitboard::RANK_2;
    auto black_pieces = std::make_shared<Bitboard>(0);
    auto extra_white_piece = bitboard::RANK_3 & bitboard::FILE_C;
    auto white_pieces = std::make_shared<Bitboard> (pawns_bitboard ^ extra_white_piece);
    auto occupied_squares = *black_pieces ^ *white_pieces;
    auto empty_squares_ptr = std::make_shared<Bitboard>(~occupied_squares);

    auto WhitePieces = std::make_shared<pieces::White>(white_pieces, black_pieces);
    auto pawns = pieces::Pawns(empty_squares_ptr, WhitePieces);
    pawns.standard_starting_position();

    // Act
    auto moves = pawns.single_pushes();

    // Assert
    EXPECT_EQ(moves.get_source(), bitboard::RANK_2 & bitboard::NOT_FILE_C);
    EXPECT_EQ(moves.get_target(), bitboard::RANK_3 & bitboard::NOT_FILE_C);
}

TEST(WhitePawnsTest, OpponentPieceCanBeCaptured) {
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
    auto pawn_bitboard = bitboard::RANK_2;
    auto black_pieces = std::make_shared<Bitboard> (bitboard::RANK_3 & bitboard::FILE_C);
    auto white_pieces = std::make_shared<Bitboard> (pawn_bitboard);
    auto occupied_squares = *black_pieces ^ *white_pieces;
    auto empty_squares_ptr = std::make_shared<Bitboard>(~occupied_squares);

    auto WhitePieces = std::make_shared<pieces::White>(white_pieces, black_pieces);
    auto pawns = pieces::Pawns(empty_squares_ptr, WhitePieces);
    pawns.standard_starting_position();

    // Act
    auto west_moves = pawns.west_captures();
    auto east_moves = pawns.east_captures();

    // Assert
    EXPECT_EQ(west_moves.get_source(), bitboard::RANK_2 & bitboard::FILE_D);
    EXPECT_EQ(east_moves.get_source(), bitboard::RANK_2 & bitboard::FILE_B);
    EXPECT_EQ(west_moves.get_target(), bitboard::RANK_3 & bitboard::FILE_C);
    EXPECT_EQ(east_moves.get_target(), bitboard::RANK_3 & bitboard::FILE_C);
}

TEST(BlackPawnsTest, CanGenerateSinglePushesFromUnobstructedStartingPosition) {
    // Assemble
    auto pawn_bitboard = std::make_shared<Bitboard>(bitboard::RANK_7);
    auto empty_squares_ptr = std::make_shared<Bitboard>(~*pawn_bitboard);
    auto black_pieces = std::make_shared<Bitboard> (0);
    auto white_pieces = std::make_shared<Bitboard> (*pawn_bitboard);

    auto BlackPieces = std::make_shared<pieces::Black>(black_pieces, white_pieces);
    auto pawns = pieces::Pawns(empty_squares_ptr, BlackPieces);
    pawns.standard_starting_position();

    // Act
    auto single_push_moves = pawns.single_pushes();

    // Assert

    EXPECT_EQ(single_push_moves.get_source(), bitboard::RANK_7);
    EXPECT_EQ(single_push_moves.get_target(), bitboard::RANK_6);
}

TEST(BlackPawnsTest, DoublePushOnlyAllowedFromRank7) {
    // Assemble
    auto pawn_bitboard = std::make_shared<Bitboard>(bitboard::RANK_7);
    auto empty_squares_ptr = std::make_shared<Bitboard>(~*pawn_bitboard);
    auto black_pieces = std::make_shared<Bitboard>(0);
    auto white_pieces = std::make_shared<Bitboard>(*pawn_bitboard);

    auto BlackPieces = std::make_shared<pieces::Black>(black_pieces, white_pieces);
    auto pawns = pieces::Pawns(empty_squares_ptr, BlackPieces);
    pawns.standard_starting_position();

    std::vector<pieces::PawnTargets> moves;

    // Act
    for (int i = 2; i <= 8; i++) {
        moves.push_back(pawns.double_pushes());
    }

    // Assert
    EXPECT_EQ(moves[0].get_source(), bitboard::RANK_7);
    EXPECT_EQ(moves[1].get_source(), bitboard::RANK_7);
    EXPECT_EQ(moves[2].get_source(), bitboard::RANK_7);
    EXPECT_EQ(moves[3].get_source(), bitboard::RANK_7);
    EXPECT_EQ(moves[4].get_source(), bitboard::RANK_7);
    EXPECT_EQ(moves[5].get_source(), bitboard::RANK_7);
    EXPECT_EQ(moves[6].get_source(), bitboard::RANK_7);
}

TEST(BlackPawnsTest, OpponentPieceBlocksPawnPushes) {
    // Tests if an opponent piece can block pawn pushes

    //   Test Board           Sources           Targets
    // . . . . . . . .    . . . . . . . .    . . . . . . . . 
    // p p p p p p p p    1 1 . 1 1 1 1 1    . . . . . . . . 
    // . . B . . . . .    . . . . . . . .    1 1 . 1 1 1 1 1 
    // . . . . . . . .    . . . . . . . .    . . . . . . . . 
    // . . . . . . . .    . . . . . . . .    . . . . . . . . 
    // . . . . . . . .    . . . . . . . .    . . . . . . . . 
    // . . . . . . . .    . . . . . . . .    . . . . . . . . 
    // . . . . . . . .    . . . . . . . .    . . . . . . . . 

    // Assemble
    auto pawn_bitboard = std::make_shared<Bitboard>(bitboard::RANK_7);
    auto white_pieces = std::make_shared<Bitboard> (bitboard::RANK_6 & bitboard::FILE_C);
    auto black_pieces = std::make_shared<Bitboard> (*pawn_bitboard);
    auto occupied_squares = *black_pieces ^ *white_pieces;
    auto empty_squares_ptr = std::make_shared<Bitboard>(~occupied_squares);

    auto BlackPieces = std::make_shared<pieces::Black>(black_pieces, white_pieces);
    auto pawns = pieces::Pawns(empty_squares_ptr, BlackPieces);
    pawns.standard_starting_position();

    // Act
    auto moves = pawns.single_pushes();

    // Assert
    EXPECT_EQ(moves.get_source(), bitboard::RANK_7 & bitboard::NOT_FILE_C);
    EXPECT_EQ(moves.get_target(), bitboard::RANK_6 & bitboard::NOT_FILE_C);
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
    auto pawn_bitboard = std::make_shared<Bitboard>(bitboard::RANK_7);
    auto white_pieces = std::make_shared<Bitboard>(0);
    auto extra_black_piece = bitboard::RANK_6 & bitboard::FILE_C;
    auto black_pieces = std::make_shared<Bitboard> (*pawn_bitboard ^ extra_black_piece);
    auto occupied_squares = *black_pieces ^ *white_pieces;
    auto empty_squares_ptr = std::make_shared<Bitboard>(~occupied_squares);

    auto BlackPieces = std::make_shared<pieces::Black>(black_pieces, white_pieces);
    auto pawns = pieces::Pawns(empty_squares_ptr, BlackPieces);
    pawns.standard_starting_position();
    // Act
    auto moves = pawns.single_pushes();

    // Assert
    EXPECT_EQ(moves.get_source(), bitboard::RANK_7 & bitboard::NOT_FILE_C);
    EXPECT_EQ(moves.get_target(), bitboard::RANK_6 & bitboard::NOT_FILE_C);
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
    auto pawn_bitboard = std::make_shared<Bitboard>(bitboard::RANK_7);
    auto white_pieces = std::make_shared<Bitboard> (bitboard::RANK_6 & bitboard::FILE_C);
    auto black_pieces = std::make_shared<Bitboard> (*pawn_bitboard);
    auto occupied_squares = *black_pieces ^ *white_pieces;
    auto empty_squares_ptr = std::make_shared<Bitboard>(~occupied_squares);

    auto BlackPieces = std::make_shared<pieces::Black>(black_pieces, white_pieces);
    auto pawns = pieces::Pawns(empty_squares_ptr, BlackPieces);
    pawns.standard_starting_position();

    // Act
    auto west_moves = pawns.west_captures();
    auto east_moves = pawns.east_captures();

    // Assert
    EXPECT_EQ(west_moves.get_source(), bitboard::RANK_7 & bitboard::FILE_D);
    EXPECT_EQ(east_moves.get_source(), bitboard::RANK_7 & bitboard::FILE_B);
    EXPECT_EQ(west_moves.get_target(), bitboard::RANK_6 & bitboard::FILE_C);
    EXPECT_EQ(east_moves.get_target(), bitboard::RANK_6 & bitboard::FILE_C);
}