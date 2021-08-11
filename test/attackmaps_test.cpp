#include <gtest/gtest.h>
#include "../attackmaps.h"
#include "../bitboard.h"

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
    auto ray = attacks::NorthRay();
    Square serialised_piece = 16;
    Bitboard occupied_squares = 0;
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
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
    auto ray = attacks::NorthRay();
    Square serialised_piece = 16;
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 48;
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 4);
    EXPECT_EQ(serialised_ray.top(), 24);
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
    auto ray = attacks::NorthRay();
    Square serialised_piece = 53; // Ray casting piece on F7
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 61; // Blocker on F8
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 1);
    EXPECT_EQ(serialised_ray.top(), 61);
}

TEST(NorthRayAttackMapTest, ReturnsEmptyBitboardForAnySquareOnTheEightRank) {

    // Assemble
    auto ray = attacks::NorthRay();
    Square serialised_pieces[8] = {56, 57, 58, 59, 60, 61, 62, 63};
    Bitboard occupied_squares = 0;
    Bitboard attack_ray[8];

    // Act
    for (int i = 0; i < 8; i++) {
        attack_ray[i] = ray.get(serialised_pieces[i], occupied_squares);
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
    auto ray = attacks::SouthRay();
    Square serialised_piece = 40;
    Bitboard occupied_squares = 0;
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
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
    auto ray = attacks::SouthRay();
    Square serialised_piece = 40;
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 8;
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 4);
    EXPECT_EQ(serialised_ray.top(), 8);
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
    auto ray = attacks::SouthRay();
    Square serialised_piece = 53; // Ray casting piece on F7
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 45; // Blocker on F6
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 1);
    EXPECT_EQ(serialised_ray.top(), 45);
}

TEST(SouthRayAttackMapTest, ReturnsEmptyBitboardForAnySquareOnTheFirstRank) {

    // Assemble
    auto ray = attacks::SouthRay();
    Square serialised_pieces[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    Bitboard occupied_squares = 0;
    Bitboard attack_ray[8];

    // Act
    for (int i = 0; i < 8; i++) {
        attack_ray[i] = ray.get(serialised_pieces[i], occupied_squares);
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
    auto ray = attacks::EastRay();
    Square serialised_piece = 40;
    Bitboard occupied_squares = 0;
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
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
    auto ray = attacks::EastRay();
    Square serialised_piece = 40; // Ray casting piece on A6
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 45; // Blocker on F6
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 5);
    EXPECT_EQ(serialised_ray.top(), 41);
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
    auto ray = attacks::EastRay();
    Square serialised_piece = 53; // Ray casting piece on F7
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 54; // Blocker on G7
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 1);
    EXPECT_EQ(serialised_ray.top(), 54);
}

TEST(EastRayAttackMapTest, ReturnsEmptyBitboardForAnySquareOnTheHFile) {

    // Assemble
    auto ray = attacks::EastRay();
    Square serialised_pieces[8] = {7, 15, 23, 31, 39, 47, 55, 63}; // H1, ..., H8
    Bitboard occupied_squares = 0;
    Bitboard attack_ray[8];

    // Act
    for (int i = 0; i < 8; i++) {
        attack_ray[i] = ray.get(serialised_pieces[i], occupied_squares);
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
    auto ray = attacks::WestRay();
    Square serialised_piece = 47; // Ray casting piece on H6
    Bitboard occupied_squares = 0; // No occupied squares
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
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
    auto ray = attacks::WestRay();
    Square serialised_piece = 47; // Ray casting piece on H6
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 42; // Blocker on C6
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 5);
    EXPECT_EQ(serialised_ray.top(), 42);
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
    auto ray = attacks::WestRay();
    Square serialised_piece = 53; // Ray casting piece on F7
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 52; // Blocker on E7
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 1);
    EXPECT_EQ(serialised_ray.top(), 52);
}

TEST(WestRayAttackMapTest, ReturnsEmptyBitboardForAnySquareOnTheAFile) {

    // Assemble
    auto ray = attacks::WestRay();
    Square serialised_pieces[8] = {0, 8, 16, 24, 32, 40, 48, 56}; // A1, ..., A8
    Bitboard occupied_squares = 0;
    Bitboard attack_ray[8];

    // Act
    for (int i = 0; i < 8; i++) {
        attack_ray[i] = ray.get(serialised_pieces[i], occupied_squares);
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
    auto ray = attacks::NorthEastRay();
    Square serialised_piece = 26; // Ray casting piece on C4
    Bitboard occupied_squares = 0; // No occupied squares
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
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
    auto ray = attacks::NorthEastRay();
    Square serialised_piece = 26; // Ray casting piece on C4
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 53; // Blocker on F7
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 3);
    EXPECT_EQ(serialised_ray.top(), 35);
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
    auto ray = attacks::NorthEastRay();
    Square serialised_piece = 53; // Ray casting piece on F7
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 62; // Blocker on G8
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 1);
    EXPECT_EQ(serialised_ray.top(), 62);
}

TEST(NorthEastRayAttackMapTest, ReturnsEmptyBitboardForAnySquareOnTheHFile) {

    // Assemble
    auto ray = attacks::NorthEastRay();
    Square serialised_pieces[8] = {7, 15, 23, 31, 39, 47, 55, 63}; // H1, ..., H8
    Bitboard occupied_squares = 0;
    Bitboard attack_ray[8];

    // Act
    for (int i = 0; i < 8; i++) {
        attack_ray[i] = ray.get(serialised_pieces[i], occupied_squares);
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
    auto ray = attacks::SouthEastRay();
    Square serialised_piece = 26; // Ray casting piece on C4
    Bitboard occupied_squares = 0; // No occupied squares
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
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
    auto ray = attacks::SouthEastRay();
    Square serialised_piece = 26; // Ray casting piece on C4
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 12; // Blocker on E2
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 2);
    EXPECT_EQ(serialised_ray.top(), 12);
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
    auto ray = attacks::SouthEastRay();
    Square serialised_piece = 53; // Ray casting piece on F7
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 46; // Blocker on G6
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 1);
    EXPECT_EQ(serialised_ray.top(), 46);
}

TEST(SouthEastRayAttackMapTest, ReturnsEmptyBitboardForAnySquareOnTheHFile) {

    // Assemble
    auto ray = attacks::SouthEastRay();
    Square serialised_pieces[8] = {7, 15, 23, 31, 39, 47, 55, 63}; // H1, ..., H8
    Bitboard occupied_squares = 0;
    Bitboard attack_ray[8];

    // Act
    for (int i = 0; i < 8; i++) {
        attack_ray[i] = ray.get(serialised_pieces[i], occupied_squares);
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
    auto ray = attacks::NorthWestRay();
    Square serialised_piece = 21; // Ray casting piece on F3
    Bitboard occupied_squares = 0; // No occupied squares
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
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
    auto ray = attacks::NorthWestRay();
    Square serialised_piece = 21; // Ray casting piece on F3
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 42; // Blocker on C6
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 3);
    EXPECT_EQ(serialised_ray.top(), 28);
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
    auto ray = attacks::NorthWestRay();
    Square serialised_piece = 53; // Ray casting piece on F7
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 60; // Blocker on G8
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 1);
    EXPECT_EQ(serialised_ray.top(), 60);
}

TEST(NorthWestRayAttackMapTest, ReturnsEmptyBitboardForAnySquareOnTheAFile) {

    // Assemble
    auto ray = attacks::NorthWestRay();
    Square serialised_pieces[8] = {0, 8, 16, 24, 32, 40, 48, 56}; // A1, ..., A8
    Bitboard occupied_squares = 0;
    Bitboard attack_ray[8];

    // Act
    for (int i = 0; i < 8; i++) {
        attack_ray[i] = ray.get(serialised_pieces[i], occupied_squares);
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
    auto ray = attacks::SouthWestRay();
    Square serialised_piece = 53; // Ray casting piece on F7
    Bitboard occupied_squares = 0; // No occupied squares
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
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
    auto ray = attacks::SouthWestRay();
    Square serialised_piece = 53; // Ray casting piece on F7
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 26; // Blocker on C4
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 3);
    EXPECT_EQ(serialised_ray.top(), 26);
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
    auto ray = attacks::SouthWestRay();
    Square serialised_piece = 53; // Ray casting piece on F7
    Bitboard occupied_squares = 1;
    occupied_squares = occupied_squares << 44; // Blocker on E6
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 1);
    EXPECT_EQ(serialised_ray.top(), 44);
}

TEST(SouthWestRayAttackMapTest, ReturnsEmptyBitboardForAnySquareOnTheAFile) {

    // Assemble
    auto ray = attacks::SouthWestRay();
    Square serialised_pieces[8] = {0, 8, 16, 24, 32, 40, 48, 56}; // A1, ..., A8
    Bitboard occupied_squares = 0;
    Bitboard attack_ray[8];

    // Act
    for (int i = 0; i < 8; i++) {
        attack_ray[i] = ray.get(serialised_pieces[i], occupied_squares);
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