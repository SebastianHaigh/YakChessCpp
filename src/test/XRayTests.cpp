#include <catch2/catch_test_macros.hpp>

#include <array>

#include <bitboard.h>
#include <AttackMaps.hpp>
#include <MagicCommon.h>

namespace yak {

template<Square from, Square to>
consteval Bitboard betweenSet()
{
  constexpr Bitboard to_bb = bitboard::static_bitboard<to>::value;

  if (attackmap::Ray<Direction::NORTH, from>::value & to_bb)
  {
    return attackmap::blockedRay<Direction::NORTH>(from, to_bb) ^ to_bb;
  }
  else if (attackmap::Ray<Direction::NORTH_EAST, from>::value & to_bb)
  {
    return attackmap::blockedRay<Direction::NORTH_EAST>(from, to_bb) ^ to_bb;
  }
  else if (attackmap::Ray<Direction::EAST, from>::value & to_bb)
  {
    return attackmap::blockedRay<Direction::EAST>(from, to_bb) ^ to_bb;
  }
  else if (attackmap::Ray<Direction::SOUTH_EAST, from>::value & to_bb)
  {
    return attackmap::blockedRay<Direction::SOUTH_EAST>(from, to_bb) ^ to_bb;
  }
  else if (attackmap::Ray<Direction::SOUTH, from>::value & to_bb)
  {
    return attackmap::blockedRay<Direction::SOUTH>(from, to_bb) ^ to_bb;
  }
  else if (attackmap::Ray<Direction::SOUTH_WEST, from>::value & to_bb)
  {
    return attackmap::blockedRay<Direction::SOUTH_WEST>(from, to_bb) ^ to_bb;
  }
  else if (attackmap::Ray<Direction::WEST, from>::value & to_bb)
  {
    return attackmap::blockedRay<Direction::WEST>(from, to_bb) ^ to_bb;
  }
  else if (attackmap::Ray<Direction::NORTH_WEST, from>::value & to_bb)
  {
    return attackmap::blockedRay<Direction::NORTH_WEST>(from, to_bb) ^ to_bb;
  }

  return 0;
}

template<Square from, Square to, Bitboard... sets>
struct OneSquareInBetween : OneSquareInBetween<from, static_cast<Square>(to + 1), sets..., betweenSet<from, to>()>
{
};

template<Square from, Bitboard... sets>
struct OneSquareInBetween<from, H8, sets...>
{
  static constexpr std::array<Bitboard, 64> value{ sets..., betweenSet<from, H8>() };
};

template<Square S, std::array<Bitboard, 64>... arrays>
struct InBetweenImpl : InBetweenImpl<static_cast<Square>(S + 1), arrays..., OneSquareInBetween<S, A1>::value>
{
};

template<std::array<Bitboard, 64>... arrays>
struct InBetweenImpl<H8, arrays...>
{
  static constexpr std::array<std::array<Bitboard, 64>, 64> value{ arrays... };
};

struct InBetween
{
  static constexpr std::array<std::array<Bitboard, 64>, 64> value{ InBetweenImpl<A1>::value };
};

template<PieceType Type>
constexpr Bitboard xRayAttacks(Bitboard occupied_bb, Bitboard blocker_bb, Square piece)
{
  auto attacks = magic::singleSquareAttack<Type>(piece, occupied_bb);
  blocker_bb &= attacks;
  return attacks ^ magic::singleSquareAttack<Type>(piece, occupied_bb ^ blocker_bb);
}

template<PieceType Type>
constexpr Bitboard pinners(Bitboard piece_bb, Bitboard occupied_bb, Bitboard friendly_bb, Square king)
{
  return xRayAttacks<Type>(occupied_bb, friendly_bb, king) & piece_bb;
}

template<PieceType Type>
constexpr Bitboard pinned(Bitboard piece_bb, Square kingSquare, Bitboard occupied_bb, Bitboard blocker_bb)
{
  auto pinners_bb = pinners<Type>(piece_bb, occupied_bb, blocker_bb, kingSquare);

  Bitboard pinned_bb{0};

  while (pinners_bb)
  {
    auto square = bitboard::popLS1B(pinners_bb);
    pinned_bb |= InBetween::value[kingSquare][square] & blocker_bb;
  }

  return pinned_bb;
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

TEST_CASE("Detect piece pinned to king by rook")
{
  // Rook (R), Bishop (B), King (K), pinned piece (P), not-pinned piece (X)
  //   Board
  // . . . . B . . .
  // . . . . . X . .
  // . . . . . . X .
  // . . R . P . . K
  // . . . . . . . .
  // . . . . . P . X
  // . . . . . . . X
  // . . . B . . . R

  const Square pinningRookSquare{ C5 };
  const Square nonPinningRookSquare{ H1 };
  const Square pinnedByRookSquare{ E5 };

  const Square pinningBishopSquare{ D1 };
  const Square nonPinningBishopSquare{ E8 };
  const Square pinnedByBishopSquare{ F3 };

  const Square kingSquare{ H5 };

  auto pinningRook_bb = bitboard::createBitboard(pinningRookSquare);
  auto nonPinningRook_bb = bitboard::createBitboard(nonPinningRookSquare);
  auto rook_bb = pinningRook_bb | nonPinningRook_bb;

  auto pinningBishop_bb = bitboard::createBitboard(pinningBishopSquare);
  auto nonPinningBishop_bb = bitboard::createBitboard(nonPinningBishopSquare);
  auto bishop_bb = pinningBishop_bb | nonPinningBishop_bb;

  auto pinnedPiece_bb = bitboard::createBitboard(pinnedByRookSquare, pinnedByBishopSquare);
  auto notPinned_bb = bitboard::createBitboard(F7, G6, H2, H3);

  auto king_bb = bitboard::createBitboard(kingSquare);

  auto ownPieces_bb = pinnedPiece_bb | notPinned_bb | king_bb; // pieces of the same colour as the king
  auto occupied_bb = rook_bb | bishop_bb | ownPieces_bb;

  auto pinnedByRook_bb = pinned<PieceType::ROOK>(rook_bb, kingSquare, occupied_bb, ownPieces_bb);
  auto pinnedByBishop_bb = pinned<PieceType::BISHOP>(bishop_bb, kingSquare, occupied_bb, ownPieces_bb);

  CHECK(pinnedByRook_bb == bitboard::createBitboard(pinnedByRookSquare));

  CHECK(pinnedByBishop_bb == bitboard::createBitboard(pinnedByBishopSquare));
}

TEST_CASE("In between tests")
{
  // Squares (S)
  //   Board                  In between squares
  // . . . . . . . .        . . . . . . . .
  // . . . . . S . .        . . . . . . . .
  // . . . . . . . .        . . . . 1 . . .
  // . . . . . . . .        . . . 1 . . . .
  // . . . . . . . .        . . 1 . . . . .
  // . S . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .

  CHECK(betweenSet<B3, F7>() == bitboard::createBitboard(C4, D5, E6));

  // Squares (S)
  //   Board                  In between squares
  // . . . . . S . .        . . . . . . . .
  // . . . . . . . .        . . . . . . 1 .
  // . . . . . . . S        . . . . . . . .
  // . . . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .

  CHECK(betweenSet<F8, H6>() == bitboard::createBitboard(G7));

  // Squares (S)
  //   Board                  In between squares
  // . . . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .
  // . S . . . S . .        . . 1 1 1 . . .
  // . . . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .

  CHECK(betweenSet<F5, B5>() == bitboard::createBitboard(C5, D5, E5));

  // Squares (S)
  //   Board                  In between squares
  // . . . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .
  // . S . . . . . .        . . . . . . . .
  // . . . . . S . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .

  CHECK(betweenSet<F5, B6>() == bitboard::EMPTY);

  // Squares (S)
  //   Board                  In between squares
  // . . . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .
  // . S . . . . . .        . . . . . . . .
  // . . S . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .

  CHECK(betweenSet<C5, B6>() == bitboard::EMPTY);

  // Squares (S)
  //   Board                  In between squares
  // . . . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .
  // . S . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .
  // . . . . . . . .        . . . . . . . .

  CHECK(betweenSet<B6, B6>() == bitboard::EMPTY);
}

} // namespace yak
