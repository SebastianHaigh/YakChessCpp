#ifndef MAGIC_BITBOARDS_H_
#define MAGIC_BITBOARDS_H_

#include "attackmaps.h"
#include "bitboard.h"
#include "types.h"

#include <stdlib.h>

#include <algorithm>
#include <array>
#include <optional>
#include <stdexcept>
#include <string>

namespace yak::magic {

template<PieceType T>
static constexpr bool Failed = false;

static constexpr int numSquares = 64;
static constexpr int numPermutations = 4096;

template<PieceType Type> struct numMaskPermutations { static_assert(Failed<Type>, "Cannot get mask permutations for this type"); };
template<> struct numMaskPermutations<PieceType::ROOK> { static constexpr int value = 4096; };
template<> struct numMaskPermutations<PieceType::BISHOP> { static constexpr int value = 4096; };

struct MagicMap
{
  std::array<Bitboard, numSquares * numPermutations> m_map;
  std::array<Bitboard, 64> m_mask;
  std::array<Bitboard, 64> m_magic;
};

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

/*
 * Rook mask for magic bitboard calculation, creates a bitboard of a rook
 * attack, where each ray doesn't include the outer edge of the board
 */
template<PieceType Type>
constexpr auto occupancyMask(Square square) -> Bitboard
{
  Bitboard mask{0};

  const int rank = square / 8;
  const int file = square % 8;

  if constexpr (Type == PieceType::ROOK)
  {
    for (int currentRank = rank + 1; currentRank <= 6; currentRank++) { mask |= (1ULL << (file + currentRank * 8)); }
    for (int currentRank = rank - 1; currentRank >= 1; currentRank--) { mask |= (1ULL << (file + currentRank * 8)); }
    for (int currentFile = file + 1; currentFile <= 6; currentFile++) { mask |= (1ULL << (currentFile + rank * 8)); }
    for (int currentFile = file - 1; currentFile >= 1; currentFile--) { mask |= (1ULL << (currentFile + rank * 8)); }
  }
  else if constexpr (Type == PieceType::BISHOP)
  {
    for (int currentRank = rank + 1, currentFile = file + 1; currentRank <= 6 && currentFile <= 6; currentRank++, currentFile++) { mask |= (1ULL << (currentFile + currentRank * 8)); }
    for (int currentRank = rank + 1, currentFile = file - 1; currentRank <= 6 && currentFile >= 1; currentRank++, currentFile--) { mask |= (1ULL << (currentFile + currentRank * 8)); }
    for (int currentRank = rank - 1, currentFile = file + 1; currentRank >= 1 && currentFile <= 6; currentRank--, currentFile++) { mask |= (1ULL << (currentFile + currentRank * 8)); }
    for (int currentRank = rank - 1, currentFile = file - 1; currentRank >= 1 && currentFile >= 1; currentRank--, currentFile--) { mask |= (1ULL << (currentFile + currentRank * 8)); }
  }
  else
  {
    static_assert(Failed<Type>, "Occupancy masks can only be calculated for ROOK and BISHOP");
  }

  return mask;
}

/*
 * Occupancy Masks for all squares.
 *
 * Generates a compile-time static table of bitboard.
 */
template<PieceType Type, Square S = A1, Bitboard ...Masks>
struct OccupancyMasks : OccupancyMasks<Type, static_cast<Square>(static_cast<int>(S) + 1), Masks..., occupancyMask<Type>(S)> {};

template<PieceType Type, Bitboard ...Masks>
struct OccupancyMasks<Type, NULL_SQUARE, Masks...>
{
  static constexpr std::array<Bitboard, 64> value = { Masks... };
};

/*
 *
 */
template<PieceType Type>
constexpr auto singleSquareAttack(int sq, Bitboard blockers) -> Bitboard
{
  Bitboard attacks{0ULL};

  const int rank = sq / 8;
  const int file = sq % 8;

  if constexpr (Type == PieceType::ROOK)
  {
    attacks |= attackmap::blockedRay<Direction::NORTH>(static_cast<Square>(sq), blockers);
    attacks |= attackmap::blockedRay<Direction::SOUTH>(static_cast<Square>(sq), blockers);
    attacks |= attackmap::blockedRay<Direction::EAST>(static_cast<Square>(sq), blockers);
    attacks |= attackmap::blockedRay<Direction::WEST>(static_cast<Square>(sq), blockers);
  }
  else if constexpr (Type == PieceType::BISHOP)
  {
    attacks |= attackmap::blockedRay<Direction::NORTH_EAST>(static_cast<Square>(sq), blockers);
    attacks |= attackmap::blockedRay<Direction::NORTH_WEST>(static_cast<Square>(sq), blockers);
    attacks |= attackmap::blockedRay<Direction::SOUTH_EAST>(static_cast<Square>(sq), blockers);
    attacks |= attackmap::blockedRay<Direction::SOUTH_WEST>(static_cast<Square>(sq), blockers);
  }
  else
  {
    static_assert(Type == PieceType::ROOK, "The attacks function only works for ROOKS and BISHOPS");
  }


  return attacks;
}

template<PieceType Type>
constexpr auto allAttacks(Square square, const std::array<Bitboard, 4096>& blockers)
{
  std::array<Bitboard, 4096> attackArray{};

  for (int i = 0; i < 4096; ++i)
  {
    attackArray[i] = singleSquareAttack<Type>(square, blockers[i]);
  }

  return attackArray;
}

/*
 * Calculation of mask permutations
 *
 * The number of set bits in the mask is calculated, this is required so that we know how many
 * permutations there are of this specific bit pattern.
 *
 * Consider the mask [10101010], there are 4 set bits and therefore 2^4 = 16 permutations of this mask.
 *
 * Therefore, the possible indexes are:
 *
 * 0000, 0001, 0010, 0011, 0100, 0101, 0110, 0111, 1000, 1001, 1010, 1011, 1100, 1101, 1110, 1111
 *
 * For each set bit (i), we find the index of that set bit (k) and then: if the ith bit of the
 * index is set, we set the kth bit of the permutation mask.
 *
 * For index 0000, none of the bits are set, and therefore this permutation will be empty.
 * For index 0001, the only the 1st bit is set, therefore this permutation will contain only the 1st set bit of the input mask, ie: [00000010]
 * For index 0010, the only the 2nd bit is set, therefore this permutation will contain only the 2nd set bit of the input mask, ie: [00001000]
 * For index 0011, the 1st and 2nd bits are set, therefore this permutation will contain both the 1st and 2nd set bits of the input mask, ie: [00001010]
 * ...
 * For index 1110, the 2nd, 3rd, and 4th bits are set, therefore the permutation is [10101000]
 * For index 1111, all bits are set, therefore the permutation is the same as the orignal input mask, ie: [10101010]
 */
constexpr auto maskPermutation(Bitboard mask, int index) -> Bitboard
{
  Bitboard result = 0ULL;

  int bitCounter{0};
  while (mask)
  {
    int nextSetBit = bitboard::popLS1B(mask);

    if (index & (1 << bitCounter))
    {
      result |= (Bitboard{1} << nextSetBit);
    }

    ++bitCounter;
  }

  return result;
}

constexpr auto transform(Bitboard board, Bitboard magic, int bits) -> int
{
  return static_cast<int>( (board * magic) >> (64 - bits));
}

struct MagicReturn
{
  std::array<Bitboard, numPermutations> m_map;
  Bitboard m_magic;
  Bitboard m_mask;
};

template<PieceType Type>
constexpr auto findMagic(Square square) -> std::optional<MagicReturn>
{
  std::array<Bitboard, 4096> allPossibleBlockers;
  const auto mask = OccupancyMasks<Type>::value[square];

  const int numBits = bitboard::countSetBits(mask);
  const int numPermutations = (1 << numBits);
  std::array<Bitboard, 4096> attacks;// = allAttacks<Type>(square, allPossibleBlockers);

  for (int i = 0; i < numPermutations; ++i)
  {
    allPossibleBlockers[i] = maskPermutation(mask, i);
    if (i > 0)
    {
      attacks[i - 1] = singleSquareAttack<Type>(static_cast<int>(square), allPossibleBlockers[i - 1]);
    }
  }

  attacks[numPermutations - 1] = singleSquareAttack<Type>(static_cast<int>(square), allPossibleBlockers[numPermutations - 1]);

  std::array<Bitboard, 4096> used{};

  for (int attempt = 0; attempt < 1000000000; ++attempt)
  {
    Bitboard potentialMagic = randomBitboard();

    if (bitboard::countSetBits((mask * potentialMagic) & 0xFF00000000000000ULL) < 6)
    {
      continue;
    }

    std::fill(used.begin(), used.end(), Bitboard{0ULL});

    bool failure{false};

    for (int i = 0; not failure && i < numPermutations; ++i)
    {
      // TODO (haigh) vectorise this
      int mapIndex = transform(allPossibleBlockers[i], potentialMagic, numBits);

      if (used[mapIndex] == Bitboard{0ULL})
      {
        used[mapIndex] = attacks[i];
      }
      else if (used[mapIndex] != attacks[i])
      {
        failure = true;
      }
    }

    if (not failure)
    {
      return MagicReturn{ used, potentialMagic, mask };
    }
  }

  return std::nullopt;
}

template<PieceType Type>
constexpr auto findAllMagics() -> std::array<MagicReturn, 64>
{
  std::array<MagicReturn, 64> allMagics;

  for (int square = 0; square < 64; ++square)
  {
    if (auto magic = findMagic<Type>(static_cast<Square>(square)))
    {
      allMagics[square] = *magic;
      continue;
    }

    throw std::runtime_error{"Failed to find magic bitboard for square " + toAlgebraic(static_cast<Square>(square))};
  }

  return allMagics;
}


} // namespace yak::magic

#endif // MAGIC_BITBOARDS_H_
