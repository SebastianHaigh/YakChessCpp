#include <catch2/catch_test_macros.hpp>
#include <attackmaps.h>
#include <bitboard.h>

TEST_CASE("NorthRayAttackMapTest: Can return ray when there are no blocking pieces") {
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
    CHECK(serialised_ray.size() == 5);
}

TEST_CASE("NorthRayAttackMapTest: CanReturnRayWhenThereIsASingleBlockingPiece") {
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
    CHECK(serialised_ray.size() == 4);
    CHECK(serialised_ray[0] == 24);
}

TEST_CASE("NorthRayAttackMapTest: CanReturnRayWhenBlockingPieceIsOnAdjacentSquare") {
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
    CHECK(serialised_ray.size() == 1);
    CHECK(serialised_ray[0] == 61);
}

TEST_CASE("NorthRayAttackMapTest: ReturnsEmptyBitboardForAnySquareOnTheEightRank") {

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
    CHECK(attack_ray[0] == 0);
    CHECK(attack_ray[1] == 0);
    CHECK(attack_ray[2] == 0);
    CHECK(attack_ray[3] == 0);
    CHECK(attack_ray[4] == 0);
    CHECK(attack_ray[5] == 0);
    CHECK(attack_ray[6] == 0);
    CHECK(attack_ray[7] == 0);
}

TEST_CASE("SouthRayAttackMapTest: CanReturnRayWhenThereAreNoBlockingPieces") {
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
    CHECK(serialised_ray.size() == 5);
}

TEST_CASE("SouthRayAttackMapTest: CanReturnRayWhenThereIsASingleBlockingPiece") {
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
    CHECK(serialised_ray.size() == 4);
    CHECK(serialised_ray[0] == 8);
}

TEST_CASE("SouthRayAttackMapTest: CanReturnRayWhenBlockingPieceIsOnAdjacentSquare") {
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
    CHECK(serialised_ray.size() == 1);
    CHECK(serialised_ray[0] == 45);
}

TEST_CASE("SouthRayAttackMapTest: ReturnsEmptyBitboardForAnySquareOnTheFirstRank") {

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
    CHECK(attack_ray[0] == 0);
    CHECK(attack_ray[1] == 0);
    CHECK(attack_ray[2] == 0);
    CHECK(attack_ray[3] == 0);
    CHECK(attack_ray[4] == 0);
    CHECK(attack_ray[5] == 0);
    CHECK(attack_ray[6] == 0);
    CHECK(attack_ray[7] == 0);
}

TEST_CASE("EastRayAttackMapTest: CanReturnRayWhenThereAreNoBlockingPieces") {
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
    CHECK(serialised_ray.size() == 7);
}

TEST_CASE("EastRayAttackMapTest: CanReturnRayWhenThereIsASingleBlockingPiece") {
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
    CHECK(serialised_ray.size() == 5);
    CHECK(serialised_ray[0] == 41);
}

TEST_CASE("EastRayAttackMapTest: CanReturnRayWhenBlockingPieceIsOnAdjacentSquare") {
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
    CHECK(serialised_ray.size() == 1);
    CHECK(serialised_ray[0] == 54);
}

TEST_CASE("EastRayAttackMapTest: ReturnsEmptyBitboardForAnySquareOnTheHFile") {

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
        CHECK(r == 0);
    }
}

TEST_CASE("WestRayAttackMapTest: CanReturnRayWhenThereAreNoBlockingPieces") {
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
    CHECK(serialised_ray.size() == 7);
}

TEST_CASE("WestRayAttackMapTest: CanReturnRayWhenThereIsASingleBlockingPiece") {
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
    CHECK(serialised_ray.size() == 5);
    CHECK(serialised_ray[0] == 42);
}

TEST_CASE("WestRayAttackMapTest: CanReturnRayWhenBlockingPieceIsOnAdjacentSquare") {
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
    CHECK(serialised_ray.size() == 1);
    CHECK(serialised_ray[0] == 52);
}

TEST_CASE("WestRayAttackMapTest: ReturnsEmptyBitboardForAnySquareOnTheAFile") {

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
    CHECK(attack_ray[0] == 0);
    CHECK(attack_ray[1] == 0);
    CHECK(attack_ray[2] == 0);
    CHECK(attack_ray[3] == 0);
    CHECK(attack_ray[4] == 0);
    CHECK(attack_ray[5] == 0);
    CHECK(attack_ray[6] == 0);
    CHECK(attack_ray[7] == 0);
}


TEST_CASE("NorthEastRayAttackMapTest: CanReturnRayWhenThereAreNoBlockingPieces") {
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
    CHECK(serialised_ray.size() == 4);
}

TEST_CASE("NorthEastRayAttackMapTest: CanReturnRayWhenThereIsASingleBlockingPiece") {
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
    CHECK(serialised_ray.size() == 3);
    CHECK(serialised_ray[0] == 35);
}

TEST_CASE("NorthEastRayAttackMapTest: CanReturnRayWhenBlockingPieceIsOnAdjacentSquare") {
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
    CHECK(serialised_ray.size() == 1);
    CHECK(serialised_ray[0] == 62);
}

TEST_CASE("NorthEastRayAttackMapTest: ReturnsEmptyBitboardForAnySquareOnTheHFile") {

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
    CHECK(attack_ray[0] == 0);
    CHECK(attack_ray[1] == 0);
    CHECK(attack_ray[2] == 0);
    CHECK(attack_ray[3] == 0);
    CHECK(attack_ray[4] == 0);
    CHECK(attack_ray[5] == 0);
    CHECK(attack_ray[6] == 0);
    CHECK(attack_ray[7] == 0);
}


TEST_CASE("SouthEastRayAttackMapTest: CanReturnRayWhenThereAreNoBlockingPieces") {
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
    CHECK(serialised_ray.size() == 3);
}

TEST_CASE("SouthEastRayAttackMapTest: CanReturnRayWhenThereIsASingleBlockingPiece") {
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
    CHECK(serialised_ray.size() == 2);
    CHECK(serialised_ray[0] == 12);
}

TEST_CASE("SouthEastRayAttackMapTest: CanReturnRayWhenBlockingPieceIsOnAdjacentSquare") {
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
    CHECK(serialised_ray.size() == 1);
    CHECK(serialised_ray[0] == 46);
}

TEST_CASE("SouthEastRayAttackMapTest: ReturnsEmptyBitboardForAnySquareOnTheHFile") {

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
    CHECK(attack_ray[0] == 0);
    CHECK(attack_ray[1] == 0);
    CHECK(attack_ray[2] == 0);
    CHECK(attack_ray[3] == 0);
    CHECK(attack_ray[4] == 0);
    CHECK(attack_ray[5] == 0);
    CHECK(attack_ray[6] == 0);
    CHECK(attack_ray[7] == 0);
}


TEST_CASE("NorthWestRayAttackMapTest: CanReturnRayWhenThereAreNoBlockingPieces") {
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
    CHECK(serialised_ray.size() == 5);
}

TEST_CASE("NorthWestRayAttackMapTest: CanReturnRayWhenThereIsASingleBlockingPiece") {
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
    CHECK(serialised_ray.size() == 3);
    CHECK(serialised_ray[0] == 28);
}

TEST_CASE("NorthWestRayAttackMapTest: CanReturnRayWhenBlockingPieceIsOnAdjacentSquare") {
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
    CHECK(serialised_ray.size() == 1);
    CHECK(serialised_ray[0] == 60);
}

TEST_CASE("NorthWestRayAttackMapTest: ReturnsEmptyBitboardForAnySquareOnTheAFile") {

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
    CHECK(attack_ray[0] == 0);
    CHECK(attack_ray[1] == 0);
    CHECK(attack_ray[2] == 0);
    CHECK(attack_ray[3] == 0);
    CHECK(attack_ray[4] == 0);
    CHECK(attack_ray[5] == 0);
    CHECK(attack_ray[6] == 0);
    CHECK(attack_ray[7] == 0);
}

TEST_CASE("SouthWestRayAttackMapTest: CanReturnRayWhenThereAreNoBlockingPieces") {
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
    CHECK(serialised_ray.size() == 5);
}

TEST_CASE("SouthWestRayAttackMapTest: CanReturnRayWhenThereIsASingleBlockingPiece") {
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
    CHECK(serialised_ray.size() == 3);
    CHECK(serialised_ray[0] == 26);
}

TEST_CASE("SouthWestRayAttackMapTest: CanReturnRayWhenBlockingPieceIsOnAdjacentSquare") {
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
    CHECK(serialised_ray.size() == 1);
    CHECK(serialised_ray[0] == 44);
}

TEST_CASE("SouthWestRayAttackMapTest: ReturnsEmptyBitboardForAnySquareOnTheAFile") {

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
    CHECK(attack_ray[0] == 0);
    CHECK(attack_ray[1] == 0);
    CHECK(attack_ray[2] == 0);
    CHECK(attack_ray[3] == 0);
    CHECK(attack_ray[4] == 0);
    CHECK(attack_ray[5] == 0);
    CHECK(attack_ray[6] == 0);
    CHECK(attack_ray[7] == 0);
}
