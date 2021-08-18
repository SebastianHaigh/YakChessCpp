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

    auto white_pawns = std::make_shared<pieces::Pawns>(empty_squares_ptr, white_pieces);
    auto black_pawns = std::make_shared<pieces::Pawns>(empty_squares_ptr, white_pieces);
    white_pawns->standard_starting_position();
    black_pawns->standard_starting_position();

    pieces::ChessMen white_side = pieces::ChessMen(white_pawns);
    pieces::ChessMen black_side = pieces::ChessMen(black_pawns);

    auto move_generator = move_generation::MoveGenerator(white_side, black_side);
    
    // Act
    auto move_stack = move_generator.generate_pseudo_legal_moves();

    // Assert
    EXPECT_EQ(move_stack.size(), 16);
}

TEST(MoveGenTest, DetectsLegalCaptures) {
    
    //   Test Board       W Pawn Targets           Targets
    // . . . . . . . .    . . . . . . . .    . . . . . . . . 
    // . . . . . . . .    . . . . . . . .    . . . . . . . . 
    // . . . . . . . .    . . . . . . . .    1 1 . 1 1 1 1 1 
    // . . . . . . . .    . . . . . . . .    . . . . . . . . 
    // . . . . . . . .    . 1 . . . . . .    . . . . . . . . 
    // . . p . . . . .    . 1 1 . . . . .    . . . . . . . . 
    // . P . . . . . .    . . . . . . . .    . . . . . . . . 
    // . . . . . . . .    . . . . . . . .    . . . . . . . . 


    // Assemble
    auto white_pawns_bitboard = bitboard::RANK_2 & bitboard::FILE_B;
    auto black_pawns_bitboard = bitboard::RANK_3 & bitboard::FILE_C;

    auto occupied = white_pawns_bitboard ^ black_pawns_bitboard;
    auto empty_squares_ptr = std::make_shared<Bitboard>(~occupied);
    auto black_piece_bitboard = std::make_shared<Bitboard> (black_pawns_bitboard);
    auto white_piece_bitboard = std::make_shared<Bitboard> (white_pawns_bitboard);

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
    EXPECT_EQ(move_stack.size(), 3);
    move_stack.top()->execute();
    EXPECT_EQ(black_pawns->current_position(), 0);
    EXPECT_EQ(white_pawns->current_position(), black_pawns_bitboard);
    EXPECT_EQ(*empty_squares_ptr, ~black_pawns_bitboard);
    move_stack.top()->undo();
    EXPECT_EQ(black_pawns->current_position(), black_pawns_bitboard);
    EXPECT_EQ(white_pawns->current_position(), white_pawns_bitboard);
    occupied = white_pawns_bitboard ^ black_pawns_bitboard;
    EXPECT_EQ(*empty_squares_ptr, ~occupied);
}