#include "bitboard.h"
#include "types.h"
#include <catch2/catch_test_macros.hpp>

#include "catch2/benchmark/catch_benchmark.hpp"

#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <MagicBitboards.h>
#include <iostream>
#include <stdexcept>

namespace yak::magic {

TEST_CASE("Random numbers can be generated")
{
  /* bitboard::print_board(rookMask(D4)); */
  /* bitboard::print_board(rookMask(D7)); */
  /* bitboard::print_board(rookMask(A3)); */

  /* static constexpr auto m = rookMask(A4); */

  /* bitboard::print_board(m); */
  /* std::cout << m << std::endl; */
  /* std::cout << std::hex << m << std::endl; */


  /* static constexpr auto perms = maskPermutations(rookMask(A4)); */

  /* for (int i = 0; i < 20; ++i) */
  /* { */
  /*   bitboard::print_board(perms[i]); */
  /* } */

  /* /1* auto magic = findMagic<PieceType::ROOK>(A4); *1/ */

  /* if (not magic) */
  /* { */
  /*   std::cout << "failed" << std::endl; */
  /* } */
  /* else */
  /* { */

  /* std::cout << std::hex << *magic << std::endl; */
  /* } */

  /* auto rookTable = rookMagicBitboard(); */

  /* if (not rookTable.first) */
  /* { */
  /*   std::cout << "failed again" << std::endl; */
  /* } */
  /* std::cout << rookTable.second[30] << ", " << rookTable.second[900] << std::endl; */


}

TEST_CASE("Compile time occupancy mask generation")
{
  auto rookMask = OccupancyMasks<PieceType::ROOK>::value[A4];
  bitboard::print_board(rookMask);

  auto bishopMask = OccupancyMasks<PieceType::BISHOP>::value[A4];
  bitboard::print_board(bishopMask);
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

  bitboard::print_board(maskPermutation(mask, 3));
  bitboard::print_board(maskPermutation(mask, 5));
  bitboard::print_board(maskPermutation(mask, 8));
  bitboard::print_board(maskPermutation(mask, 11));
  bitboard::print_board(maskPermutation(mask, 14));
  bitboard::print_board(maskPermutation(mask, 21));
  bitboard::print_board(maskPermutation(mask, 25));
  bitboard::print_board(maskPermutation(mask, 31));
  bitboard::print_board(maskPermutation(mask, 35));
}

TEST_CASE("Generate a magic")
{

  try
  {
    auto magic = findAllMagics<PieceType::ROOK>();

    auto board = bitboard::createBitboard(A4, A7, C8, E2, B4, B8, F4);

    std::cout << "board of blockers" << std::endl;
    bitboard::print_board(board);

    for (int square = 0; square < 64; ++square)
    {
      const auto& mag = magic[square];

      std::cout << std::endl << "masked" << std::endl;
      bitboard::print_board(board & mag.m_mask);

      std::cout << std::endl << "Attacks" << std::endl;
      bitboard::print_board(mag.m_map[transform(board & mag.m_mask, mag.m_magic, 12)]);
    }

  }
  catch (std::runtime_error& e)
  {
    std::cout << "Failed with error " << e.what() << std::endl;
  }

}

TEST_CASE("Magic finding benchmark")
{
  BENCHMARK("find magics")
  {
    auto magic = findAllMagics<PieceType::ROOK>();
  };

}

} // namespace yak::magic
