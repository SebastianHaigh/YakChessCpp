#include <gtest/gtest.h>
#include "../bitboard.h"

TEST(BitBoardTest, SouthOne) {
  // Assemble
    Bitboard initial_bitboard = 0x000000000000ff00; // RANK 2
    Bitboard shifted_bitboard = 0;
    Bitboard expected_bitboard = 0x00000000000000ff; // RANK 1
  // Act

    shifted_bitboard = bitboard::south_one(initial_bitboard);

  // Assert
    EXPECT_EQ(shifted_bitboard, expected_bitboard);
}

TEST(BitBoardTest, SouthOneFromRank1) {
  // Assemble
    Bitboard initial_bitboard = 0x00000000000000ff; // RANK 1
    Bitboard shifted_bitboard = 0;
    Bitboard expected_bitboard = 0; // EMPTY BOARD
  // Act

    shifted_bitboard = bitboard::south_one(initial_bitboard);

  // Assert
    EXPECT_EQ(shifted_bitboard, expected_bitboard);
}


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

TEST(BitBoardTest, NorthOneFromRank8) {
  // Assemble
    Bitboard initial_bitboard = 0xff00000000000000; // RANK 8
    Bitboard shifted_bitboard = 0;
    Bitboard expected_bitboard = 0; // EMPTY BOARD
  // Act

    shifted_bitboard = bitboard::north_one(initial_bitboard);

  // Assert
    EXPECT_EQ(shifted_bitboard, expected_bitboard);
}

TEST(BitBoardTest, EastOne) {
  // Assemble
    Bitboard initial_bitboard = 0x0202020202020202; // B FILE
    Bitboard shifted_bitboard = 0;
    Bitboard expected_bitboard = 0x0404040404040404; // C FILE
  // Act

    shifted_bitboard = bitboard::east_one(initial_bitboard);

  // Assert
    EXPECT_EQ(shifted_bitboard, expected_bitboard);
}

TEST(BitBoardTest, EastOneFromHFile) {
  // Assemble
    Bitboard initial_bitboard = 0x8080808080808080; // H FILE
    Bitboard shifted_bitboard = 0;
    Bitboard expected_bitboard = 0; // EMPTY BOARD
  // Act

    shifted_bitboard = bitboard::east_one(initial_bitboard);

  // Assert
    EXPECT_EQ(shifted_bitboard, expected_bitboard);
}

TEST(BitBoardTest, WestOne) {
  // Assemble
    Bitboard initial_bitboard = 0x0202020202020202; // B FILE
    Bitboard shifted_bitboard = 0;
    Bitboard expected_bitboard = 0x0101010101010101; // A FILE
  // Act

    shifted_bitboard = bitboard::west_one(initial_bitboard);

  // Assert
    EXPECT_EQ(shifted_bitboard, expected_bitboard);
}

TEST(BitBoardTest, WestOneFromAFile) {
  // Assemble
    Bitboard initial_bitboard = 0x0101010101010101; // A FILE
    Bitboard shifted_bitboard = 0;
    Bitboard expected_bitboard = 0; // EMPTY BOARD
  // Act

    shifted_bitboard = bitboard::west_one(initial_bitboard);

  // Assert
    EXPECT_EQ(shifted_bitboard, expected_bitboard);
}