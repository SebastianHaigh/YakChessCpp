#include <gtest/gtest.h>

#include "../movegen.h"
#include "../pieces.h"
#include "../bitboard.h"

TEST(MoveGenTest, CanCreateObject) {
    // Assemble
    auto white_pawns_bitboard = std::make_shared<Bitboard>(bitboard::RANK_2);
    auto black_pawns_bitboard = std::make_shared<Bitboard>(bitboard::RANK_7);
    auto occupied = *white_pawns_bitboard ^ *black_pawns_bitboard;
    auto empty_squares_ptr = std::make_shared<Bitboard>(~occupied);
    auto black_piece_bitboard = std::make_shared<Bitboard> (*black_pawns_bitboard);
    auto white_piece_bitboard = std::make_shared<Bitboard> (*white_pawns_bitboard);

    auto white_pieces = std::make_shared<pieces::White>(white_piece_bitboard, black_piece_bitboard);
    auto black_pieces = std::make_shared<pieces::Black>(black_piece_bitboard, white_piece_bitboard);

    auto white_pawns = std::make_shared<pieces::Pawns>(white_pawns_bitboard, empty_squares_ptr, white_pieces);
    auto black_pawns = std::make_shared<pieces::Pawns>(black_pawns_bitboard, empty_squares_ptr, white_pieces);

    pieces::ChessMen white_side = pieces::ChessMen(white_pawns);
    pieces::ChessMen black_side = pieces::ChessMen(black_pawns);

    auto move_generator = move_generation::MoveGenerator(white_side, black_side);
    
    // Act
    auto move_stack = move_generator.generate_pseudo_legal_moves();

    // Assert
    EXPECT_EQ(move_stack.size(), 16);
}

TEST(MoveGenTest, DetectsLegalCaptures) {
    // Assemble
    auto white_pawns_bitboard = std::make_shared<Bitboard>(bitboard::RANK_2);
    auto extra_black_pawn = bitboard::RANK_3 & bitboard::FILE_C;
    auto black_pawns_bitboard = std::make_shared<Bitboard>(bitboard::RANK_7 ^ extra_black_pawn);
    auto occupied = *white_pawns_bitboard ^ *black_pawns_bitboard;
    auto empty_squares_ptr = std::make_shared<Bitboard>(~occupied);
    auto black_piece_bitboard = std::make_shared<Bitboard> (*black_pawns_bitboard);
    auto white_piece_bitboard = std::make_shared<Bitboard> (*white_pawns_bitboard);

    auto white_pieces = std::make_shared<pieces::White>(white_piece_bitboard, black_piece_bitboard);
    auto black_pieces = std::make_shared<pieces::Black>(black_piece_bitboard, white_piece_bitboard);

    auto white_pawns = std::make_shared<pieces::Pawns>(white_pawns_bitboard, empty_squares_ptr, white_pieces);
    auto black_pawns = std::make_shared<pieces::Pawns>(black_pawns_bitboard, empty_squares_ptr, black_pieces);

    pieces::ChessMen white_side = pieces::ChessMen(white_pawns);
    pieces::ChessMen black_side = pieces::ChessMen(black_pawns);

    auto move_generator = move_generation::MoveGenerator(white_side, black_side);
    
    // Act
    auto move_stack = move_generator.generate_pseudo_legal_moves();

    // Assert
    auto expected_white_pawn_positions = bitboard::RANK_2 & bitboard::NOT_FILE_B;
    expected_white_pawn_positions ^= extra_black_pawn;
    occupied = expected_white_pawn_positions ^ bitboard::RANK_7;
    EXPECT_EQ(move_stack.size(), 16);
    move_stack.top()->execute();
    EXPECT_EQ(*black_pawns_bitboard, bitboard::RANK_7);
    EXPECT_EQ(*black_piece_bitboard, bitboard::RANK_7);
    EXPECT_EQ(*white_pawns_bitboard, expected_white_pawn_positions);
    EXPECT_EQ(*white_piece_bitboard, expected_white_pawn_positions);
    EXPECT_EQ(*empty_squares_ptr, ~occupied);
    move_stack.top()->undo();
    EXPECT_EQ(*black_pawns_bitboard, bitboard::RANK_7 ^ extra_black_pawn);
    EXPECT_EQ(*black_piece_bitboard, bitboard::RANK_7 ^ extra_black_pawn);
    EXPECT_EQ(*white_pawns_bitboard, bitboard::RANK_2);
    EXPECT_EQ(*white_piece_bitboard, bitboard::RANK_2);
    occupied = *white_pawns_bitboard ^ *black_pawns_bitboard;
    EXPECT_EQ(*empty_squares_ptr, ~occupied);
}