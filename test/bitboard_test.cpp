#include <gtest/gtest.h>

#include "../bitboard.h"

TEST(BitBoardTest, NorthOne) {
    // Assemble
    Bitboard initial_bitboard = 0x000000000000ff00; // RANK 2
    Bitboard shifted_bitboard = 0;
    Bitboard expected_bitboard = 0x0000000000ff0000; // RANK 3
    
    // Act
    shifted_bitboard = bitboard::north_one(initial_bitboard);
    
    // Assert
    EXPECT_EQ(shifted_bitboard, expected_bitboard);
}