#include <catch2/catch_test_macros.hpp>

#include <pieces.h>
#include <bitboard.h>
#include <pawns.h>

#include <string>

namespace yak {

TEST_CASE("Check double push target")
{
  CHECK(pawns::pawnDoublePushTarget<PieceColour::BLACK>() == bitboard::RANK_5);
  CHECK(pawns::pawnDoublePushTarget<PieceColour::WHITE>() == bitboard::RANK_4);
}

TEST_CASE("BlackPawnsTest: WestAttackSources")
{
  Bitboard testSquare_bb = bitboard::createBitboard(C6);

  Bitboard target = bitboard::createBitboard(C6);
  Bitboard expected = bitboard::createBitboard(D7);

  Bitboard actual = pawns::pawnWestAttackSource<PieceColour::BLACK>(target);

  CHECK(actual == expected);
}

TEST_CASE("BlackPawnsTest: WestAttackTarget")
{
  Bitboard source = bitboard::createBitboard(E6);
  Bitboard expected = bitboard::createBitboard(D5);

  Bitboard actual = pawns::pawnWestAttackTarget<PieceColour::BLACK>(source);

  CHECK(actual == expected);
}

TEST_CASE("BlackPawnsTest: EastAttackSources")
{
  Bitboard target = bitboard::createBitboard(C6);
  Bitboard expected = bitboard::createBitboard(B7);

  Bitboard actual = pawns::pawnEastAttackSource<PieceColour::BLACK>(target);

  CHECK(actual == expected);
}

TEST_CASE("BlackPawnsTest: EastAttackTarget")
{
  Bitboard source = bitboard::createBitboard(E6);
  Bitboard expected = bitboard::createBitboard(F5);

  Bitboard actual = pawns::pawnEastAttackTarget<PieceColour::BLACK>(source);

  CHECK(actual == expected);
}

TEST_CASE("WhitePawnsTest: WestAttackSources")
{
  Bitboard target = bitboard::createBitboard(C6);
  Bitboard expected = bitboard::createBitboard(D5);

  Bitboard actual = pawns::pawnWestAttackSource<PieceColour::WHITE>(target);

  CHECK(actual == expected);
}

TEST_CASE("WhitePawnsTest: WestAttackTarget")
{
  Bitboard source = bitboard::createBitboard(E6);
  Bitboard expected = bitboard::createBitboard(D7);

  Bitboard actual = pawns::pawnWestAttackTarget<PieceColour::WHITE>(source);

  CHECK(actual == expected);
}

TEST_CASE("WhitePawnsTest: EastAttackSources")
{
  Bitboard target = bitboard::createBitboard(C6);
  Bitboard expected = bitboard::createBitboard(B5);

  Bitboard actual = pawns::pawnEastAttackSource<PieceColour::WHITE>(target);

  CHECK(actual == expected);
}

TEST_CASE("WhitePawnsTest: EastAttackTarget")
{
  Bitboard source = bitboard::createBitboard(E6);
  Bitboard expected = bitboard::createBitboard(F7);

  Bitboard actual = pawns::pawnEastAttackTarget<PieceColour::WHITE>(source);

  CHECK(actual == expected);
}

TEST_CASE("Knight attacks")
{
  Bitboard knight_bb{ bitboard::EMPTY }, occupied_bb{ bitboard::EMPTY }, expected_bb{ bitboard::EMPTY };

  SECTION("Knight on A1")
  {
    knight_bb = bitboard::static_bitboard<A1>::value;
    expected_bb = bitboard::createBitboard(B3, C2);
  }

  SECTION("Knight on D4")
  {
    knight_bb = bitboard::static_bitboard<D4>::value;
    expected_bb = bitboard::createBitboard(B3, B5, C2, C6, E2, E6, F3, F5);
  }

  auto actual_bb = piece::pieceAttacks<PieceType::KNIGHT>(knight_bb, occupied_bb);

  CHECK(actual_bb == expected_bb);
}

TEST_CASE("King attacks")
{
  Bitboard occupied_bb{ bitboard::EMPTY };
  Bitboard king_bb{ bitboard::EMPTY };
  Bitboard expected_bb{ bitboard::EMPTY };

  SECTION("King on A1")
  {
    king_bb = bitboard::createBitboard(A1);
    expected_bb = bitboard::createBitboard(A2, B1, B2);
  }

  SECTION("King on D4")
  {
    king_bb = bitboard::createBitboard(D4);
    expected_bb = bitboard::createBitboard(C3, C4, C5, D3, D5, E3, E4, E5);
  }

  auto actual_bb = piece::pieceAttacks<PieceType::KING>(king_bb, occupied_bb);

  CHECK(actual_bb == expected_bb);
}

TEST_CASE("Rook attacks")
{
  Bitboard rook_bb = bitboard::createBitboard(A1);
  Bitboard opponentPieces_bb = bitboard::createBitboard(A5);
  Bitboard friendlyPieces_bb = rook_bb | bitboard::createBitboard(H8);
  auto occupied_bb = opponentPieces_bb | friendlyPieces_bb;
  auto expected_bb = bitboard::createBitboard(A2, A3, A4, A5) | bitboard::RANK_1 & ~rook_bb;

  auto actual_bb = piece::pieceAttacks<PieceType::ROOK>(rook_bb, occupied_bb);

  CHECK(actual_bb == expected_bb);
}

TEST_CASE("Queen attacks")
{
  Bitboard queen_bb = bitboard::createBitboard(A1);
  Bitboard opponentPieces_bb = bitboard::createBitboard(A8);
  Bitboard friendlyPieces_bb = queen_bb | bitboard::createBitboard(H8);
  Bitboard occupied = opponentPieces_bb | friendlyPieces_bb;
  Bitboard expected = (bitboard::FILE_A | bitboard::RANK_1 | bitboard::DIAG_A1_H8) & ~queen_bb;

  Bitboard actual = piece::pieceAttacks<PieceType::QUEEN>(queen_bb, occupied);

  CHECK(actual == expected);
}

TEST_CASE("Bishop attacks")
{
  // Arrange
  Bitboard bishop_bb = bitboard::createBitboard(A1, A8);
  Bitboard opponentPieces_bb = bitboard::createBitboard(A8);
  Bitboard friendlyPieces_bb = bishop_bb | bitboard::createBitboard(H8);
  Bitboard occupied = opponentPieces_bb | friendlyPieces_bb;
  Bitboard expected = (bitboard::DIAG_A8_H1 | bitboard::DIAG_A1_H8) & bitboard::NOT_FILE_A;

  // Act
  Bitboard actual = piece::pieceAttacks<PieceType::BISHOP>(bishop_bb, occupied);

  // Assert
  CHECK(actual == expected);
}

TEST_CASE("Fen Tests: Piece types")
{
  std::string fenChars;
  PieceType expectedType[6]{ PieceType::PAWN, PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK, PieceType::QUEEN, PieceType::KING };
  PieceColour expectedColour{ PieceColour::NULL_COLOUR };

  SECTION("White pieces")
  {
    fenChars = "PNBRQKX";
    expectedColour = PieceColour::WHITE;
  }

  SECTION("Black pieces")
  {
    fenChars = "pnbrqkx";
    expectedColour = PieceColour::BLACK;
  }

  for (int i = 0; i < 6; ++i)
  {
    CHECK(piece::fenCharToPieceType(fenChars[i]) == expectedType[i]);
    CHECK(piece::fenCharToPieceColour(fenChars[i]) == expectedColour);
  }

  CHECK(piece::fenCharToPieceType(fenChars[6]) == PieceType::NULL_PIECE);
  CHECK(piece::fenCharToPieceColour(fenChars[6]) == PieceColour::NULL_COLOUR);
}

} // namespace yak
