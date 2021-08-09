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