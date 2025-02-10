#pragma once

#include <bitboard.h>
#include "MagicCommon.h"
#include <optional>

namespace yak::magic::generation {

inline auto random_uint64() -> Bitboard
{
  Bitboard u1, u2, u3, u4;
  u1 = (Bitboard)(rand()) & 0xFFFF; u2 = (Bitboard)(rand()) & 0xFFFF;
  u3 = (Bitboard)(rand()) & 0xFFFF; u4 = (Bitboard)(rand()) & 0xFFFF;
  return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

inline auto randomBitboard() -> Bitboard
{
  return random_uint64() & random_uint64() & random_uint64();
}

template<PieceType Type>
constexpr auto findMagic(Square square) -> std::optional<Bitboard>
{
  std::array<Bitboard, MAX_PERMUTATIONS> blockerPermutations;
  std::array<Bitboard, MAX_PERMUTATIONS> attackPermutations;
  std::array<Bitboard, MAX_PERMUTATIONS> used;

  const auto mask = OccupancyMasks<Type>::value[square];
  const int numBits = bitboard::countSetBits(mask);
  const int numPermutations = (1 << numBits);

  for (int i = 0; i < numPermutations; ++i)
  {
    blockerPermutations[i] = maskPermutation(mask, i);
    attackPermutations[i] = singleSquareAttack<Type>(static_cast<int>(square), blockerPermutations[i]);
  }

  // Brute force search for a magic number that can work
  for (int attempt = 0; attempt < 1000000000; ++attempt)
  {
    Bitboard magic = randomBitboard();

    // Not enough bits, this magic number will not work
    if (bitboard::countSetBits((mask * magic) & 0xFF00000000000000ULL) < 6)
    {
      continue;
    }

    std::fill(used.begin(), used.end(), Bitboard{0ULL});
    bool failure{false};

    for (int i = 0; not failure && i < numPermutations; ++i)
    {
      // TODO (haigh) vectorise this
      int mapIndex = transform(blockerPermutations[i], magic, numBits);

      if (used[mapIndex] == Bitboard{0ULL})
      {
        used[mapIndex] = attackPermutations[i];
      }
      else if (used[mapIndex] != attackPermutations[i])
      {
        failure = true;
      }
    }

    if (not failure)
    {
      return magic;
    }
  }

  return {};
}

template<PieceType Type>
constexpr auto findAllMagics() -> std::array<Bitboard, 64>
{
  std::array<Bitboard, 64> allMagics;

  for (int square = 0; square < 64; ++square)
  {
    if (auto magic = findMagic<Type>(static_cast<Square>(square)))
    {
      allMagics[square] = *magic;
      continue;
    }

    /* throw std::runtime_error{"Failed to find magic bitboard for square " + toAlgebraic(static_cast<Square>(square))}; */
  }

  return allMagics;
}

} // namespace yak::magic::generation
