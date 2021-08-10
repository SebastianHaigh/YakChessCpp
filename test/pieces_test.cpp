#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "../pieces.h"
#include "../bitboard.h"

TEST(WhitePawnsTest, CanGenerateSinglePushesFromUnobstructedStartingPosition) {
    // Assemble
    std::shared_ptr<Bitboard> pawns_ptr = std::make_shared<Bitboard>(bitboard::RANK_2);
    std::shared_ptr<Bitboard> empty_squares_ptr = std::make_shared<Bitboard>(~*pawns_ptr);
    std::shared_ptr<Bitboard> black_pieces = std::make_shared<Bitboard> (0);
    std::shared_ptr<Bitboard> white_pieces = std::make_shared<Bitboard> (*pawns_ptr);

    auto WhitePieces = std::make_shared<pieces::White>(white_pieces, black_pieces);
    auto pawns = pieces::Pawns(pawns_ptr, empty_squares_ptr, WhitePieces);
    
    // Act
    auto single_push_moves = pawns.single_pushes();

    // Assert
    EXPECT_EQ(single_push_moves.get_source(), bitboard::RANK_2);
    EXPECT_EQ(single_push_moves.get_target(), bitboard::RANK_3);
}

TEST(WhitePawnsTest, DoublePushOnlyAllowedFromRank2) {
    // Assemble
    std::shared_ptr<Bitboard> pawns_ptr = std::make_shared<Bitboard>(bitboard::RANK_2);
    std::shared_ptr<Bitboard> empty_squares_ptr = std::make_shared<Bitboard>(~*pawns_ptr);
    std::shared_ptr<Bitboard> black_pieces = std::make_shared<Bitboard> (0);
    std::shared_ptr<Bitboard> white_pieces = std::make_shared<Bitboard> (*pawns_ptr);

    auto WhitePieces = std::make_shared<pieces::White>(white_pieces, black_pieces);
    auto pawns = pieces::Pawns(pawns_ptr, empty_squares_ptr, WhitePieces);

    std::vector<pieces::PawnTargets> moves;

    // Act
    for (int i = 2; i <= 8; i++) {
        moves.push_back(pawns.double_pushes());
        *pawns_ptr = bitboard::north_one(*pawns_ptr);
        *empty_squares_ptr = ~*pawns_ptr;
    }

    // Assert
    EXPECT_EQ(moves[0].get_source(), bitboard::RANK_2);
    EXPECT_EQ(moves[1].get_source(), 0);
    EXPECT_EQ(moves[2].get_source(), 0);
    EXPECT_EQ(moves[3].get_source(), 0);
    EXPECT_EQ(moves[4].get_source(), 0);
    EXPECT_EQ(moves[5].get_source(), 0);
    EXPECT_EQ(moves[6].get_source(), 0); 
}

TEST(WhitePawnsTest, ObstructionBlocksPawnPushes) {
    // Assemble
    std::shared_ptr<Bitboard> pawns_ptr = std::make_shared<Bitboard>(bitboard::RANK_2);
    std::shared_ptr<Bitboard> black_pieces = std::make_shared<Bitboard> (bitboard::RANK_3 & bitboard::FILE_C);
    std::shared_ptr<Bitboard> white_pieces = std::make_shared<Bitboard> (*pawns_ptr);
    auto occupied_squares = *black_pieces ^ *white_pieces;
    std::shared_ptr<Bitboard> empty_squares_ptr = std::make_shared<Bitboard>(~occupied_squares);

    auto WhitePieces = std::make_shared<pieces::White>(white_pieces, black_pieces);
    auto pawns = pieces::Pawns(pawns_ptr, empty_squares_ptr, WhitePieces);

    // Act
    auto moves = pawns.single_pushes();

    // Assert
    EXPECT_EQ(moves.get_source(), bitboard::RANK_2 & bitboard::NOT_FILE_C);
    EXPECT_EQ(moves.get_target(), bitboard::RANK_3 & bitboard::NOT_FILE_C);
}

TEST(WhitePawnsTest, OpponentPieceCanBeCaptured) {
    // Assemble
    std::shared_ptr<Bitboard> pawns_ptr = std::make_shared<Bitboard>(bitboard::RANK_2);
    std::shared_ptr<Bitboard> black_pieces = std::make_shared<Bitboard> (bitboard::RANK_3 & bitboard::FILE_C);
    std::shared_ptr<Bitboard> white_pieces = std::make_shared<Bitboard> (*pawns_ptr);
    auto occupied_squares = *black_pieces ^ *white_pieces;
    std::shared_ptr<Bitboard> empty_squares_ptr = std::make_shared<Bitboard>(~occupied_squares);

    auto WhitePieces = std::make_shared<pieces::White>(white_pieces, black_pieces);
    auto pawns = pieces::Pawns(pawns_ptr, empty_squares_ptr, WhitePieces);

    // Act
    auto west_moves = pawns.west_captures();
    auto east_moves = pawns.east_captures();

    // Assert
    EXPECT_EQ(west_moves.get_source(), bitboard::RANK_2 & bitboard::FILE_D);
    EXPECT_EQ(east_moves.get_source(), bitboard::RANK_2 & bitboard::FILE_B);
    EXPECT_EQ(west_moves.get_target(), bitboard::RANK_3 & bitboard::FILE_C);
    EXPECT_EQ(east_moves.get_target(), bitboard::RANK_3 & bitboard::FILE_C);
}

TEST(WhitePawnsTest, ReturnsVaildCaptures) {
    // Assemble


    // Act

}

TEST(BlackPawnsTest, CanGenerateSinglePushesFromUnobstructedStartingPosition) {
    // Assemble
    std::shared_ptr<Bitboard> pawns_ptr = std::make_shared<Bitboard>(bitboard::RANK_7);
    std::shared_ptr<Bitboard> empty_squares_ptr = std::make_shared<Bitboard>(~*pawns_ptr);
    std::shared_ptr<Bitboard> black_pieces = std::make_shared<Bitboard> (0);
    std::shared_ptr<Bitboard> white_pieces = std::make_shared<Bitboard> (*pawns_ptr);

    auto BlackPieces = std::make_shared<pieces::Black>(black_pieces, white_pieces);
    auto pawns = pieces::Pawns(pawns_ptr, empty_squares_ptr, BlackPieces);
    
    // Act
    auto single_push_moves = pawns.single_pushes();

    // Assert

    EXPECT_EQ(single_push_moves.get_source(), bitboard::RANK_7);
    EXPECT_EQ(single_push_moves.get_target(), bitboard::RANK_6);
}

TEST(BlackPawnsTest, DoublePushOnlyAllowedFromRank7) {
    // Assemble
    std::shared_ptr<Bitboard> pawns_ptr = std::make_shared<Bitboard>(bitboard::RANK_7);
    std::shared_ptr<Bitboard> empty_squares_ptr = std::make_shared<Bitboard>(~*pawns_ptr);
    std::shared_ptr<Bitboard> black_pieces = std::make_shared<Bitboard> (0);
    std::shared_ptr<Bitboard> white_pieces = std::make_shared<Bitboard> (*pawns_ptr);

    auto BlackPieces = std::make_shared<pieces::Black>(black_pieces, white_pieces);
    auto pawns = pieces::Pawns(pawns_ptr, empty_squares_ptr, BlackPieces);

    std::vector<pieces::PawnTargets> moves;

    // Act
    for (int i = 2; i <= 8; i++) {
        moves.push_back(pawns.double_pushes());
        *pawns_ptr = bitboard::south_one(*pawns_ptr);
        *empty_squares_ptr = ~*pawns_ptr;
    }

    // Assert
    EXPECT_EQ(moves[0].get_source(), bitboard::RANK_7);
    EXPECT_EQ(moves[1].get_source(), 0);
    EXPECT_EQ(moves[2].get_source(), 0);
    EXPECT_EQ(moves[3].get_source(), 0);
    EXPECT_EQ(moves[4].get_source(), 0);
    EXPECT_EQ(moves[5].get_source(), 0);
    EXPECT_EQ(moves[6].get_source(), 0);
}
