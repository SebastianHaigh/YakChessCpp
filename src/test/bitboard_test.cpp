#include <catch2/catch_test_macros.hpp>

#include <bitboard.h>
#include <types.h>

#include <array>
#include <string>
#include <vector>

namespace yak::bitboard {

TEST_CASE("BitboardTest: NorthEastOneFromMiddleOfBoard")
{
  CHECK(shift<Direction::NORTH>(static_bitboard<E4>::value) == static_bitboard<E5>::value);
  CHECK(shift<Direction::NORTH_EAST>(static_bitboard<E4>::value) == static_bitboard<F5>::value);
  CHECK(shift<Direction::EAST>(static_bitboard<E4>::value) == static_bitboard<F4>::value);
  CHECK(shift<Direction::SOUTH_EAST>(static_bitboard<E4>::value) == static_bitboard<F3>::value);
  CHECK(shift<Direction::SOUTH>(static_bitboard<E4>::value) == static_bitboard<E3>::value);
  CHECK(shift<Direction::SOUTH_WEST>(static_bitboard<E4>::value) == static_bitboard<D3>::value);
  CHECK(shift<Direction::WEST>(static_bitboard<E4>::value) == static_bitboard<D4>::value);
  CHECK(shift<Direction::NORTH_WEST>(static_bitboard<E4>::value) == static_bitboard<D5>::value);

  CHECK(shift<Direction::NORTH>(RANK_2) == RANK_3);

  CHECK(shift<Direction::WEST>(FILE_B) == FILE_A);

  CHECK(shift<Direction::NORTH_EAST>(static_bitboard<A2>::value) == static_bitboard<B3>::value);

  CHECK(shift<Direction::NORTH_WEST>(FILE_A) == EMPTY);
  CHECK(shift<Direction::SOUTH_WEST>(FILE_A) == EMPTY);

  CHECK(shift<Direction::EAST>(FILE_B) == FILE_C);

  CHECK(shift<Direction::SOUTH_EAST>(static_bitboard<A2>::value) == static_bitboard<B1>::value);

  CHECK(shift<Direction::SOUTH>(RANK_1) == Bitboard{0ULL});
  CHECK(shift<Direction::NORTH>(RANK_8) == Bitboard{0ULL});
  CHECK(shift<Direction::WEST>(FILE_A) == Bitboard{0ULL});
  CHECK(shift<Direction::EAST>(FILE_H) == Bitboard{0ULL});
}

TEST_CASE("BitboardTest: PopMS1B")
{
  Bitboard initial_bb{ RANK_8 };

  const std::array<Bitboard, 8> expected_bb = {
    createBitboard(A8, B8, C8, D8, E8, F8, G8),
    createBitboard(A8, B8, C8, D8, E8, F8),
    createBitboard(A8, B8, C8, D8, E8),
    createBitboard(A8, B8, C8, D8),
    createBitboard(A8, B8, C8),
    createBitboard(A8, B8),
    createBitboard(A8),
    EMPTY};

  int expectedIndex{ 63 };

  for (const auto& bb : expected_bb)
  {
    CHECK(popMS1B(initial_bb) == expectedIndex);
    CHECK(initial_bb == bb);

    --expectedIndex;
  }
}

TEST_CASE("BitboardTest: Pop MS1B with value one")
{
  // Arrange
  Bitboard initial =  0x0000000000000001;
  Bitboard expected = 0x0000000000000000;

  // Act
  Square MS1B_index = popMS1B(initial);

  // Assert
  CHECK(MS1B_index == 0);
  CHECK(initial == expected);
}

TEST_CASE("BitboardTest: Pop MS1B with only one set bit")
{
  // Arrange
  Bitboard expected = 0x0000000000000000;
  std::vector<Bitboard> actual;

  // Act
  for (int i = 0; i < 63; i++) {
    Bitboard one = Bitboard{ 1 } << i;
    Square MS1B_index = popMS1B(one);
    actual.push_back(one);
  }

  // Assert
  for (auto board : actual)
  {
    CHECK(board == expected);
  }
}

TEST_CASE("BitboardTest: Pop LS1B")
{
  Bitboard initial_bb{ RANK_8 };

  const std::array<Bitboard, 8> expected_bb = {
    createBitboard(B8, C8, D8, E8, F8, G8, H8),
    createBitboard(C8, D8, E8, F8, G8, H8),
    createBitboard(D8, E8, F8, G8, H8),
    createBitboard(E8, F8, G8, H8),
    createBitboard(F8, G8, H8),
    createBitboard(G8, H8),
    createBitboard(H8),
    EMPTY};

  int expectedIndex{ 56 };

  for (const auto& bb : expected_bb)
  {
    CHECK(popLS1B(initial_bb) == expectedIndex);
    CHECK(initial_bb == bb);

    ++expectedIndex;
  }
}

TEST_CASE("BitboardTest: Scanning empty board returns empty vector")
{
  Bitboard initial_bb = EMPTY;
  std::vector<Square> serialised;

  SECTION("Scan forward")
  {
    serialised = scanForward(initial_bb);
  }

  SECTION("Scan backward")
  {
    serialised = scanBackward(initial_bb);
  }

  CHECK(serialised.empty());
}

TEST_CASE("BitboardTest: Scan bitboard with single piece on E4")
{
  Bitboard initial_bb{ static_bitboard<E4>::value };

  std::vector<Square> serialised;

  SECTION("Scan forward")
  {
    serialised = scanForward(initial_bb);
  }

  SECTION("Scan backward")
  {
    serialised = scanBackward(initial_bb);
  }

  // Assert
  REQUIRE_FALSE(serialised.empty());
  CHECK(serialised.front() == E4);
}

TEST_CASE("BitboardTest: Get rank and file indexes of square")
{
  File expectedFile{ 7 };
  Rank expectedRank{ 7 };

  CHECK(fileIndex(H8) == expectedFile);
  CHECK(rankIndex(H8) == expectedRank);
}

TEST_CASE("BitboardTest: Rank and file indexes returns minus one when square index is out of range")
{
  CHECK(fileIndex(NULL_SQUARE) == -1);
  CHECK(rankIndex(NULL_SQUARE) == -1);
}

TEST_CASE("BitboardTest: Get square index from vaild rank and file indices")
{
  File validFile{ 7 };
  Rank validRank{ 0 };

  CHECK(squareIndex(validFile, validRank) == H1);
}

TEST_CASE("BitboardTest: Set and unset a square on bitboard")
{
  Bitboard expected_bb{ 1 };
  Bitboard board_bb{ 0 };

  SECTION("From square")
  {
    setSquare(board_bb, A1);
  }

  SECTION("From rank and file")
  {
    setSquare(board_bb, 0, 0);
  }

  CHECK(board_bb == expected_bb);
}

TEST_CASE("BitboardTest: Convert algebraic square notation to square index")
{
  Square expected{ A1 }; // Start at A1
  Square actual[8][8];
  std::string ranks{"12345678"};
  std::string files;

  SECTION("Lower case letters")
  {
    files = "abcdefgh";
  }

  SECTION("Upper case letters")
  {
    files = "ABCDEFGH";
  }

  for (int iRank = 0; iRank < 8; ++iRank)
  {
    for (int iFile = 0; iFile < 8; ++iFile)
    {
      actual[iRank][iFile] = squareIndex(std::string{ files[iFile], ranks[iRank] });
    }
  }

  for (int iRank = 0; iRank < 8; ++iRank)
  {
    for (int iFile = 0; iFile < 8; ++iFile)
    {
      CHECK(actual[iRank][iFile] == expected);
      expected = static_cast<Square>(expected + 1);
    }
  }
}

TEST_CASE("BitboardTest: Convert square index to algebraic square notation")
{
  Square square{ A1 };
  std::vector<std::string> expected{ "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8" };

  for (int i = 0; i < 64; ++i)
  {
    std::string actual = toAlgebraic(static_cast<Square>(i));
    CHECK(actual == expected[i]);
  }
}

TEST_CASE("StaticBitboardTest: Multi piece static bitboard generates correctly")
{
  CHECK(static_bitboard<A1, B1, C1, D1, E1, F1, G1, H1>::value == RANK_1);
}

} // namespace yak::bitboard
