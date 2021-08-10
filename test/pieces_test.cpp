#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "../pieces.h"
#include "../bitboard.h"

TEST(WhitePawnsTest, CanGenerateValidMovesFromUnobstructedStartingPosition) {
    // Assemble
    std::shared_ptr<Bitboard> pawns_ptr = std::make_shared<Bitboard>(bitboard::RANK_2);
    std::shared_ptr<Bitboard> empty_squares_ptr = std::make_shared<Bitboard>(~*pawns_ptr);
    auto pawns = pieces::WhitePawns(pawns_ptr, empty_squares_ptr);
    Bitboard expected_sources = bitboard::RANK_2;
    Bitboard expected_targets_single_push = bitboard::RANK_3;
    Bitboard expected_targets_double_push = bitboard::RANK_4;
    
    // Act
    auto moves = pawns.get_all_moves();

    // Assert
    auto pawn_pushes = moves.top();
    moves.pop();
    // EXPECT_EQ(pawn_pushes, expected_sources);
    // EXPECT_EQ(pawn_pushes, expected_targets_double_push);
    pawn_pushes = moves.top();
    moves.pop();
    // EXPECT_EQ(pawn_pushes, expected_sources);
    // EXPECT_EQ(pawn_pushes, expected_targets_single_push);
}

TEST(WhitePawnsTest, DoublePushOnlyAllowedFromRank2) {
    // Assemble
    std::shared_ptr<Bitboard> pawns_ptr = std::make_shared<Bitboard>(bitboard::RANK_2);
    std::shared_ptr<Bitboard> empty_squares_ptr = std::make_shared<Bitboard>(bitboard::NOT_RANK_2);
    auto pawns = pieces::WhitePawns(pawns_ptr, empty_squares_ptr);

    std::vector<std::stack<pieces::SourceTargetPair>> moves;

    // Act
    for (int i = 2; i < 8; i++) {
        moves.push_back(pawns.get_all_moves());
        *pawns_ptr = bitboard::north_one(*pawns_ptr);
        *empty_squares_ptr = ~*pawns_ptr;
    }

    // Assert
    // EXPECT_EQ(moves[0].top(), bitboard::RANK_4);
    // EXPECT_EQ(moves[1].top(), 0);
    // EXPECT_EQ(moves[2].top(), 0);
    // EXPECT_EQ(moves[3].top(), 0);
    // EXPECT_EQ(moves[4].top(), 0);
    // EXPECT_EQ(moves[5].top(), 0);
}

TEST(WhitePawnsTest, ReturnsVaildCaptures) {
    // Assemble
    std::shared_ptr<Bitboard> pawns_ptr = std::make_shared<Bitboard>(bitboard::RANK_2);
    std::shared_ptr<Bitboard> empty_squares_ptr = std::make_shared<Bitboard>(bitboard::NOT_RANK_2);
    auto pawns = pieces::WhitePawns(pawns_ptr, empty_squares_ptr);

    // Act

}

TEST(BlackPawnsTest, CanGenerateValidMovesFromUnobstructedStartingPosition) {
    // Assemble
    std::shared_ptr<Bitboard> pawns_ptr = std::make_shared<Bitboard>(bitboard::RANK_7);
    std::shared_ptr<Bitboard> empty_squares_ptr = std::make_shared<Bitboard>(bitboard::NOT_RANK_7);
    auto pawns = pieces::BlackPawns(pawns_ptr, empty_squares_ptr);
    Bitboard expected_sources = bitboard::RANK_7;
    Bitboard expected_targets_single_push = bitboard::RANK_6;
    Bitboard expected_targets_double_push = bitboard::RANK_5;
    
    // Act
    auto moves = pawns.get_all_moves();

    // Assert
    auto pawn_pushes = moves.top();
    moves.pop();
    // EXPECT_EQ(pawn_pushes, expected_sources);
    // EXPECT_EQ(pawn_pushes, expected_targets_double_push);
    pawn_pushes = moves.top();
    moves.pop();
    // EXPECT_EQ(pawn_pushes, expected_sources);
    // EXPECT_EQ(pawn_pushes, expected_targets_single_push);
}

TEST(BlackPawnsTest, DoublePushOnlyAllowedFromRank7) {
    // Assemble
    std::shared_ptr<Bitboard> pawns_ptr = std::make_shared<Bitboard>(bitboard::RANK_7);
    std::shared_ptr<Bitboard> empty_squares_ptr = std::make_shared<Bitboard>(bitboard::NOT_RANK_7);
    auto pawns = pieces::BlackPawns(pawns_ptr, empty_squares_ptr);

    std::vector<std::stack<pieces::SourceTargetPair>> moves;

    // Act
    for (int i = 2; i < 8; i++) {
        moves.push_back(pawns.get_all_moves());
        *pawns_ptr = bitboard::south_one(*pawns_ptr);
        *empty_squares_ptr = ~*pawns_ptr;
    }

    // Assert
    // EXPECT_EQ(moves[0].top(), bitboard::RANK_5);
    // EXPECT_EQ(moves[1].top(), 0);
    // EXPECT_EQ(moves[2].top(), 0);
    // EXPECT_EQ(moves[3].top(), 0);
    // EXPECT_EQ(moves[4].top(), 0);
    // EXPECT_EQ(moves[5].top(), 0);
}
