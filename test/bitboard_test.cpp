#include "../bitboard.h"
#include <gtest/gtest.h>

namespace {
    TEST(BitBoardTest, NorthOne) {
        Bitboard board = 1;
        expected = board << 8;
        EXPECT_EQ(expected, bitboard::north_one(board));
    }
} // namespace
