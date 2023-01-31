#include <gtest/gtest.h>
#include "../src/attackmaps.h"
#include "../src/bitboard.h"

TEST(NorthRayAttackMapTest, CanReturnRayWhenThereAreNoBlockingPieces) {
    // 1 . . . . . . . 
    // 1 . . . . . . . 
    // 1 . . . . . . . 
    // 1 . . . . . . . 
    // 1 . . . . . . .
    // x . . . . . . .
    // . . . . . . . .
    // . . . . . . . .

    // Assemble
    Square serialised_piece = 16;
    Bitboard occupied_squares = 0;

    // Act
    Bitboard attack_ray = yak::attackmap::blocked_ray<Direction::NORTH>(serialised_piece, occupied_squares);

    // Assert
    std::vector<Square> serialised_ray = yak::bitboard::scanForward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 5);
}

TEST(NorthRayAttackMapTest, CanReturnRayWhenThereIsASingleBlockingPiece) {
    // . . . . . . . . 
    // B . . . . . . . 
    // 1 . . . . . . . 
    // 1 . . . . . . . 
    // 1 . . . . . . .
    // x . . . . . . .
    // . . . . . . . .
    // . . . . . . . .

    // Assemble
    Square serialised_piece = 16;
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 48;
    
    // Act
    Bitboard attack_ray = yak::attackmap::blocked_ray<Direction::NORTH>(serialised_piece, occupied_squares);

    // Assert
    std::vector<Square> serialised_ray = yak::bitboard::scanForward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 4);
    EXPECT_EQ(serialised_ray[0], 24);
}

TEST(NorthRayAttackMapTest, CanReturnRayWhenBlockingPieceIsOnAdjacentSquare) {
    // . . . . . B . . 
    // . . . . . x . . 
    // . . . . . . . . 
    // . . . . . . . . 
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .

    // Assemble
    Square serialised_piece = 53; // Ray casting piece on F7
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 61; // Blocker on F8
    
    // Act
    Bitboard attack_ray = yak::attackmap::blocked_ray<Direction::NORTH>(serialised_piece, occupied_squares);

    // Assert
    std::vector<Square> serialised_ray = yak::bitboard::scanForward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 1);
    EXPECT_EQ(serialised_ray[0], 61);
}

TEST(NorthRayAttackMapTest, ReturnsEmptyBitboardForAnySquareOnTheEightRank) {

    // Assemble
    Square serialised_pieces[8] = {56, 57, 58, 59, 60, 61, 62, 63};
    Bitboard occupied_squares = 0;
    Bitboard attack_ray[8];

    constexpr std::array<Bitboard, 64> rayMap = yak::attackmap::rayMap<Direction::NORTH>::value;
    // Act
    for (int i = 0; i < 8; i++) {
        attack_ray[i] = rayMap[serialised_pieces[i]];
    }

    // Assert
    EXPECT_EQ(attack_ray[0], 0);
    EXPECT_EQ(attack_ray[1], 0);
    EXPECT_EQ(attack_ray[2], 0);
    EXPECT_EQ(attack_ray[3], 0);
    EXPECT_EQ(attack_ray[4], 0);
    EXPECT_EQ(attack_ray[5], 0);
    EXPECT_EQ(attack_ray[6], 0);
    EXPECT_EQ(attack_ray[7], 0);
}


TEST(SouthRayAttackMapTest, CanReturnRayWhenThereAreNoBlockingPieces) {
    // . . . . . . . . 
    // . . . . . . . . 
    // x . . . . . . . 
    // 1 . . . . . . . 
    // 1 . . . . . . .
    // 1 . . . . . . .
    // 1 . . . . . . .
    // 1 . . . . . . .

    // Assemble
    Square serialised_piece = 40;
    Bitboard occupied_squares = 0;

    // Act
    Bitboard attack_ray = yak::attackmap::blocked_ray<Direction::SOUTH>(serialised_piece, occupied_squares);

    // Assert
    std::vector<Square> serialised_ray = yak::bitboard::scanForward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 5);
}

TEST(SouthRayAttackMapTest, CanReturnRayWhenThereIsASingleBlockingPiece) {
    // . . . . . . . . 
    // . . . . . . . . 
    // x . . . . . . . 
    // 1 . . . . . . . 
    // 1 . . . . . . .
    // 1 . . . . . . .
    // B . . . . . . .
    // . . . . . . . .

    // Assemble
    Square serialised_piece = 40;
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 8;

    // Act
    Bitboard attack_ray = yak::attackmap::blocked_ray<Direction::SOUTH>(serialised_piece, occupied_squares);

    // Assert
    std::vector<Square> serialised_ray = yak::bitboard::scanForward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 4);
    EXPECT_EQ(serialised_ray[0], 8);
}

TEST(SouthRayAttackMapTest, CanReturnRayWhenBlockingPieceIsOnAdjacentSquare) {
    // . . . . . . . . 
    // . . . . . x . . 
    // . . . . . B . . 
    // . . . . . . . . 
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .

    // Assemble
    Square serialised_piece = 53; // Ray casting piece on F7
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 45; // Blocker on F6

    // Act
    Bitboard attack_ray = yak::attackmap::blocked_ray<Direction::SOUTH>(serialised_piece, occupied_squares);

    // Assert
    std::vector<Square> serialised_ray = yak::bitboard::scanForward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 1);
    EXPECT_EQ(serialised_ray[0], 45);
}

TEST(SouthRayAttackMapTest, ReturnsEmptyBitboardForAnySquareOnTheFirstRank) {

    // Assemble
    Square serialised_pieces[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    Bitboard occupied_squares = 0;
    Bitboard attack_ray[8];
  constexpr std::array<Bitboard, 64> rayMap = yak::attackmap::rayMap<Direction::SOUTH>::value;
    // Act
    for (int i = 0; i < 8; i++) {
        attack_ray[i] = rayMap[serialised_pieces[i]];
    }

    // Assert
    EXPECT_EQ(attack_ray[0], 0);
    EXPECT_EQ(attack_ray[1], 0);
    EXPECT_EQ(attack_ray[2], 0);
    EXPECT_EQ(attack_ray[3], 0);
    EXPECT_EQ(attack_ray[4], 0);
    EXPECT_EQ(attack_ray[5], 0);
    EXPECT_EQ(attack_ray[6], 0);
    EXPECT_EQ(attack_ray[7], 0);
}


TEST(EastRayAttackMapTest, CanReturnRayWhenThereAreNoBlockingPieces) {
    // . . . . . . . . 
    // . . . . . . . . 
    // x 1 1 1 1 1 1 1 
    // . . . . . . . . 
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .

    // Assemble
    Square serialised_piece = 40;
    Bitboard occupied_squares = 0;

    // Act
    Bitboard attack_ray = yak::attackmap::blocked_ray<Direction::EAST>(serialised_piece, occupied_squares);

    // Assert
    std::vector<Square> serialised_ray = yak::bitboard::scanForward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 7);
}

TEST(EastRayAttackMapTest, CanReturnRayWhenThereIsASingleBlockingPiece) {
    // . . . . . . . . 
    // . . . . . . . . 
    // x 1 1 1 1 B . . 
    // . . . . . . . . 
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .

    // Assemble
    Square serialised_piece = 40; // Ray casting piece on A6
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 45; // Blocker on F6

    // Act
    Bitboard attack_ray = yak::attackmap::blocked_ray<Direction::EAST>(serialised_piece, occupied_squares);

    // Assert
    std::vector<Square> serialised_ray = yak::bitboard::scanForward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 5);
    EXPECT_EQ(serialised_ray[0], 41);
}

TEST(EastRayAttackMapTest, CanReturnRayWhenBlockingPieceIsOnAdjacentSquare) {
    // . . . . . . . . 
    // . . . . . x B . 
    // . . . . . . . . 
    // . . . . . . . . 
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .

    // Assemble
    Square serialised_piece = 53; // Ray casting piece on F7
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 54; // Blocker on G7
    
    // Act
    Bitboard attack_ray = yak::attackmap::blocked_ray<Direction::EAST>(serialised_piece, occupied_squares);

    // Assert
    std::vector<Square> serialised_ray = yak::bitboard::scanForward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 1);
    EXPECT_EQ(serialised_ray[0], 54);
}

TEST(EastRayAttackMapTest, ReturnsEmptyBitboardForAnySquareOnTheHFile) {

    // Assemble
    Square serialised_pieces[8] = {7, 15, 23, 31, 39, 47, 55, 63}; // H1, ..., H8
    Bitboard occupied_squares = 0;
    std::vector<Bitboard>attack_ray(8, 0);
  constexpr std::array<Bitboard, 64> rayMap = yak::attackmap::rayMap<Direction::EAST>::value;

    // Act
    for (int i = 0; i < 8; i++) {
        attack_ray[i] = rayMap[serialised_pieces[i]];
    }

    // Assert
    for (auto r : attack_ray) {
        EXPECT_EQ(r, 0);
    }
}


TEST(WestRayAttackMapTest, CanReturnRayWhenThereAreNoBlockingPieces) {
    // . . . . . . . . 
    // . . . . . . . . 
    // 1 1 1 1 1 1 1 x 
    // . . . . . . . . 
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .

    // Assemble
    Square serialised_piece = 47; // Ray casting piece on H6
    Bitboard occupied_squares = 0; // No occupied squares

    // Act
    Bitboard attack_ray = yak::attackmap::blocked_ray<Direction::WEST>(serialised_piece, occupied_squares);

    // Assert
    std::vector<Square> serialised_ray = yak::bitboard::scanForward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 7);
}

TEST(WestRayAttackMapTest, CanReturnRayWhenThereIsASingleBlockingPiece) {
    // . . . . . . . . 
    // . . . . . . . . 
    // . . B 1 1 1 1 x 
    // . . . . . . . . 
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .

    // Assemble
    Square serialised_piece = 47; // Ray casting piece on H6
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 42; // Blocker on C6
    
    // Act
    Bitboard attack_ray = yak::attackmap::blocked_ray<Direction::WEST>(serialised_piece, occupied_squares);

    // Assert
    std::vector<Square> serialised_ray = yak::bitboard::scanForward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 5);
    EXPECT_EQ(serialised_ray[0], 42);
}

TEST(WestRayAttackMapTest, CanReturnRayWhenBlockingPieceIsOnAdjacentSquare) {
    // . . . . . . . . 
    // . . . . B x . . 
    // . . . . . . . . 
    // . . . . . . . . 
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .

    // Assemble
    Square serialised_piece = 53; // Ray casting piece on F7
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 52; // Blocker on E7

    // Act
    Bitboard attack_ray = yak::attackmap::blocked_ray<Direction::WEST>(serialised_piece, occupied_squares);

    // Assert
    std::vector<Square> serialised_ray = yak::bitboard::scanForward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 1);
    EXPECT_EQ(serialised_ray[0], 52);
}

TEST(WestRayAttackMapTest, ReturnsEmptyBitboardForAnySquareOnTheAFile) {

    // Assemble
    Square serialised_pieces[8] = {0, 8, 16, 24, 32, 40, 48, 56}; // A1, ..., A8
    Bitboard occupied_squares = 0;
    Bitboard attack_ray[8];
  constexpr std::array<Bitboard, 64> rayMap = yak::attackmap::rayMap<Direction::WEST>::value;
    // Act
    for (int i = 0; i < 8; i++) {
        attack_ray[i] = rayMap[serialised_pieces[i]];
    }

    // Assert
    EXPECT_EQ(attack_ray[0], 0);
    EXPECT_EQ(attack_ray[1], 0);
    EXPECT_EQ(attack_ray[2], 0);
    EXPECT_EQ(attack_ray[3], 0);
    EXPECT_EQ(attack_ray[4], 0);
    EXPECT_EQ(attack_ray[5], 0);
    EXPECT_EQ(attack_ray[6], 0);
    EXPECT_EQ(attack_ray[7], 0);
}


TEST(NorthEastRayAttackMapTest, CanReturnRayWhenThereAreNoBlockingPieces) {
    // . . . . . . 1 . 
    // . . . . . 1 . . 
    // . . . . 1 . . . 
    // . . . 1 . . . . 
    // . . x . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .

    // Assemble
    Square serialised_piece = 26; // Ray casting piece on C4
    Bitboard occupied_squares = 0; // No occupied squares

    // Act
    Bitboard attack_ray = yak::attackmap::blocked_ray<Direction::NORTH_EAST>(serialised_piece, occupied_squares);

    // Assert
    std::vector<Square> serialised_ray = yak::bitboard::scanForward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 4);
}

TEST(NorthEastRayAttackMapTest, CanReturnRayWhenThereIsASingleBlockingPiece) {
    // . . . . . . . . 
    // . . . . . B . . 
    // . . . . 1 . . . 
    // . . . 1 . . . . 
    // . . x . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .

    // Assemble
    Square serialised_piece = 26; // Ray casting piece on C4
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 53; // Blocker on F7

    // Act
    Bitboard attack_ray = yak::attackmap::blocked_ray<Direction::NORTH_EAST>(serialised_piece, occupied_squares);

    // Assert
    std::vector<Square> serialised_ray = yak::bitboard::scanForward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 3);
    EXPECT_EQ(serialised_ray[0], 35);
}

TEST(NorthEastRayAttackMapTest, CanReturnRayWhenBlockingPieceIsOnAdjacentSquare) {
    // . . . . . . B . 
    // . . . . . x . . 
    // . . . . . . . . 
    // . . . . . . . . 
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .

    // Assemble
    Square serialised_piece = 53; // Ray casting piece on F7
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 62; // Blocker on G8
    
    // Act
    Bitboard attack_ray = yak::attackmap::blocked_ray<Direction::NORTH_EAST>(serialised_piece, occupied_squares);

    // Assert
    std::vector<Square> serialised_ray = yak::bitboard::scanForward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 1);
    EXPECT_EQ(serialised_ray[0], 62);
}

TEST(NorthEastRayAttackMapTest, ReturnsEmptyBitboardForAnySquareOnTheHFile) {

    // Assemble
    Square serialised_pieces[8] = {7, 15, 23, 31, 39, 47, 55, 63}; // H1, ..., H8
    Bitboard occupied_squares = 0;
    Bitboard attack_ray[8];
  constexpr std::array<Bitboard, 64> rayMap = yak::attackmap::rayMap<Direction::NORTH_EAST>::value;
  // Act
  for (int i = 0; i < 8; i++) {
    attack_ray[i] = rayMap[serialised_pieces[i]];
  }

    // Assert
    EXPECT_EQ(attack_ray[0], 0);
    EXPECT_EQ(attack_ray[1], 0);
    EXPECT_EQ(attack_ray[2], 0);
    EXPECT_EQ(attack_ray[3], 0);
    EXPECT_EQ(attack_ray[4], 0);
    EXPECT_EQ(attack_ray[5], 0);
    EXPECT_EQ(attack_ray[6], 0);
    EXPECT_EQ(attack_ray[7], 0);
}


TEST(SouthEastRayAttackMapTest, CanReturnRayWhenThereAreNoBlockingPieces) {
    // . . . . . . . . 
    // . . . . . . . . 
    // . . . . . . . . 
    // . . . . . . . . 
    // . . x . . . . .
    // . . . 1 . . . .
    // . . . . 1 . . .
    // . . . . . 1 . .

    // Assemble
    Square serialised_piece = 26; // Ray casting piece on C4
    Bitboard occupied_squares = 0; // No occupied squares

    // Act
    Bitboard attack_ray = yak::attackmap::blocked_ray<Direction::SOUTH_EAST>(serialised_piece, occupied_squares);

    // Assert
    std::vector<Square> serialised_ray = yak::bitboard::scanForward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 3);
}

TEST(SouthEastRayAttackMapTest, CanReturnRayWhenThereIsASingleBlockingPiece) {
    // . . . . . . . . 
    // . . . . . . . . 
    // . . . . . . . . 
    // . . . . . . . . 
    // . . x . . . . .
    // . . . 1 . . . .
    // . . . . B . . .
    // . . . . . . . .

    // Assemble
    Square serialised_piece = 26; // Ray casting piece on C4
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 12; // Blocker on E2

    // Act
    Bitboard attack_ray = yak::attackmap::blocked_ray<Direction::SOUTH_EAST>(serialised_piece, occupied_squares);

    // Assert
    std::vector<Square> serialised_ray = yak::bitboard::scanForward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 2);
    EXPECT_EQ(serialised_ray[0], 12);
}

TEST(SouthEastRayAttackMapTest, CanReturnRayWhenBlockingPieceIsOnAdjacentSquare) {
    // . . . . . . . . 
    // . . . . . x . . 
    // . . . . . . B . 
    // . . . . . . . . 
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .

    // Assemble
    Square serialised_piece = 53; // Ray casting piece on F7
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 46; // Blocker on G6
    
    // Act
    Bitboard attack_ray = yak::attackmap::blocked_ray<Direction::SOUTH_EAST>(serialised_piece, occupied_squares);

    // Assert
    std::vector<Square> serialised_ray = yak::bitboard::scanForward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 1);
    EXPECT_EQ(serialised_ray[0], 46);
}

TEST(SouthEastRayAttackMapTest, ReturnsEmptyBitboardForAnySquareOnTheHFile) {

    // Assemble
    Square serialised_pieces[8] = {7, 15, 23, 31, 39, 47, 55, 63}; // H1, ..., H8
    Bitboard occupied_squares = 0;
    Bitboard attack_ray[8];

  constexpr std::array<Bitboard, 64> rayMap = yak::attackmap::rayMap<Direction::SOUTH_EAST>::value;
  // Act
  for (int i = 0; i < 8; i++) {
    attack_ray[i] = rayMap[serialised_pieces[i]];
  }

    // Assert
    EXPECT_EQ(attack_ray[0], 0);
    EXPECT_EQ(attack_ray[1], 0);
    EXPECT_EQ(attack_ray[2], 0);
    EXPECT_EQ(attack_ray[3], 0);
    EXPECT_EQ(attack_ray[4], 0);
    EXPECT_EQ(attack_ray[5], 0);
    EXPECT_EQ(attack_ray[6], 0);
    EXPECT_EQ(attack_ray[7], 0);
}


TEST(NorthWestRayAttackMapTest, CanReturnRayWhenThereAreNoBlockingPieces) {
    // 1 . . . . . . . 
    // . 1 . . . . . . 
    // . . 1 . . . . . 
    // . . . 1 . . . . 
    // . . . . 1 . . .
    // . . . . . x . .
    // . . . . . . . .
    // . . . . . . . .

    // Assemble
    Square serialised_piece = 21; // Ray casting piece on F3
    Bitboard occupied_squares = 0; // No occupied squares

    // Act
    Bitboard attack_ray = yak::attackmap::blocked_ray<Direction::NORTH_WEST>(serialised_piece, occupied_squares);

    // Assert
    std::vector<Square> serialised_ray = yak::bitboard::scanForward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 5);
}

TEST(NorthWestRayAttackMapTest, CanReturnRayWhenThereIsASingleBlockingPiece) {
    // . . . . . . . . 
    // . . . . . . . . 
    // . . B . . . . . 
    // . . . 1 . . . . 
    // . . . . 1 . . .
    // . . . . . x . .
    // . . . . . . . .
    // . . . . . . . .

    // Assemble
    Square serialised_piece = 21; // Ray casting piece on F3
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 42; // Blocker on C6

    // Act
    Bitboard attack_ray = yak::attackmap::blocked_ray<Direction::NORTH_WEST>(serialised_piece, occupied_squares);

    // Assert
    std::vector<Square> serialised_ray = yak::bitboard::scanForward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 3);
    EXPECT_EQ(serialised_ray[0], 28);
}

TEST(NorthWestRayAttackMapTest, CanReturnRayWhenBlockingPieceIsOnAdjacentSquare) {
    // . . . . B . . . 
    // . . . . . x . . 
    // . . . . . . . . 
    // . . . . . . . . 
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .

    // Assemble
    Square serialised_piece = 53; // Ray casting piece on F7
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 60; // Blocker on G8

    // Act
    Bitboard attack_ray = yak::attackmap::blocked_ray<Direction::NORTH_WEST>(serialised_piece, occupied_squares);

    // Assert
    std::vector<Square> serialised_ray = yak::bitboard::scanForward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 1);
    EXPECT_EQ(serialised_ray[0], 60);
}

TEST(NorthWestRayAttackMapTest, ReturnsEmptyBitboardForAnySquareOnTheAFile) {

    // Assemble
    Square serialised_pieces[8] = {0, 8, 16, 24, 32, 40, 48, 56}; // A1, ..., A8
    Bitboard occupied_squares = 0;
    Bitboard attack_ray[8];

  constexpr std::array<Bitboard, 64> rayMap = yak::attackmap::rayMap<Direction::NORTH_WEST>::value;
  // Act
  for (int i = 0; i < 8; i++) {
    attack_ray[i] = rayMap[serialised_pieces[i]];
  }


  // Assert
    EXPECT_EQ(attack_ray[0], 0);
    EXPECT_EQ(attack_ray[1], 0);
    EXPECT_EQ(attack_ray[2], 0);
    EXPECT_EQ(attack_ray[3], 0);
    EXPECT_EQ(attack_ray[4], 0);
    EXPECT_EQ(attack_ray[5], 0);
    EXPECT_EQ(attack_ray[6], 0);
    EXPECT_EQ(attack_ray[7], 0);
}


TEST(SouthWestRayAttackMapTest, CanReturnRayWhenThereAreNoBlockingPieces) {
    // . . . . . . . . 
    // . . . . . x . . 
    // . . . . 1 . . . 
    // . . . 1 . . . . 
    // . . 1 . . . . .
    // . 1 . . . . . .
    // 1 . . . . . . .
    // . . . . . . . .

    // Assemble
    Square serialised_piece = 53; // Ray casting piece on F7
    Bitboard occupied_squares = 0; // No occupied squares

    // Act
    Bitboard attack_ray = yak::attackmap::blocked_ray<Direction::SOUTH_WEST>(serialised_piece, occupied_squares);

    // Assert
    std::vector<Square> serialised_ray = yak::bitboard::scanForward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 5);
}

TEST(SouthWestRayAttackMapTest, CanReturnRayWhenThereIsASingleBlockingPiece) {
    // . . . . . . . . 
    // . . . . . x . . 
    // . . . . 1 . . . 
    // . . . 1 . . . . 
    // . . B . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .

    // Assemble
    Square serialised_piece = 53; // Ray casting piece on F7
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 26; // Blocker on C4

    // Act
    Bitboard attack_ray = yak::attackmap::blocked_ray<Direction::SOUTH_WEST>(serialised_piece, occupied_squares);

    // Assert
    std::vector<Square> serialised_ray = yak::bitboard::scanForward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 3);
    EXPECT_EQ(serialised_ray[0], 26);
}

TEST(SouthWestRayAttackMapTest, CanReturnRayWhenBlockingPieceIsOnAdjacentSquare) {
    // . . . . . . . . 
    // . . . . . x . . 
    // . . . . B . . . 
    // . . . . . . . . 
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .

    // Assemble
    Square serialised_piece = 53; // Ray casting piece on F7
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 44; // Blocker on E6

    // Act
    Bitboard attack_ray = yak::attackmap::blocked_ray<Direction::SOUTH_WEST>(serialised_piece, occupied_squares);

    // Assert
    std::vector<Square> serialised_ray = yak::bitboard::scanForward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 1);
    EXPECT_EQ(serialised_ray[0], 44);
}

TEST(SouthWestRayAttackMapTest, ReturnsEmptyBitboardForAnySquareOnTheAFile) {

    // Assemble
    Square serialised_pieces[8] = {0, 8, 16, 24, 32, 40, 48, 56}; // A1, ..., A8
    Bitboard occupied_squares = 0;
    Bitboard attack_ray[8];

  constexpr std::array<Bitboard, 64> rayMap = yak::attackmap::rayMap<Direction::SOUTH_WEST>::value;
  // Act
  for (int i = 0; i < 8; i++) {
    attack_ray[i] = rayMap[serialised_pieces[i]];
  }

  // Assert
    EXPECT_EQ(attack_ray[0], 0);
    EXPECT_EQ(attack_ray[1], 0);
    EXPECT_EQ(attack_ray[2], 0);
    EXPECT_EQ(attack_ray[3], 0);
    EXPECT_EQ(attack_ray[4], 0);
    EXPECT_EQ(attack_ray[5], 0);
    EXPECT_EQ(attack_ray[6], 0);
    EXPECT_EQ(attack_ray[7], 0);
}