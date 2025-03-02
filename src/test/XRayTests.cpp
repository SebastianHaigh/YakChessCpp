#include <catch2/catch_test_macros.hpp>

#include <bitboard.h>
#include <AttackMaps.hpp>
#include <MagicCommon.h>

namespace yak {

template<PieceType Type>
Bitboard xRayAttacks(Bitboard occupied_bb, Bitboard blocker_bb, Square piece)
{
  auto attacks = magic::singleSquareAttack<Type>(piece, occupied_bb);
  blocker_bb &= attacks;
  return attacks ^ magic::singleSquareAttack<Type>(piece, occupied_bb ^ blocker_bb);
}

TEST_CASE("X-Ray Test 1")
{
  // Blockers (B), test squares (t)
  //   Board                x-ray attack (rook)
  // . . B . . . . .       . . . 1 . . . .       . . . . . . . 1
  // . . . . . . . .       . . . 1 . . . .       1 . . . . . 1 .
  // . B . . . . . .       . . . 1 . . . .       . . . . . 1 . .
  // . . . B B . . .       . . . . . . . .       . . . . . . . .
  // . B . t . B . .       1 . . t . . 1 1       . . . t . . . .
  // . . . . B . . .       . . . . . . . .       . . . . . . . .
  // . B . . B . . .       . . . . . . . .       . . . . . 1 . .
  // . . . . . . . .       . . . . . . . .       1 . . . . . 1 .

  const Square rookSquare{ D4 };
  auto blockers_bb = bitboard::createBitboard(B2, B4, B6, C8, D5, E2, E3, E5, F4);
  auto occupied_bb = blockers_bb;

  CHECK(xRayAttacks<PieceType::ROOK>(occupied_bb, blockers_bb, rookSquare) == bitboard::createBitboard(A4, D6, D7, D8, G4, H4));
  CHECK(xRayAttacks<PieceType::BISHOP>(occupied_bb, blockers_bb, rookSquare) == bitboard::createBitboard(A1, A7, F2, F6, G1, G7, H8));
}

TEST_CASE("X-Ray Test 2")
{
  // Blockers (B), test squares (t)
  //   Board                x-ray attack (rook)
  // . O B . . . . .       . . . . . . . .       . . . . . . . .
  // B . . . . . O .       . . . . . . . .       1 . . . . . 1 .
  // . B . O . . . .       . . . 1 . . . .       . . . . . 1 . .
  // . . . B B . . .       . . . . . . . .       . . . . . . . .
  // O B . t . B B .       1 . . t . . 1 .       . . . t . . . .
  // . . . . B . . .       . . . . . . . .       . . . . . . . .
  // . O . O B . . .       . . . . . . . .       . . . . . 1 . .
  // . . . . . . . .       . . . . . . . .       . . . . . . 1 .

  const Square rookSquare{ D4 };
  auto blockers_bb = bitboard::createBitboard(A7, B4, B6, C8, D5, E2, E3, E5, F4, G4);
  auto occupied_bb = blockers_bb | bitboard::createBitboard(A4, B2, B8, D2, D6, G7);

  CHECK(xRayAttacks<PieceType::ROOK>(occupied_bb, blockers_bb, rookSquare) == bitboard::createBitboard(A4, D6, G4));
  CHECK(xRayAttacks<PieceType::BISHOP>(occupied_bb, blockers_bb, rookSquare) == bitboard::createBitboard(A7, F2, F6, G1, G7));
}

} // namespace yak
