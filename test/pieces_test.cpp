#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "../pieces.h"
#include "../bitboard.h"

TEST(WhitePawnsTest, CanGenerateValidMovesFromUnobstructedStartingPosition) {
    // Assemble
    std::shared_ptr<Bitboard> pawns_ptr = std::make_shared<Bitboard>(bitboard::RANK_2);
    std::shared_ptr<Bitboard> empty_squares_ptr = std::make_shared<Bitboard>(~*pawns_ptr);
    std::shared_ptr<Bitboard> black_pieces = std::make_shared<Bitboard> (0);
    std::shared_ptr<Bitboard> white_pieces = std::make_shared<Bitboard> (*pawns_ptr);

    auto WhitePieces = std::make_shared<pieces::White>(white_pieces, black_pieces);
    auto pawns = pieces::Pawns(pawns_ptr, empty_squares_ptr, WhitePieces);
    Bitboard expected_sources = bitboard::RANK_2;
    Bitboard expected_targets_single_push = bitboard::RANK_3;
    Bitboard expected_targets_double_push = bitboard::RANK_4;
    
    // Act
    auto moves = pawns.all_quiet_moves();

    // Assert
}

TEST(WhitePawnsTest, DoublePushOnlyAllowedFromRank2) {
    // Assemble
    std::shared_ptr<Bitboard> pawns_ptr = std::make_shared<Bitboard>(bitboard::RANK_2);
    std::shared_ptr<Bitboard> empty_squares_ptr = std::make_shared<Bitboard>(~*pawns_ptr);
    std::shared_ptr<Bitboard> black_pieces = std::make_shared<Bitboard> (0);
    std::shared_ptr<Bitboard> white_pieces = std::make_shared<Bitboard> (*pawns_ptr);

    auto WhitePieces = std::make_shared<pieces::White>(white_pieces, black_pieces);
    auto pawns = pieces::Pawns(pawns_ptr, empty_squares_ptr, WhitePieces);

    std::vector<std::stack<pieces::PawnTargets>> moves;

    // Act
    for (int i = 2; i <= 8; i++) {
        moves.push_back(pawns.all_quiet_moves());
        *pawns_ptr = bitboard::north_one(*pawns_ptr);
        *empty_squares_ptr = ~*pawns_ptr;
    }

    // Assert
    EXPECT_EQ(moves[0].size(), 16); // Double and single push from rank 2
    EXPECT_EQ(moves[1].size(), 8); // Single push from rank 3
    EXPECT_EQ(moves[2].size(), 8); // Single push from rank 4
    EXPECT_EQ(moves[3].size(), 8); // Single push from rank 5
    EXPECT_EQ(moves[4].size(), 8); // Single push from rank 6
    EXPECT_EQ(moves[5].size(), 8); // Single push from rank 7
    EXPECT_EQ(moves[6].size(), 0); // Nowhere to go from rank 8
}

TEST(WhitePawnsTest, ReturnsVaildCaptures) {
    // Assemble


    // Act

}

TEST(BlackPawnsTest, CanGenerateValidMovesFromUnobstructedStartingPosition) {
    // Assemble
    std::shared_ptr<Bitboard> pawns_ptr = std::make_shared<Bitboard>(bitboard::RANK_7);
    std::shared_ptr<Bitboard> empty_squares_ptr = std::make_shared<Bitboard>(~*pawns_ptr);
    std::shared_ptr<Bitboard> black_pieces = std::make_shared<Bitboard> (0);
    std::shared_ptr<Bitboard> white_pieces = std::make_shared<Bitboard> (*pawns_ptr);

    auto BlackPieces = std::make_shared<pieces::Black>(black_pieces, white_pieces);
    auto pawns = pieces::Pawns(pawns_ptr, empty_squares_ptr, BlackPieces);
    
    // Act
    auto moves = pawns.all_quiet_moves();

    // Assert
    EXPECT_EQ(moves.size(), 16);
}

TEST(BlackPawnsTest, DoublePushOnlyAllowedFromRank7) {
    // Assemble
    std::shared_ptr<Bitboard> pawns_ptr = std::make_shared<Bitboard>(bitboard::RANK_7);
    std::shared_ptr<Bitboard> empty_squares_ptr = std::make_shared<Bitboard>(~*pawns_ptr);
    std::shared_ptr<Bitboard> black_pieces = std::make_shared<Bitboard> (0);
    std::shared_ptr<Bitboard> white_pieces = std::make_shared<Bitboard> (*pawns_ptr);

    auto BlackPieces = std::make_shared<pieces::Black>(black_pieces, white_pieces);
    auto pawns = pieces::Pawns(pawns_ptr, empty_squares_ptr, BlackPieces);

    std::vector<std::stack<pieces::PawnTargets>> moves;

    // Act
    for (int i = 2; i <= 8; i++) {
        moves.push_back(pawns.all_quiet_moves());
        *pawns_ptr = bitboard::south_one(*pawns_ptr);
        *empty_squares_ptr = ~*pawns_ptr;
    }

    // Assert
    EXPECT_EQ(moves[0].size(), 16); // Double and single push from rank 7
    EXPECT_EQ(moves[1].size(), 8); // Single push from rank 6
    EXPECT_EQ(moves[2].size(), 8); // Single push from rank 5
    EXPECT_EQ(moves[3].size(), 8); // Single push from rank 4
    EXPECT_EQ(moves[4].size(), 8); // Single push from rank 3
    EXPECT_EQ(moves[5].size(), 8); // Single push from rank 2
    EXPECT_EQ(moves[6].size(), 0); // Nowhere to go from rank 1
}
