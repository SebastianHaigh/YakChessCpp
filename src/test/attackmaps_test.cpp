#include <catch2/catch_test_macros.hpp>

#include <attackmaps.h>
#include <types.h>
#include <bitboard.h>

#include <array>
#include <vector>

namespace yak::attackmap {

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
    Square serialisedPiece{A3};
    Bitboard occupiedSquares_bb{0};

    // Act
    Bitboard attackRay_bb = yak::attackmap::blockedRay<Direction::NORTH>(serialisedPiece, occupiedSquares_bb);

    // Assert
    std::vector<Square> serialisedRay = yak::bitboard::scanForward(attackRay_bb);
    CHECK(serialisedRay.size() == 5);
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
    Square serialisedPiece{A3};
    Bitboard occupiedSquares_bb{1};
    occupiedSquares_bb = occupiedSquares_bb << 48;

    // Act
    Bitboard attackRay_bb = yak::attackmap::blockedRay<Direction::NORTH>(serialisedPiece, occupiedSquares_bb);

    // Assert
    std::vector<Square> serialisedRay = yak::bitboard::scanForward(attackRay_bb);
    CHECK(serialisedRay.size() == 4);
    CHECK(serialisedRay[0] == 24);
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
    Square serialisedPiece{F7}; // Ray casting piece on F7
    Bitboard occupiedSquares_bb = 1;
    occupiedSquares_bb = occupiedSquares_bb << 61; // Blocker on F8

    // Act
    Bitboard attackRay_bb = yak::attackmap::blockedRay<Direction::NORTH>(serialisedPiece, occupiedSquares_bb);

    // Assert
    std::vector<Square> serialisedRay = yak::bitboard::scanForward(attackRay_bb);
    CHECK(serialisedRay.size() == 1);
    CHECK(serialisedRay[0] == 61);
}

TEST_CASE("NorthRayAttackMapTest: ReturnsEmptyBitboardForAnySquareOnTheEighthRank") {

    // Assemble
    Square serialisedPieces[8] = { A8, B8, C8, D8, E8, F8, G8, H8 };
    Bitboard occupiedSquares_bb = 0;
    Bitboard attackRay_bb[8];

    constexpr std::array<Bitboard, 64> rayMap = yak::attackmap::rayMap<Direction::NORTH>::value;
    // Act
    for (int i = 0; i < 8; i++) {
        attackRay_bb[i] = rayMap[serialisedPieces[i]];
    }

    // Assert
    CHECK(attackRay_bb[0] == 0);
    CHECK(attackRay_bb[1] == 0);
    CHECK(attackRay_bb[2] == 0);
    CHECK(attackRay_bb[3] == 0);
    CHECK(attackRay_bb[4] == 0);
    CHECK(attackRay_bb[5] == 0);
    CHECK(attackRay_bb[6] == 0);
    CHECK(attackRay_bb[7] == 0);
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
    Square serialisedPiece = A6;
    Bitboard occupiedSquares_bb = 0;

    // Act
    Bitboard attackRay_bb = yak::attackmap::blockedRay<Direction::SOUTH>(serialisedPiece, occupiedSquares_bb);

    // Assert
    std::vector<Square> serialisedRay = yak::bitboard::scanForward(attackRay_bb);
    CHECK(serialisedRay.size() == 5);
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
    Square serialisedPiece = A6;
    Bitboard occupiedSquares_bb = 1;
    occupiedSquares_bb = occupiedSquares_bb << 8;

    // Act
    Bitboard attackRay_bb = yak::attackmap::blockedRay<Direction::SOUTH>(serialisedPiece, occupiedSquares_bb);

    // Assert
    std::vector<Square> serialisedRay = yak::bitboard::scanForward(attackRay_bb);
    CHECK(serialisedRay.size() == 4);
    CHECK(serialisedRay[0] == 8);
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
    Square serialisedPiece = F7;
    Bitboard occupiedSquares_bb = 1;
    occupiedSquares_bb = occupiedSquares_bb << 45; // Blocker on F6

    // Act
    Bitboard attackRay_bb = yak::attackmap::blockedRay<Direction::SOUTH>(serialisedPiece, occupiedSquares_bb);

    // Assert
    std::vector<Square> serialisedRay = yak::bitboard::scanForward(attackRay_bb);
    CHECK(serialisedRay.size() == 1);
    CHECK(serialisedRay[0] == 45);
}

TEST_CASE("SouthRayAttackMapTest: ReturnsEmptyBitboardForAnySquareOnTheFirstRank") {

    // Assemble
    Square serialisedPieces[8] = { A1, B1, C1, D1, E1, F1, H1 };
    Bitboard occupiedSquares_bb = 0;
    Bitboard attackRay_bb[8];
  constexpr std::array<Bitboard, 64> rayMap = yak::attackmap::rayMap<Direction::SOUTH>::value;
    // Act
    for (int i = 0; i < 8; i++) {
        attackRay_bb[i] = rayMap[serialisedPieces[i]];
    }

    // Assert
    CHECK(attackRay_bb[0] == 0);
    CHECK(attackRay_bb[1] == 0);
    CHECK(attackRay_bb[2] == 0);
    CHECK(attackRay_bb[3] == 0);
    CHECK(attackRay_bb[4] == 0);
    CHECK(attackRay_bb[5] == 0);
    CHECK(attackRay_bb[6] == 0);
    CHECK(attackRay_bb[7] == 0);
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
    Square serialisedPiece = A6;
    Bitboard occupiedSquares_bb = 0;

    // Act
    Bitboard attackRay_bb = yak::attackmap::blockedRay<Direction::EAST>(serialisedPiece, occupiedSquares_bb);

    // Assert
    std::vector<Square> serialisedRay = yak::bitboard::scanForward(attackRay_bb);
    CHECK(serialisedRay.size() == 7);
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
    Square serialisedPiece = A6;
    Bitboard occupiedSquares_bb = 1;
    occupiedSquares_bb = occupiedSquares_bb << 45; // Blocker on F6

    // Act
    Bitboard attackRay_bb = yak::attackmap::blockedRay<Direction::EAST>(serialisedPiece, occupiedSquares_bb);

    // Assert
    std::vector<Square> serialisedRay = yak::bitboard::scanForward(attackRay_bb);
    CHECK(serialisedRay.size() == 5);
    CHECK(serialisedRay[0] == 41);
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
    Square serialisedPiece = F7;
    Bitboard occupiedSquares_bb = 1;
    occupiedSquares_bb = occupiedSquares_bb << 54; // Blocker on G7

    // Act
    Bitboard attackRay_bb = yak::attackmap::blockedRay<Direction::EAST>(serialisedPiece, occupiedSquares_bb);

    // Assert
    std::vector<Square> serialisedRay = yak::bitboard::scanForward(attackRay_bb);
    CHECK(serialisedRay.size() == 1);
    CHECK(serialisedRay[0] == 54);
}

TEST_CASE("EastRayAttackMapTest: ReturnsEmptyBitboardForAnySquareOnTheHFile") {

    // Assemble
    Square serialisedPieces[8] = { H1, H2, H3, H4, H5, H6, H7, H8 };
    Bitboard occupiedSquares_bb = 0;
    std::vector<Bitboard>attackRay_bb(8, 0);
  constexpr std::array<Bitboard, 64> rayMap = yak::attackmap::rayMap<Direction::EAST>::value;

    // Act
    for (int i = 0; i < 8; i++) {
        attackRay_bb[i] = rayMap[serialisedPieces[i]];
    }

    // Assert
    for (auto r : attackRay_bb) {
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
    Square serialisedPiece = H6;
    Bitboard occupiedSquares_bb = 0; // No occupied squares

    // Act
    Bitboard attackRay_bb = yak::attackmap::blockedRay<Direction::WEST>(serialisedPiece, occupiedSquares_bb);

    // Assert
    std::vector<Square> serialisedRay = yak::bitboard::scanForward(attackRay_bb);
    CHECK(serialisedRay.size() == 7);
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
    Square serialisedPiece = H6; // Ray casting piece on H6
    Bitboard occupiedSquares_bb = 1;
    occupiedSquares_bb = occupiedSquares_bb << 42; // Blocker on C6

    // Act
    Bitboard attackRay_bb = yak::attackmap::blockedRay<Direction::WEST>(serialisedPiece, occupiedSquares_bb);

    // Assert
    std::vector<Square> serialisedRay = yak::bitboard::scanForward(attackRay_bb);
    CHECK(serialisedRay.size() == 5);
    CHECK(serialisedRay[0] == 42);
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
    Square serialisedPiece = F7; // Ray casting piece on F7
    Bitboard occupiedSquares_bb = 1;
    occupiedSquares_bb = occupiedSquares_bb << 52; // Blocker on E7

    // Act
    Bitboard attackRay_bb = yak::attackmap::blockedRay<Direction::WEST>(serialisedPiece, occupiedSquares_bb);

    // Assert
    std::vector<Square> serialisedRay = yak::bitboard::scanForward(attackRay_bb);
    CHECK(serialisedRay.size() == 1);
    CHECK(serialisedRay[0] == 52);
}

TEST_CASE("WestRayAttackMapTest: ReturnsEmptyBitboardForAnySquareOnTheAFile") {

    // Assemble
    Square serialisedPieces[8] = { A1, A2, A3, A4, A5, A6, A7, A8 };
    Bitboard occupiedSquares_bb = 0;
    Bitboard attackRay_bb[8];
  constexpr std::array<Bitboard, 64> rayMap = yak::attackmap::rayMap<Direction::WEST>::value;
    // Act
    for (int i = 0; i < 8; i++) {
        attackRay_bb[i] = rayMap[serialisedPieces[i]];
    }

    // Assert
    CHECK(attackRay_bb[0] == 0);
    CHECK(attackRay_bb[1] == 0);
    CHECK(attackRay_bb[2] == 0);
    CHECK(attackRay_bb[3] == 0);
    CHECK(attackRay_bb[4] == 0);
    CHECK(attackRay_bb[5] == 0);
    CHECK(attackRay_bb[6] == 0);
    CHECK(attackRay_bb[7] == 0);
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
    Square serialisedPiece = C4;
    Bitboard occupiedSquares_bb = 0; // No occupied squares

    // Act
    Bitboard attackRay_bb = yak::attackmap::blockedRay<Direction::NORTH_EAST>(serialisedPiece, occupiedSquares_bb);

    // Assert
    std::vector<Square> serialisedRay = yak::bitboard::scanForward(attackRay_bb);
    CHECK(serialisedRay.size() == 4);
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
    Square serialisedPiece = C4;
    Bitboard occupiedSquares_bb = 1;
    occupiedSquares_bb = occupiedSquares_bb << 53; // Blocker on F7

    // Act
    Bitboard attackRay_bb = yak::attackmap::blockedRay<Direction::NORTH_EAST>(serialisedPiece, occupiedSquares_bb);

    // Assert
    std::vector<Square> serialisedRay = yak::bitboard::scanForward(attackRay_bb);
    CHECK(serialisedRay.size() == 3);
    CHECK(serialisedRay[0] == 35);
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
    Square serialisedPiece = F7;
    Bitboard occupiedSquares_bb = 1;
    occupiedSquares_bb = occupiedSquares_bb << 62; // Blocker on G8

    // Act
    Bitboard attackRay_bb = yak::attackmap::blockedRay<Direction::NORTH_EAST>(serialisedPiece, occupiedSquares_bb);

    // Assert
    std::vector<Square> serialisedRay = yak::bitboard::scanForward(attackRay_bb);
    CHECK(serialisedRay.size() == 1);
    CHECK(serialisedRay[0] == 62);
}

TEST_CASE("NorthEastRayAttackMapTest: ReturnsEmptyBitboardForAnySquareOnTheHFile") {

  // Assemble
  Square serialisedPieces[8] = { H1, H2, H3, H4, H5, H6, H7, H8 };
  Bitboard occupiedSquares_bb = 0;
  Bitboard attackRay_bb[8];
  constexpr std::array<Bitboard, 64> rayMap = yak::attackmap::rayMap<Direction::NORTH_EAST>::value;

  // Act
  for (int i = 0; i < 8; i++) {
    attackRay_bb[i] = rayMap[serialisedPieces[i]];
  }

  // Assert
  CHECK(attackRay_bb[0] == 0);
  CHECK(attackRay_bb[1] == 0);
  CHECK(attackRay_bb[2] == 0);
  CHECK(attackRay_bb[3] == 0);
  CHECK(attackRay_bb[4] == 0);
  CHECK(attackRay_bb[5] == 0);
  CHECK(attackRay_bb[6] == 0);
  CHECK(attackRay_bb[7] == 0);
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
  Square serialisedPiece = C4; // Ray casting piece on C4
  Bitboard occupiedSquares_bb = 0; // No occupied squares

  // Act
  Bitboard attackRay_bb = yak::attackmap::blockedRay<Direction::SOUTH_EAST>(serialisedPiece, occupiedSquares_bb);

  // Assert
  std::vector<Square> serialisedRay = yak::bitboard::scanForward(attackRay_bb);
  CHECK(serialisedRay.size() == 3);
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
    Square serialisedPiece = C4; // Ray casting piece on C4
    Bitboard occupiedSquares_bb = 1;
    occupiedSquares_bb = occupiedSquares_bb << 12; // Blocker on E2

    // Act
    Bitboard attackRay_bb = yak::attackmap::blockedRay<Direction::SOUTH_EAST>(serialisedPiece, occupiedSquares_bb);

    // Assert
    std::vector<Square> serialisedRay = yak::bitboard::scanForward(attackRay_bb);
    CHECK(serialisedRay.size() == 2);
    CHECK(serialisedRay[0] == 12);
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
    Square serialisedPiece = F7;
    Bitboard occupiedSquares_bb = 1;
    occupiedSquares_bb = occupiedSquares_bb << 46; // Blocker on G6

    // Act
    Bitboard attackRay_bb = yak::attackmap::blockedRay<Direction::SOUTH_EAST>(serialisedPiece, occupiedSquares_bb);

    // Assert
    std::vector<Square> serialisedRay = yak::bitboard::scanForward(attackRay_bb);
    CHECK(serialisedRay.size() == 1);
    CHECK(serialisedRay[0] == 46);
}

TEST_CASE("SouthEastRayAttackMapTest: ReturnsEmptyBitboardForAnySquareOnTheHFile") {

    // Assemble
    Square serialisedPieces[8] = { H1, H2, H3, H4, H5, H6, H7, H8 };
    Bitboard occupiedSquares_bb = 0;
    Bitboard attackRay_bb[8];

  constexpr std::array<Bitboard, 64> rayMap = yak::attackmap::rayMap<Direction::SOUTH_EAST>::value;
  // Act
  for (int i = 0; i < 8; i++) {
    attackRay_bb[i] = rayMap[serialisedPieces[i]];
  }

    // Assert
    CHECK(attackRay_bb[0] == 0);
    CHECK(attackRay_bb[1] == 0);
    CHECK(attackRay_bb[2] == 0);
    CHECK(attackRay_bb[3] == 0);
    CHECK(attackRay_bb[4] == 0);
    CHECK(attackRay_bb[5] == 0);
    CHECK(attackRay_bb[6] == 0);
    CHECK(attackRay_bb[7] == 0);
}

TEST_CASE("NorthWestRayAttackMapTest: CanReturnRayWhenThereAreNoBlockingPieces")
{
  // 1 . . . . . . .
  // . 1 . . . . . .
  // . . 1 . . . . .
  // . . . 1 . . . .
  // . . . . 1 . . .
  // . . . . . x . .
  // . . . . . . . .
  // . . . . . . . .

  // TODO (haigh) Can we use a generate command here?
  Square serialisedPiece{F3};
  Bitboard occupiedSquares_bb{0};
  Bitboard expectedRay_bb{0};

  SECTION("No blocking pieces")
  {
    expectedRay_bb = bitboard::createBitboard(E4, D5, C6, B7, A8);
  }

  SECTION("Single blocking piece")
  {
    expectedRay_bb = bitboard::createBitboard(E4, D5, C6);
    occupiedSquares_bb = bitboard::createBitboard(C6);
  }

  SECTION("Blocking piece is on adjacent square")
  {
    expectedRay_bb = bitboard::createBitboard(E4);
    occupiedSquares_bb = bitboard::createBitboard(E4);
  }

  auto attackRay_bb = attackmap::blockedRay<Direction::NORTH_WEST>(serialisedPiece, occupiedSquares_bb);

  CHECK(attackRay_bb == expectedRay_bb);
}

TEST_CASE("NorthWestRayAttackMapTest: CanReturnRayWhenBlockingPieceIsOnAdjacentSquare")
{
    // . . . . B . . .
    // . . . . . x . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .

    // Assemble
    Square serialisedPiece = F7; // Ray casting piece on F7
    Bitboard occupiedSquares_bb = 1;
    occupiedSquares_bb = occupiedSquares_bb << 60; // Blocker on G8

    // Act
    Bitboard attackRay_bb = yak::attackmap::blockedRay<Direction::NORTH_WEST>(serialisedPiece, occupiedSquares_bb);

    // Assert
    std::vector<Square> serialisedRay = yak::bitboard::scanForward(attackRay_bb);
    CHECK(serialisedRay.size() == 1);
    CHECK(serialisedRay[0] == 60);
}

TEST_CASE("NorthWestRayAttackMapTest: Returns empty bitboard for any square on the A file")
{
  Square serialisedPieces[8] = { A1, A2, A3, A4, A5, A6, A6, A8 };
  Bitboard occupiedSquares_bb = 0;
  Bitboard attackRay_bb[8];

  constexpr std::array<Bitboard, 64> rayMap = yak::attackmap::rayMap<Direction::NORTH_WEST>::value;

  for (int i = 0; i < 8; i++) {
    CHECK(rayMap[serialisedPieces[i]] == 0);
  }
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
    Square serialisedPiece = F7; // Ray casting piece on F7
    Bitboard occupiedSquares_bb = 0; // No occupied squares

    // Act
    Bitboard attackRay_bb = yak::attackmap::blockedRay<Direction::SOUTH_WEST>(serialisedPiece, occupiedSquares_bb);

    // Assert
    std::vector<Square> serialisedRay = yak::bitboard::scanForward(attackRay_bb);
    CHECK(serialisedRay.size() == 5);
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
    Square serialisedPiece = F7; // Ray casting piece on F7
    Bitboard occupiedSquares_bb = 1;
    occupiedSquares_bb = occupiedSquares_bb << 26; // Blocker on C4

    // Act
    Bitboard attackRay_bb = yak::attackmap::blockedRay<Direction::SOUTH_WEST>(serialisedPiece, occupiedSquares_bb);

    // Assert
    std::vector<Square> serialisedRay = yak::bitboard::scanForward(attackRay_bb);
    CHECK(serialisedRay.size() == 3);
    CHECK(serialisedRay[0] == 26);
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
  Square serialisedPiece = F7;
  Bitboard occupiedSquares_bb = bitboard::createBitboard(E6);

  // Act
  Bitboard attackRay_bb = attackmap::blockedRay<Direction::SOUTH_WEST>(serialisedPiece, occupiedSquares_bb);

  // Assert
  std::vector<Square> serialisedRay = yak::bitboard::scanForward(attackRay_bb);
  CHECK(serialisedRay.size() == 1);
  CHECK(serialisedRay[0] == 44);
}

TEST_CASE("SouthWestRayAttackMapTest: ReturnsEmptyBitboardForAnySquareOnTheAFile")
{
  Square serialisedPieces[8] = { A1, A2, A3, A4, A5, A6, A6, A8 };
  Bitboard occupiedSquares_bb{0};
  Bitboard attackRay_bb[8];

  constexpr std::array<Bitboard, 64> rayMap = yak::attackmap::rayMap<Direction::SOUTH_WEST>::value;

  for (int i = 0; i < 8; i++)
  {
    attackRay_bb[i] = rayMap[serialisedPieces[i]];
  }

  CHECK(attackRay_bb[0] == 0);
  CHECK(attackRay_bb[1] == 0);
  CHECK(attackRay_bb[2] == 0);
  CHECK(attackRay_bb[3] == 0);
  CHECK(attackRay_bb[4] == 0);
  CHECK(attackRay_bb[5] == 0);
  CHECK(attackRay_bb[6] == 0);
  CHECK(attackRay_bb[7] == 0);
}

} // namespace yak::attackmap
