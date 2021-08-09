#include <gtest/gtest.h>
#include "../attackmaps.h"
#include "../bitboard.h"

TEST(AttackMapTest, CanReturnRayWhenThereAreNoBlockingPieces) {
    // 1 . . . . . . . 
    // 1 . . . . . . . 
    // 1 . . . . . . . 
    // 1 . . . . . . . 
    // 1 . . . . . . .
    // x . . . . . . .
    // . . . . . . . .
    // . . . . . . . .

    // Assemble
    attacks::NorthRay ray = attacks::NorthRay();
    Square serialised_piece = 16;
    Bitboard occupied_squares = 0;
    Bitboard attack_ray;

    // Act
    attack_ray = ray.get(serialised_piece, occupied_squares);

    // Assert
    std::stack<Square> serialised_ray = bitboard::scan_forward(attack_ray);
    EXPECT_EQ(serialised_ray.size(), 5);
}

TEST(AttackMapTest, CanReturnRayWhenThereIsASingleBlockingPiece) {
    // . . . . . . . . 
    // B . . . . . . . 
    // 1 . . . . . . . 
    // 1 . . . . . . . 
    // 1 . . . . . . .
    // x . . . . . . .
    // . . . . . . . .
    // . . . . . . . .

    // Assemble
    attacks::NorthRay ray = attacks::NorthRay();
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

TEST(AttackMapTest, ReturnsEmptyBitboardForAnySquareOnTheEightRank) {

    // Assemble
    attacks::NorthRay ray = attacks::NorthRay();
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