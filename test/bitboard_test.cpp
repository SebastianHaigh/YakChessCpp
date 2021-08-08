#include "../bitboard.h"
#include <gtest/gtest.h>


TEST(BitBoardTest, NorthOne) {
    Bitboard board = 1;
    Bitboard expected = board << 8;
    EXPECT_EQ(expected, bitboard::north_one(board));
}

