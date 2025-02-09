#include <catch2/catch_test_macros.hpp>

#include <bitboard.h>
#include <types.h>
#include <MagicBitboards.h>

#include <array>
#include <memory>

namespace yak::magic {

TEST_CASE("Compile time occupancy mask generation")
{
  Bitboard mask{ bitboard::EMPTY };
  // . . . . . . . .
  // 1 . . . . . . .
  // 1 . . . . . . .
  // 1 . . . . . . .
  // x 1 1 1 1 1 1 .
  // 1 . . . . . . .
  // 1 . . . . . . .
  // . . . . . . . .
  mask = OccupancyMasks<PieceType::ROOK>::value[A4];
  CHECK(mask == bitboard::static_bitboard<A2, A3, A5, A6, A7, B4, C4, D4, E4, F4, G4>::value);

  // . . . . . . . .
  // . . . . 1 . . .
  // . 1 1 1 x 1 1 .
  // . . . . 1 . . .
  // . . . . 1 . . .
  // . . . . 1 . . .
  // . . . . 1 . . .
  // . . . . . . . .
  mask = OccupancyMasks<PieceType::ROOK>::value[E6];
  CHECK(mask == bitboard::static_bitboard<E2, E3, E4, E5, E7, B6, C6, D6, F6, G6>::value);

  // . . . . . . . .
  // . . . 1 . . . .
  // . . 1 . . . . .
  // . 1 . . . . . .
  // x . . . . . . .
  // . 1 . . . . . .
  // . . 1 . . . . .
  // . . . . . . . .
  mask = OccupancyMasks<PieceType::BISHOP>::value[A4];
  CHECK(mask == bitboard::static_bitboard<B3, B5, C2, C6, D7>::value);

  // . . . . . . . .
  // . . 1 . . . 1 .
  // . . . 1 . 1 . .
  // . . . . x . . .
  // . . . 1 . 1 . .
  // . . 1 . . . 1 .
  // . 1 . . . . . .
  // . . . . . . . .
  mask = OccupancyMasks<PieceType::BISHOP>::value[E5];
  CHECK(mask == bitboard::static_bitboard<B2, C3, D4, F6, G7, C7, D6, F4, G3>::value);
}

TEST_CASE("Mask permutation")
{
  static constexpr Bitboard mask = Bitboard{0b01010101};

  std::array<Bitboard, 16> permutations = {
    Bitboard{0b00000000}, Bitboard{0b00000001}, Bitboard{0b00000100}, Bitboard{0b00000101},
    Bitboard{0b00010000}, Bitboard{0b00010001}, Bitboard{0b00010100}, Bitboard{0b00010101},
    Bitboard{0b01000000}, Bitboard{0b01000001}, Bitboard{0b01000100}, Bitboard{0b01000101},
    Bitboard{0b01010000}, Bitboard{0b01010001}, Bitboard{0b01010100}, Bitboard{0b01010101},
  };

  int index{0};
  for (const auto& permutation : permutations)
  {
    CHECK(maskPermutation(mask, index) == permutation);
    ++index;
  }
}

TEST_CASE("Magic generation rooks")
{
  auto magic = findAllMagics<PieceType::ROOK>();

  auto board = bitboard::createBitboard(A4, A7, B4, B8, C8, E2, F4);
  // Blockers (B), test squares (t)
  // . B B . . . . .
  // B . . . . . . .
  // . . . t . . . .
  // . . . . . . . .
  // B B . t . B . .
  // . . . . . . . .
  // . t . . B . . .
  // . . . . . . . .
  //
  // . . . . . . . .    . . . 1 . . . .    . . . 1 . . . .
  // . . . . . . . .    . . . 1 . . . .    . . . 1 . . . .
  // . . . . . . . .    . . . 1 . . . .    1 1 1 t 1 1 1 1
  // . . . . . . . .    . . . 1 . . . .    . . . 1 . . . .
  // . 1 . . . . . .    . 1 1 t 1 1 . .    . . . 1 . . . .
  // . 1 . . . . . .    . . . 1 . . . .    . . . 1 . . . .
  // 1 t 1 1 1 . . .    . . . 1 . . . .    . . . 1 . . . .
  // . 1 . . . . . .    . . . 1 . . . .    . . . 1 . . . .

  Square testSquares[3] = { B2, D4, D6 };
  Bitboard expectedAttacks[3] = { bitboard::static_bitboard<B1, B3, B4, A2, C2, D2, E2>::value,
                                  bitboard::static_bitboard<D1, D2, D3, D5, D6, D7, D8, B4, C4, E4, F4>::value,
                                  bitboard::static_bitboard<A6, B6, C6, E6, F6, G6, H6, D1, D2, D3, D4, D5, D7, D8>::value };
  const size_t numBits =  10; // Number of bits for the rook occupancy mask for B2, D4, D6
  bitboard::print_board(board);

  for (int i = 0; i < 3; ++i)
  {
    const auto& mag = magic[testSquares[i]];
    auto attack_bb = mag.m_map[transform(board & mag.m_mask, mag.m_magic, 10)];
    CHECK(attack_bb == expectedAttacks[i]);
  }
}

TEST_CASE("Magic generation bishops")
{
  // Blockers (B), test squares (t)
  // . B B . . . . .
  // B . . . . . . .
  // . . . t . . . .
  // . . . . . . . .
  // B B . t . B . .
  // . . . . . . . .
  // . t . . B . . .
  // . . . . . . . .

  Square testSquares[3] = { B2, D4, D6 };
  auto blockers_bb = bitboard::createBitboard(A4, A7, B4, B8, C8, E2, F4);
  Bitboard expectedAttacks[3];
  size_t numBits[3];
  std::array<MagicReturn, 64> magic;

  SECTION("Rooks")
  {
    magic = findAllMagics<PieceType::ROOK>();

    // . . . . . . . .    . . . 1 . . . .    . . . 1 . . . .
    // . . . . . . . .    . . . 1 . . . .    . . . 1 . . . .
    // . . . . . . . .    . . . 1 . . . .    1 1 1 t 1 1 1 1
    // . . . . . . . .    . . . 1 . . . .    . . . 1 . . . .
    // . 1 . . . . . .    . 1 1 t 1 1 . .    . . . 1 . . . .
    // . 1 . . . . . .    . . . 1 . . . .    . . . 1 . . . .
    // 1 t 1 1 1 . . .    . . . 1 . . . .    . . . 1 . . . .
    // . 1 . . . . . .    . . . 1 . . . .    . . . 1 . . . .

    expectedAttacks[0] = bitboard::static_bitboard<B1, B3, B4, A2, C2, D2, E2>::value;
    expectedAttacks[1] = bitboard::static_bitboard<D1, D2, D3, D5, D6, D7, D8, B4, C4, E4, F4>::value;
    expectedAttacks[2] = bitboard::static_bitboard<A6, B6, C6, E6, F6, G6, H6, D1, D2, D3, D4, D5, D7, D8>::value;

    numBits[0] = 10; // Occupancy mask bits for rook on B2
    numBits[1] = 10; // Occupancy mask bits for rook on D4
    numBits[2] = 10; // Occupancy mask bits for rook on D6
  }

  SECTION("Bishops")
  {
    magic = findAllMagics<PieceType::BISHOP>();

    // . . . . . . . 1    . . . . . . . 1    . 1 . . . 1 . .
    // . . . . . . 1 .    1 . . . . . 1 .    . . 1 . 1 . . .
    // . . . . . 1 . .    . 1 . . . 1 . .    . . . t . . . .
    // . . . . 1 . . .    . . 1 . 1 . . .    . . 1 . 1 . . .
    // . . . 1 . . . .    . . . t . . . .    . 1 . . . 1 . .
    // 1 . 1 . . . . .    . . 1 . 1 . . .    . . . . . . . .
    // . t . . . . . .    . 1 . . . 1 . .    . . . . . . . .
    // 1 . 1 . . . . .    1 . . . . . 1 .    . . . . . . . .

    expectedAttacks[0] = bitboard::static_bitboard<A1, A3, C1, C3, D4, E5, F6, G7, H8>::value;
    expectedAttacks[1] = bitboard::static_bitboard<A1, A7, B2, B6, C3, C5, E3, E5, F2, F6, G1, G7, H8>::value;
    expectedAttacks[2] = bitboard::static_bitboard<B4, B8, C5, C7, E5, E7, F4, F8>::value;

    numBits[0] = 5; // Occupancy mask bits for bishop on B2
    numBits[1] = 9; // Occupancy mask bits for bishop on D4
    numBits[2] = 7; // Occupancy mask bits for bishop on D6
  }

  bitboard::print_board(blockers_bb);

  for (int i = 0; i < 3; ++i)
  {
    const auto& mag = magic[testSquares[i]];
    auto attack_bb = mag.m_map[transform(blockers_bb & mag.m_mask, mag.m_magic, numBits[i])];
    CHECK(attack_bb == expectedAttacks[i]);
  }
}
} // namespace yak::magic
