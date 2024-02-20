#include <catch2/catch_test_macros.hpp>
#include <bitboard.h>
#include "types.h"

namespace yak { namespace bitboard {

TEST_CASE("Shift a bitboard by one square")
{
  const Bitboard initial_bb = static_bitboard<E4>::value;
  Bitboard shifted_bb, expected_bb;

  SECTION("North")
  {
    expected_bb = static_bitboard<E5>::value; // one square north
    shifted_bb = shift<Direction::NORTH>(initial_bb);
  }

  SECTION("North East")
  {
    expected_bb = static_bitboard<F5>::value; // one square north east
    shifted_bb = shift<Direction::NORTH_EAST>(initial_bb);
  }

  SECTION("East")
  {
    expected_bb = static_bitboard<F4>::value; // one square east
    shifted_bb = shift<Direction::EAST>(initial_bb);
  }

  SECTION("South East")
  {
    expected_bb = static_bitboard<F3>::value; // one square south east
    shifted_bb = shift<Direction::SOUTH_EAST>(initial_bb);
  }

  SECTION("South")
  {
    expected_bb = static_bitboard<E3>::value;   // one square south
    shifted_bb = shift<Direction::SOUTH>(initial_bb);
  }

  SECTION("South West")
  {
    expected_bb = static_bitboard<D3>::value;   // one square south west
    shifted_bb = shift<Direction::SOUTH_WEST>(initial_bb);
  }

  SECTION("West")
  {
    expected_bb = static_bitboard<D4>::value;   // one square west
    shifted_bb = shift<Direction::WEST>(initial_bb);
  }

  SECTION("North West")
  {
    expected_bb = static_bitboard<D5>::value;   // one square north west
    shifted_bb = shift<Direction::NORTH_WEST>(initial_bb);
  }

  INFO("Checking that shifted bb:\n" << bitboard::to_string(shifted_bb) << "\n\nis equal to expected bb:\n" << bitboard::to_string(expected_bb) << "\n");
  CHECK(shifted_bb == expected_bb);
}

TEST_CASE("BitboardTest: Shifting off the board")
{
  CHECK(shift<Direction::SOUTH>(RANK_1) == Bitboard{0ULL});
  CHECK(shift<Direction::NORTH>(RANK_8) == Bitboard{0ULL});
  CHECK(shift<Direction::WEST>(FILE_A) == Bitboard{0ULL});
  CHECK(shift<Direction::EAST>(FILE_H) == Bitboard{0ULL});
}

TEST_CASE("BitboardTest: NorthOne")
{
  // Arrange
  Bitboard initial_bitboard = 0x000000000000ff00; // RANK 2
  Bitboard shifted_bitboard = 0;
  Bitboard expected_bitboard = 0x0000000000ff0000; // RANK 3

  // Act
  shifted_bitboard = shift<Direction::NORTH>(initial_bitboard);

  // Assert
  CHECK(shifted_bitboard == expected_bitboard);
}

TEST_CASE("BitboardTest: EastOne")
{
  // Arrange
  Bitboard initial_bitboard = 0x0202020202020202; // B FILE
  Bitboard shifted_bitboard = 0;
  Bitboard expected_bitboard = 0x0404040404040404; // C FILE

  // Act
  shifted_bitboard = shift<Direction::EAST>(initial_bitboard);

  // Assert
  CHECK(shifted_bitboard == expected_bitboard);
}

TEST_CASE("BitboardTest: NorthEastOneFromMiddleOfBoard")
{
  CHECK(shift<Direction::WEST>(FILE_B) == FILE_A);


  CHECK(shift<Direction::NORTH_EAST>(static_bitboard<A2>::value) == static_bitboard<B3>::value);

  CHECK(shift<Direction::NORTH_WEST>(FILE_A) == EMPTY);
  CHECK(shift<Direction::SOUTH_WEST>(FILE_A) == EMPTY);


  CHECK(shift<Direction::SOUTH_EAST>(static_bitboard<A2>::value) == static_bitboard<B1>::value);
}

TEST_CASE("BitboardTest: PopMS1B")
{
  // Arrange
  Bitboard initial = 0xff00000000000000;
  Bitboard expected = 0x7f00000000000000;

  // Act
  Square MS1B_index = popMS1B(initial);

  // Assert
  CHECK(MS1B_index == 63);
  CHECK(initial == expected);
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
  // Arrange
  Bitboard initial = 0xff00000000000000;
  Bitboard expected = 0xfe00000000000000;

  // Act
  Square MS1B_index = popLS1B(initial);

  // Assert
  CHECK(MS1B_index == 56);
  CHECK(initial == expected);
}

TEST_CASE("BitboardTest: Scan forward on empty board returns empty vector") {
    // Arrange
    Bitboard initial_bitboard = 0;
    // Act
    std::vector<Square> serialised_bitboard = scanForward(initial_bitboard);

    // Assert
    CHECK(serialised_bitboard.empty());
}

TEST_CASE("BitboardTest: ScanForwardForPieceOnE4") {
    // Arrange
    Bitboard initial_bitboard = static_bitboard<E4>::value;

    // Act
    std::vector<Square> serialised_bitboard = scanForward(initial_bitboard);

    // Assert
    CHECK(serialised_bitboard[0] == E4);
}

TEST_CASE("BitboardTest: ScanBackwardOnEmptyBoardReturnsEmptyStack") {
    // Arrange
    Bitboard initial_bitboard = 0;

    // Act
    std::vector<Square> serialised_bitboard = scanBackward(initial_bitboard);

    // Assert
    CHECK(serialised_bitboard.size() == 0);
}

TEST_CASE("BitboardTest: ScanBackwardForPieceOnE4") {
    // Arrange
    Bitboard initial_bitboard = static_bitboard<E4>::value;

    // Act
    std::vector<Square> serialised_bitboard = scanBackward(initial_bitboard);

    // Assert
    CHECK(serialised_bitboard[0] == E4);
}

TEST_CASE("BitboardTest: CanGetFileIndexOfSquareOnTheBoard") {
    // Arrange
    Square square_index = H8;
    File expected_file = 7;
    File file_of_a8;

    // Act
    file_of_a8 = fileIndex(square_index);

    // Assert
    CHECK(file_of_a8 == expected_file);
}

TEST_CASE("BitboardTest: FileIndexReturnsMinusOneWhenSquareIndexIsOutOfRange") {
    // Arrange
    Square square_index = static_cast<Square>(65);
    File fileOfInvalidSquare;

    // Act
    fileOfInvalidSquare = fileIndex(square_index);

    // Assert
    CHECK(fileOfInvalidSquare == -1);
}

TEST_CASE("BitboardTest: CanGetRankIndexOfSquareOnTheBoard") {
    // Arrange
    Square square_index = H1;
    Rank expected_rank = 0; // First Rank
    Rank rank_of_a8;

    // Act
    rank_of_a8 = rankIndex(square_index);

    // Assert
    CHECK(rank_of_a8 == expected_rank);
}

TEST_CASE("BitboardTest: RankIndexReturnsMinusOneWhenSquareIndexIsOutOfRange") {
    // Arrange
    Square square_index = static_cast<Square>(65);
    Rank rankOfInvalidSquare;

    // Act
    rankOfInvalidSquare = rankIndex(square_index);

    // Assert
    CHECK(rankOfInvalidSquare == -1);
}

TEST_CASE("BitboardTest: CanGetSquareIndexFromVaildRankAndFileIndices") {
    // Arrange
    File validFile = 7;
    Rank validRank = 0;
    Square expectedSquare = H1;
    Square validSquare;

    // Act
    validSquare = squareIndex(validFile, validRank);

    // Assert
    CHECK(validSquare == expectedSquare);
}

TEST_CASE("BitboardTest: SetSquareCanCorrectlySetAnUnsetSquareOnBitboard") {
  // Arrange
  Square square{A1};
  Bitboard expected{1};
  Bitboard actual{0};

  // Act
  setSquare(actual, square);

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("BitboardTest: SetSquareCanCorrectlySetAnUnsetSquareOnBitboardWithRankAndFile") {
  // Arrange
  Rank rank{ 0 };
  File file{ 0 };
  Bitboard expected{ 1 };
  Bitboard actual{ 0 };

  // Act
  setSquare(actual, rank, file);

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("BitboardTest: CanConvertAlgebraicSquareNotationToSquareIndex") {
    // Arrange
    Square expected{A1}; // Start at A1
    Square actual[8][8]{A1};
    char ranks[8]{'1', '2', '3', '4', '5', '6', '7', '8'};
    char files[8]{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

    // Act
    for (int iRank = 0; iRank < 8; ++iRank) {
        for (int iFile = 0; iFile < 8; ++iFile) {
            std::string algebraic_square{ files[iFile], ranks[iRank] };
            actual[iRank][iFile] = squareIndex(algebraic_square);
        }
    }

    // Assert
    for (int iRank = 0; iRank < 8; ++iRank) {
        for (int iFile = 0; iFile < 8; ++iFile) {
            CHECK(actual[iRank][iFile] == expected);
            expected = static_cast<Square>(expected + 1);
        }
    }
}

TEST_CASE("BitboardTest: CanConvertAlgebraicSquareNotationToSquareIndexWithCapitalFile") {
    // Arrange
    Square expected{A1}; // Start at A1
    Square actual[8][8]{A1};
    char ranks[8]{ '1', '2', '3', '4', '5', '6', '7', '8' };
    char files[8]{ 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };

    // Act
    for (int iRank = 0; iRank < 8; ++iRank) {
        for (int iFile = 0; iFile < 8; ++iFile) {
            std::string algebraic_square{ files[iFile], ranks[iRank] };
            actual[iRank][iFile] = squareIndex(algebraic_square);
        }
    }

    // Assert
    for (int iRank = 0; iRank < 8; ++iRank) {
        for (int iFile = 0; iFile < 8; ++iFile) {
            CHECK(actual[iRank][iFile] == expected);
            expected = static_cast<Square>(expected + 1);
        }
    }
}

TEST_CASE("BitboardTest: CanConvertSquareIndexToAlgebraicSquareNotation") {
    // Arrange
    Square square{ A1 };
    std::vector<std::string> expected{ "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
        "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
        "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
        "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
        "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
        "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
        "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
        "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8" };

    // Act & Assert
    for (int i = 0; i < 64; i++) {
        std::string actual = toAlgebraic(static_cast<Square>(i));
        CHECK(actual == expected[i]);
    }
}

TEST_CASE("StaticBitboardTest: MultiPieceBitboardGeneratesCorrectly") {
    // Arrange
    Bitboard expected = RANK_1;

    // Act
    Bitboard actual = static_bitboard<A1, B1, C1, D1, E1, F1, G1, H1>::value;

    // Assert
    INFO("Checking actual bb:\n" << bitboard::to_string(actual) << "\n\nis equal to expected:\n" << bitboard::to_string(expected) << "\n");
    CHECK(actual == expected);
}

} // namespace yak
} // namespace bitboard
