#include <gtest/gtest.h>
#include "../bitboard.h"

TEST(BitboardTest, SouthOne) {
    // Arrange
    Bitboard initial_bitboard = 0x000000000000ff00; // RANK 2
    Bitboard shifted_bitboard = 0;
    Bitboard expected_bitboard = 0x00000000000000ff; // RANK 1
    // Act

    shifted_bitboard = bitboard::south_one(initial_bitboard);

    // Assert
    EXPECT_EQ(shifted_bitboard, expected_bitboard);
}

TEST(BitboardTest, SouthOneFromRank1) {
    // Arrange
    Bitboard initial_bitboard = 0x00000000000000ff; // RANK 1
    Bitboard shifted_bitboard = 0;
    Bitboard expected_bitboard = 0; // EMPTY BOARD
    // Act
    shifted_bitboard = bitboard::south_one(initial_bitboard);

    // Assert
    EXPECT_EQ(shifted_bitboard, expected_bitboard);
}


TEST(BitboardTest, NorthOne) {
    // Arrange
    Bitboard initial_bitboard = 0x000000000000ff00; // RANK 2
    Bitboard shifted_bitboard = 0;
    Bitboard expected_bitboard = 0x0000000000ff0000; // RANK 3
    
    // Act
    shifted_bitboard = bitboard::north_one(initial_bitboard);
    
    // Assert
    EXPECT_EQ(shifted_bitboard, expected_bitboard);
}

TEST(BitboardTest, NorthOneFromRank8) {
    // Arrange
    Bitboard initial_bitboard = 0xff00000000000000; // RANK 8
    Bitboard shifted_bitboard = 0;
    Bitboard expected_bitboard = 0; // EMPTY BOARD
    
    // Act
    shifted_bitboard = bitboard::north_one(initial_bitboard);

    // Assert
    EXPECT_EQ(shifted_bitboard, expected_bitboard);
}

TEST(BitboardTest, EastOne) {
    // Arrange
    Bitboard initial_bitboard = 0x0202020202020202; // B FILE
    Bitboard shifted_bitboard = 0;
    Bitboard expected_bitboard = 0x0404040404040404; // C FILE

    // Act
    shifted_bitboard = bitboard::east_one(initial_bitboard);

    // Assert
    EXPECT_EQ(shifted_bitboard, expected_bitboard);
}

TEST(BitboardTest, EastOneFromHFile) {
    // Arrange
    Bitboard initial_bitboard = 0x8080808080808080; // H FILE
    Bitboard shifted_bitboard = 0;
    Bitboard expected_bitboard = 0; // EMPTY BOARD
    
    // Act
    shifted_bitboard = bitboard::east_one(initial_bitboard);

    // Assert
    EXPECT_EQ(shifted_bitboard, expected_bitboard);
}

TEST(BitboardTest, WestOne) {
    // Arrange
    Bitboard initial_bitboard = 0x0202020202020202; // B FILE
    Bitboard shifted_bitboard = 0;
    Bitboard expected_bitboard = 0x0101010101010101; // A FILE
  
    // Act
    shifted_bitboard = bitboard::west_one(initial_bitboard);

    // Assert
    EXPECT_EQ(shifted_bitboard, expected_bitboard);
}

TEST(BitboardTest, WestOneFromAFile) {
    // Arrange
    Bitboard initial_bitboard = 0x0101010101010101; // A FILE
    Bitboard shifted_bitboard = 0;
    Bitboard expected_bitboard = 0; // EMPTY BOARD

    // Act
    shifted_bitboard = bitboard::west_one(initial_bitboard);

    // Assert
    EXPECT_EQ(shifted_bitboard, expected_bitboard);
}

TEST(BitboardTest, NorthEastOneFromMiddleOfBoard) { 
    // Arrange
    Bitboard initial_bitboard = 1 << 8; // A2
    Bitboard shifted_bitboard = 0;
    Bitboard expected_bitboard = 1 << 17; // A2 should go to B3 (square 17)
  
    // Act
    shifted_bitboard = bitboard::north_east_one(initial_bitboard);

    // Assert
    EXPECT_EQ(shifted_bitboard, expected_bitboard);
}

TEST(BitboardTest, SouthEastOneFromMiddleOfBoard) { 
    // Arrange
    Bitboard initial_bitboard = 1 << 8; // A2
    Bitboard shifted_bitboard = 0;
    Bitboard expected_bitboard = 1 << 1; // A2 should go to B1 (square 1)

    // Act
    shifted_bitboard =  bitboard::south_east_one(initial_bitboard);

    // Assert
    EXPECT_EQ(shifted_bitboard, expected_bitboard);
}

TEST(BitboardTest, NorthWestOneFromMiddleOfBoard) { 
    // Arrange
    Bitboard initial_bitboard = 0x0101010101010101; // A FILE
    Bitboard shifted_bitboard = 0;
    Bitboard expected_bitboard = 0; // EMPTY BOARD

    //Act
    shifted_bitboard = bitboard::north_west_one(initial_bitboard);

    // Assert
    EXPECT_EQ(shifted_bitboard, expected_bitboard);
}

TEST(BitboardTest, SouthWestOneFromMiddleOfBoard) { 
    // Arrange
    Bitboard initial_bitboard = 0x0101010101010101; // A FILE
    Bitboard shifted_bitboard = 0;
    Bitboard expected_bitboard = 0; // EMPTY BOARD

    // Act
    shifted_bitboard = bitboard::south_west_one(initial_bitboard);

    // Assert
    EXPECT_EQ(shifted_bitboard, expected_bitboard);
}

TEST(BitboardTest, ScanForwardOnEmptyBoardReturnsEmptyStack) {
    // Arrange
    Bitboard initial_bitboard = 0;
    std::stack<Square> serialised_bitboard;

    // Act
    serialised_bitboard = bitboard::scan_forward(initial_bitboard);

    // Assert
    EXPECT_EQ(serialised_bitboard.size(), 0);
}

TEST(BitboardTest, ScanForwardForPieceOnE4) {
    // Arrange
    Bitboard initial_bitboard = 1 << 28;  // E4
    std::stack<Square> serialised_bitboard;

    // Act
    serialised_bitboard = bitboard::scan_forward(initial_bitboard);

    // Assert
    EXPECT_EQ(serialised_bitboard.top(), 28);
}

TEST(BitboardTest, ScanBackwardOnEmptyBoardReturnsEmptyStack) {
    // Arrange
    Bitboard initial_bitboard = 0;
    std::stack<Square> serialised_bitboard;

    // Act
    serialised_bitboard = bitboard::scan_backward(initial_bitboard);

    // Assert
    EXPECT_EQ(serialised_bitboard.size(), 0); // Force Fail
}

TEST(BitboardTest, ScanBackwardForPieceOnE4) { 
    // Arrange
    Bitboard initial_bitboard = 1 << 28;  // E4
    std::stack<Square> serialised_bitboard;

    // Act
    serialised_bitboard = bitboard::scan_backward(initial_bitboard);

    // Assert
    EXPECT_EQ(serialised_bitboard.top(), 28); // Force Fail   
}

TEST(BitboardTest, CanGetFileIndexOfSquareOnTheBoard) { 
    // Arrange
    Square square_index = 7;  // I think this is A8
    File expected_file = 7;
    File file_of_a8;

    // Act
    file_of_a8 = bitboard::file_index(square_index);

    // Assert
    EXPECT_EQ(file_of_a8, expected_file);
}

TEST(BitboardTest, FileIndexReturnsMinusOneWhenSquareIndexIsOutOfRange) { 
    // Arrange
    Square square_index = 65;
    File file_of_invalid_square;

    // Act
    file_of_invalid_square = bitboard::file_index(square_index);

    // Assert
    EXPECT_EQ(file_of_invalid_square, -1);
}

TEST(BitboardTest, CanGetRankIndexOfSquareOnTheBoard) {
    // Arrange
    Square square_index = 7;  // I think this is A8
    Rank expected_rank = 0; // First Rank
    Rank rank_of_a8;

    // Act
    rank_of_a8 = bitboard::rank_index(square_index);

    // Assert
    EXPECT_EQ(rank_of_a8, expected_rank);
}

TEST(BitboardTest, RankIndexReturnsMinusOneWhenSquareIndexIsOutOfRange) { 
    // Arrange
    Square square_index = 65;
    Rank rank_of_invalid_square;

    // Act
    rank_of_invalid_square = bitboard::rank_index(square_index);

    // Assert
    EXPECT_EQ(rank_of_invalid_square, -1);
}

TEST(BitboardTest, CanGetSquareIndexFromVaildRankAndFileIndices) {
    // Arrange
    File valid_file = 7;
    Rank valid_rank = 0;
    Square expected_square = 7;
    Square valid_square;

    // Act
    valid_square = bitboard::square_index(valid_file, valid_rank);

    // Assert
    EXPECT_EQ(valid_square, expected_square);
}

TEST(BitboardTest, SetSquareCanCorrectlySetAnUnsetSquareOnBitboard) {
    // Arrange
    Square square{ 0 };
    Bitboard expected{ 1 };
    Bitboard actual{ 0 };

    // Act
    bitboard::set_square(actual, square);

    // Assert
    EXPECT_EQ(actual, expected);
}

TEST(BitboardTest, SetSquareCanCorrectlySetAnUnsetSquareOnBitboardWithRankAndFile) {
    // Arrange
    Rank rank{ 0 };
    File file{ 0 };
    Bitboard expected{ 1 };
    Bitboard actual{ 0 };

    // Act
    bitboard::set_square(actual, rank, file);

    // Assert
    EXPECT_EQ(actual, expected);
}

TEST(BitboardTest, SquareToBitboardDetectsE4) {
    // Arrange
    Square square{ 28 }; // E4
    Bitboard expected{ bitboard::RANK_4 & bitboard::FILE_E };

    // Act
    Bitboard actual = bitboard::to_bitboard(square);

    // Assert
    EXPECT_EQ(actual, expected);
}