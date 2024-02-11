#include <catch2/catch_test_macros.hpp>

#include <memory>
#include <vector>
#include <utility>

#include <pieces.h>
#include <bitboard.h>
#include "generation.h"
#include <types.h>

namespace yak {

TEST_CASE("BlackPawnsTest: CanGenerateSinglePushesFromUnobstructedStartingPosition")
{
  // Assemble
  Bitboard pawns = bitboard::RANK_7;
  Move moveList[20];
  int moveCounter{ 0 };

  // Act
  move::generatePawnSinglePushes<PieceColour::BLACK, false>(&moveList[moveCounter], moveCounter, pawns, ~pawns);

  // Assert
  CHECK(moveCounter == 8);
}

TEST_CASE("BlackPawnsTest: Double push target is rank 5")
{
  CHECK(pawns::pawnDoublePushTarget<PieceColour::BLACK>() == bitboard::RANK_5);
}

TEST_CASE("BlackPawnsTest: DoublePushOnlyAllowedFromRank7")
{
  // Assemble
  Bitboard pawns = yak::bitboard::RANK_1;
  Move moveList[20];
  int moveCounter{ 0 };
  int actual[8]{ 0 };

  // Act
  for (int i = 0; i < 8; i++)
  {
    moveCounter = 0;
    move::generatePawnDoublePushes<PieceColour::BLACK, false>(&moveList[moveCounter],
                                                              moveCounter,
                                                              pawns,
                                                              ~pawns);
    actual[i] = moveCounter;
    pawns = yak::bitboard::shift<Direction::NORTH>(pawns);
  }


  // Assert
  CHECK(actual[0] == 0); // RANK 1, No moves
  CHECK(actual[1] == 0); // RANK 2, No moves
  CHECK(actual[2] == 0); // RANK 3, No moves
  CHECK(actual[3] == 0); // RANK 4, No moves
  CHECK(actual[4] == 0); // RANK 5, No moves
  CHECK(actual[5] == 0); // RANK 6, No moves
  CHECK(actual[6] == 8); // RANK 7, Eight moves
  CHECK(actual[7] == 0); // RANK 8, No moves
}

TEST_CASE("BlackPawnsTest: Friendly piece blocks pawn pushes")
{
  // Tests if an friendly piece can block pawn pushes.

  //   Test Board           Sources           Targets
  // . . . . . . . .    . . . . . . . .    . . . . . . . .
  // p p p p p p p p    1 1 . 1 1 1 1 1    . . . . . . . .
  // . . b . . . . .    . . . . . . . .    1 1 . 1 1 1 1 1
  // . . . . . . . .    . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .    . . . . . . . .

  // Assemble
  auto pawnBitboard = bitboard::RANK_7;
  auto extraPiece = bitboard::static_bitboard<C6>::value;
  auto emptySquares_bb = ~(pawnBitboard ^ extraPiece);
  Move moveList[20];
  int moveCounter{ 0 };

  // Act
  move::generatePawnSinglePushes<PieceColour::BLACK, false>(&moveList[moveCounter],
                                                            moveCounter,
                                                            pawnBitboard,
                                                            emptySquares_bb);

  // Assert
  CHECK(moveCounter == 7);
}

TEST_CASE("BlackPawnsTest: WestAttackSources")
{
    // Arrange
    Bitboard target = bitboard::static_bitboard<C6>::value;
    Bitboard expected = bitboard::static_bitboard<D7>::value;

    // Act
    Bitboard actual = pawns::pawnWestAttackSource<PieceColour::BLACK>(target);

    // Assert
    CHECK(actual == expected);
}

TEST_CASE("BlackPawnsTest: WestAttackTarget")
{
    // Arrange
    Bitboard source = yak::bitboard::toBitboard("e6");
    Bitboard expected = yak::bitboard::toBitboard("d5");

    // Act
    Bitboard actual = pawns::pawnWestAttackTarget<PieceColour::BLACK>(source);

    // Assert
    CHECK(actual == expected);
}

TEST_CASE("BlackPawnsTest: EastAttackSources")
{
    // Arrange
    Bitboard target = yak::bitboard::toBitboard("c6");
    Bitboard expected = yak::bitboard::toBitboard("b7");

    // Act
    Bitboard actual = pawns::pawnEastAttackSource<PieceColour::BLACK>(target);

    // Assert
    CHECK(actual == expected);
}

TEST_CASE("BlackPawnsTest: EastAttackTarget")
{
    // Arrange
    Bitboard source = yak::bitboard::toBitboard("e6");
    Bitboard expected = yak::bitboard::toBitboard("f5");

    // Act
    Bitboard actual = pawns::pawnEastAttackTarget<PieceColour::BLACK>(source);

    // Assert
    CHECK(actual == expected);
}

TEST_CASE("WhitePawnsTest: WestAttackSources")
{
    // Arrange
    Bitboard target = yak::bitboard::toBitboard("c6");
    Bitboard expected = yak::bitboard::toBitboard("d5");

    // Act
    Bitboard actual = pawns::pawnWestAttackSource<PieceColour::WHITE>(target);

    // Assert
    CHECK(actual == expected);
}

TEST_CASE("WhitePawnsTest: WestAttackTarget")
{
    // Arrange
    Bitboard source = yak::bitboard::toBitboard("e6");
    Bitboard expected = yak::bitboard::toBitboard("d7");

    // Act
    Bitboard actual = pawns::pawnWestAttackTarget<PieceColour::WHITE>(source);

    // Assert
    CHECK(actual == expected);
}

TEST_CASE("WhitePawnsTest: EastAttackSources")
{
    // Arrange
    Bitboard target = yak::bitboard::toBitboard("c6");
    Bitboard expected = yak::bitboard::toBitboard("b5");

    // Act
    Bitboard actual = pawns::pawnEastAttackSource<PieceColour::WHITE>(target);

    // Assert
    CHECK(actual == expected);
}

TEST_CASE("WhitePawnsTest: EastAttackTarget")
{
  // Arrange
  Bitboard source = yak::bitboard::toBitboard("e6");
  Bitboard expected = yak::bitboard::toBitboard("f7");

  // Act
  Bitboard actual = pawns::pawnEastAttackTarget<PieceColour::WHITE>(source);

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("BlackPawnsTest: OpponentPieceCanBeCaptured") {
  // Tests if the white pawns on their starting squares can return valid
  // capture moves for the situation on the board shown below.

  //   Test Board           Sources           Targets
  // . . . . . . . .    . . . . . . . .    . . . . . . . .
  // p p p p p p p p    . 1 . 1 . . . .    . . . . . . . .
  // . . B . . . . .    . . . . . . . .    . . 1 . . . . .
  // . . . . . . . .    . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .    . . . . . . . .

  // Assemble
  Bitboard pawn_bitboard = yak::bitboard::RANK_7;
  Bitboard opponentPieces_bb = yak::bitboard::RANK_6 & yak::bitboard::FILE_C;

  pawns::pawnWestAttackSource<PieceColour::BLACK>(opponentPieces_bb);
  // Act
  // auto west_moves = pawns.west_captures(pawn_bitboard, opponentPieces_bb);
  //auto east_moves = pawns.east_captures(pawn_bitboard, opponentPieces_bb);

  // Assert
  //CHECK(west_moves.size() == 1);
  //CHECK(east_moves.size() == 1);
}

TEST_CASE("WhitePawnTests: CanGenerateSinglePushesFromUnobstructedStartingPosition") {
    // Assemble
    Bitboard pawns = yak::bitboard::RANK_2;
    Move moveList[20];
    int moveCounter{ 0 };

    // Act
    move::generatePawnSinglePushes<PieceColour::WHITE, false>(&moveList[moveCounter],
                                                              moveCounter,
                                                              pawns,
                                                              ~pawns);

    // Assert

    CHECK(moveCounter == 8);
}

TEST_CASE("WhitePawnTests: CanGenerateDoublePushesFromUnobstructedStartingPosition") {
    // Assemble
    Bitboard pawns = yak::bitboard::RANK_2;
    Move moveList[20];
    int moveCounter{ 0 };

    // Act
    move::generatePawnSinglePushes<PieceColour::WHITE, false>(&moveList[moveCounter],
                                                              moveCounter,
                                                              pawns,
                                                              ~pawns);

    // Assert
    CHECK(moveCounter == 8);
}

TEST_CASE("WhitePawnTests: PieceObstructsSinglePush")
{
    // Tests if a piece can block pawn pushes.

    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . . . . . . .
    // . . B . . . . .
    // P P P P P P P P
    // . . . . . . . .

    // Arrange
    Bitboard pawns = yak::bitboard::RANK_2;
    Bitboard extra_piece = yak::bitboard::static_bitboard<C3>::value;
    Bitboard emptySquares_bb = ~(pawns ^ extra_piece);
    Move moveList[20];
    int moveCounter{ 0 };

    // Act
    move::generatePawnSinglePushes<PieceColour::WHITE, false>(&moveList[moveCounter], moveCounter, pawns, emptySquares_bb);

    // Assert
    CHECK(moveCounter == 7);

}

TEST_CASE("WhitePawnTests: PiecesObstructsDoublePush")
{
  // Tests if pieces can block pawn double pushes.

  // . . . . . . . .
  // . . . . . . . .
  // . . . . . . . .
  // . . . . . . . .
  // . . . . R . . .
  // . . B . . . . .
  // P P P P P P P P
  // . . . . . . . .

  // Arrange
  Bitboard pawns = yak::bitboard::RANK_2;
  Bitboard extra_pieces = yak::bitboard::static_bitboard<C3, E4>::value;
  Bitboard emptySquares_bb = ~(pawns ^ extra_pieces);
  Move moveList[20];
  int moveCounter{ 0 };

  // Act
  move::generatePawnDoublePushes<PieceColour::WHITE, false>(&moveList[moveCounter],
                                                            moveCounter,
                                                            pawns,
                                                            emptySquares_bb);

  // Assert
  CHECK(moveCounter == 6);
}

TEST_CASE("WhitePawnTests: OpponentPieceCanBeCaptured") {
  // Tests if the white pawns on their starting squares can return valid
  // capture moves for the situation on the board shown below.

  // . . . . . . . .
  // . . . . . . . .
  // . . . . . . . .
  // . . . . . . . .
  // . . . . . . . .
  // . . b . . . . .
  // P P P P P P P P
  // . . . . . . . .

  // Assemble
  Bitboard pawns = yak::bitboard::RANK_2;
  Bitboard opponentPieces_bb = yak::bitboard::static_bitboard<C3>::value;
  Move moveList[20];
  int moveCounter{ 0 };

  // Act
  move::generatePawnEastCaptures<PieceColour::WHITE, false>(&moveList[moveCounter],
                                                            moveCounter,
                                                            pawns,
                                                            opponentPieces_bb);

  move::generatePawnWestCaptures<PieceColour::WHITE, false>(&moveList[moveCounter],
                                                            moveCounter,
                                                            pawns,
                                                            opponentPieces_bb);

  // Assert
  CHECK(moveCounter == 2);
}

TEST_CASE("KnightTests: KnightOnA1AttacksTheCorrectSquares") {
  // Arrange
  Move moveList[50];
  int moveCounter{ 0 };
  Bitboard knights = yak::bitboard::static_bitboard<A1>::value;
  Bitboard emptySquares_bb = yak::bitboard::UNIVERSAL;
  Bitboard opponentPieces_bb = yak::bitboard::EMPTY;

  // Act
  moveCounter += move::generatePieceMoves<PieceType::KNIGHT>(&moveList[0],
                                                             knights,
                                                             emptySquares_bb,
                                                             opponentPieces_bb);

  // Assert
  CHECK(moveCounter == 2);
  CHECK(moveList[0].to == C2);
  CHECK(moveList[1].to == B3);
}

TEST_CASE("KnightTests: KnightOnD4AttacksTheCorrectSquares")
{
    // Arrange
    Move moveList[50];
    int moveCounter{ 0 };
    Bitboard knights = yak::bitboard::static_bitboard<D4>::value;
    Bitboard emptySquares_bb = yak::bitboard::UNIVERSAL;
    Bitboard opponentPieces_bb = yak::bitboard::EMPTY;

    // Act
    moveCounter += move::generatePieceMoves<PieceType::KNIGHT>(&moveList[0],
                                                               knights,
                                                               emptySquares_bb,
                                                               opponentPieces_bb);

    // Assert
    CHECK(moveCounter == 8);
    CHECK(moveList[0].to == C2);
    CHECK(moveList[1].to == E2);
    CHECK(moveList[2].to == B3);
    CHECK(moveList[3].to == F3);
    CHECK(moveList[4].to == B5);
    CHECK(moveList[5].to == F5);
    CHECK(moveList[6].to == C6);
    CHECK(moveList[7].to == E6);
}

TEST_CASE("KingTests: KnightOnA1AttacksTheCorrectSquares") {
    // Arrange
    Square startingSquare{ A1 };
    Move moveList[50];
    int moveCounter{ 0 };
    Bitboard emptySquares_bb = yak::bitboard::UNIVERSAL;
    Bitboard opponentPieces_bb = yak::bitboard::EMPTY;

    // Act
    moveCounter += move::generatePieceMoves<PieceType::KING>(&moveList[0],
                                                             yak::bitboard::toBitboard(startingSquare),
                                                             emptySquares_bb,
                                                             opponentPieces_bb);

    // Assert
    CHECK(moveCounter == 3);
    CHECK(moveList[0].to == B1);
    CHECK(moveList[1].to == A2);
    CHECK(moveList[2].to == B2);
}

TEST_CASE("KingTests: KnightOnD4AttacksTheCorrectSquares") {
    // Arrange
    Square startingSquare{ D4 };
    Move moveList[50];
    int moveCounter{ 0 };
    Bitboard emptySquares_bb = yak::bitboard::UNIVERSAL;
    Bitboard opponentPieces_bb = yak::bitboard::EMPTY;

    // Act
    moveCounter += move::generatePieceMoves<PieceType::KING>(&moveList[0],
                                                             yak::bitboard::toBitboard(startingSquare),
                                                             emptySquares_bb,
                                                             opponentPieces_bb);

    // Assert
    CHECK(moveCounter == 8);
    CHECK(moveList[0].to == C3);
    CHECK(moveList[1].to == D3);
    CHECK(moveList[2].to == E3);
    CHECK(moveList[3].to == C4);
    CHECK(moveList[4].to == E4);
    CHECK(moveList[5].to == C5);
    CHECK(moveList[6].to == D5);
    CHECK(moveList[7].to == E5);
}

TEST_CASE("RookTests: RookAttacksProperly") {
    // Arrange
    yak::attackmap::RookMap attackMap;
    Move list[100];
    int moveCounter{ 0 };
    Bitboard rook_bb = yak::bitboard::static_bitboard<A1>::value;
    Bitboard opponentPieces = yak::bitboard::static_bitboard<A5>::value;
    auto emptySquares_bb = ~(rook_bb | opponentPieces);

    // Act
    moveCounter += move::generatePieceMoves<PieceType::ROOK>(&list[0],
                                                             rook_bb,
                                                             emptySquares_bb,
                                                             opponentPieces);

    // Assert
    CHECK(moveCounter == 11);
}

TEST_CASE("QueenTests: CanCalculateQueenAttackBitboard") {
    // Arrange
    yak::attackmap::RookMap attackMap;
    Move list[100];
    int moveCounter{ 0 };
    Bitboard queen_bb = yak::bitboard::static_bitboard<A1>::value;
    Bitboard opponentPieces_bb = yak::bitboard::static_bitboard<A8>::value;
    Bitboard friendlyPieces_bb = queen_bb | yak::bitboard::static_bitboard<H8>::value;
    Bitboard occupied = opponentPieces_bb | friendlyPieces_bb;
    Bitboard expected = (yak::bitboard::FILE_A | yak::bitboard::RANK_1 | yak::bitboard::DIAG_A1_H8) & ~queen_bb;

    // Act
    Bitboard actual = piece::pieceAttacks<PieceType::QUEEN>(queen_bb, occupied);

    // Assert
    CHECK(actual == expected);
}

TEST_CASE("BishopTests: CanCalculateBishopAttackBitboardWithTwoBishops") {
    // Arrange
    Bitboard bishop_bb = bitboard::static_bitboard<A1, A8>::value;
    Bitboard opponentPieces_bb = bitboard::static_bitboard<A8>::value;
    Bitboard friendlyPieces_bb = bishop_bb | bitboard::static_bitboard<H8>::value;
    Bitboard occupied = opponentPieces_bb | friendlyPieces_bb;
    Bitboard expected = (bitboard::DIAG_A8_H1 | bitboard::DIAG_A1_H8) & bitboard::NOT_FILE_A;

    // Act
    Bitboard actual = piece::pieceAttacks<PieceType::BISHOP>(bishop_bb, occupied);

    // Assert
    CHECK(actual == expected);
}
TEST_CASE("FunctionTests: FenCharToPieceTypeTestForBlackPieces") {
    // Arrange
    std::string fenChars{ "pnbrqkx" };
    PieceType expected[7]{ PieceType::PAWN, PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK, PieceType::QUEEN, PieceType::KING, PieceType::NULL_PIECE };
    PieceType actual[7]{ PieceType::NULL_PIECE };

    // Act
    for (int i = 0; i < 7; i++) {
        actual[i] = piece::fenCharToPieceType(fenChars[i]);
    }

    // Assert
    for (int i = 0; i < 7; i++) {
        CHECK(actual[i] == expected[i]);
    }
}

TEST_CASE("FunctionTests: FenCharToPieceTypeTestForWhitePieces") {
  // Arrange
  std::string fenChars{ "PNBRQKX" };
  PieceType expected[7]{ PieceType::PAWN, PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK, PieceType::QUEEN, PieceType::KING, PieceType::NULL_PIECE };
  PieceType actual[7]{ PieceType::NULL_PIECE };

  // Act
  for (int i = 0; i < 7; i++) {
    actual[i] = piece::fenCharToPieceType(fenChars[i]);
  }

  // Assert
  for (int i = 0; i < 7; i++) {
    CHECK(actual[i] == expected[i]);
  }
}

TEST_CASE("FunctionTests: FenCharToPieceColourTestForBlackPieces") {
  // Arrange
  std::string fenChars{ "pnbrqkx" };
  std::vector<PieceColour> expected(6, PieceColour::BLACK);
  expected.push_back(PieceColour::NULL_COLOUR);
  std::vector<PieceColour> actual(7, PieceColour::NULL_COLOUR);

  // Act
  for (int i = 0; i < 7; i++) {
    actual[i] = piece::fenCharToPieceColour(fenChars[i]);
  }

  // Assert
  for (int i = 0; i < 7; i++) {
    CHECK(actual[i] == expected[i]);
  }
}

TEST_CASE("FunctionTests: Fen char To PieceColour Test ForWhitePieces") {
  // Arrange
  std::string fenChars{ "PNBRQKX" };
  std::vector<PieceColour> expected(6, PieceColour::WHITE);
  expected.push_back(PieceColour::NULL_COLOUR);
  PieceColour actual[7]{ PieceColour::NULL_COLOUR };

  // Act
  for (int i = 0; i < 7; i++) {
    actual[i] = piece::fenCharToPieceColour(fenChars[i]);
  }

  // Assert
  for (int i = 0; i < 7; i++) {
    CHECK(actual[i] == expected[i]);
  }
}

} // namespace yak
