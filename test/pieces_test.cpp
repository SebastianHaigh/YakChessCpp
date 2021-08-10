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
    
    // Act
    auto moves = pawns.all_quiet_moves();

    // Assert    
    auto double_push_moves = moves.top();
    moves.pop();
    auto single_push_moves = moves.top();

    EXPECT_EQ(single_push_moves.get_source(), bitboard::RANK_2);
    EXPECT_EQ(single_push_moves.get_target(), bitboard::RANK_3);
    EXPECT_EQ(double_push_moves.get_source(), bitboard::RANK_2);
    EXPECT_EQ(double_push_moves.get_target(), bitboard::RANK_4);

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
    EXPECT_EQ(moves[0].top().get_source(), bitboard::RANK_2);
    EXPECT_EQ(moves[1].top().get_source(), 0);
    EXPECT_EQ(moves[2].top().get_source(), 0);
    EXPECT_EQ(moves[3].top().get_source(), 0);
    EXPECT_EQ(moves[4].top().get_source(), 0);
    EXPECT_EQ(moves[5].top().get_source(), 0);
    EXPECT_EQ(moves[6].top().get_source(), 0); 
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
    auto double_push_moves = moves.top();
    moves.pop();
    auto single_push_moves = moves.top();

    EXPECT_EQ(single_push_moves.get_source(), bitboard::RANK_7);
    EXPECT_EQ(single_push_moves.get_target(), bitboard::RANK_6);
    EXPECT_EQ(double_push_moves.get_source(), bitboard::RANK_7);
    EXPECT_EQ(double_push_moves.get_target(), bitboard::RANK_5);
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
    EXPECT_EQ(moves[0].top().get_source(), bitboard::RANK_7);
    EXPECT_EQ(moves[1].top().get_source(), 0);
    EXPECT_EQ(moves[2].top().get_source(), 0);
    EXPECT_EQ(moves[3].top().get_source(), 0);
    EXPECT_EQ(moves[4].top().get_source(), 0);
    EXPECT_EQ(moves[5].top().get_source(), 0);
    EXPECT_EQ(moves[6].top().get_source(), 0);
}
