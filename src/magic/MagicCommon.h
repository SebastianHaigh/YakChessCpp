#pragma once

#include "../types.h" // TODO (haigh) improve this include
#include "../bitboard.h" // TODO (haigh) improve this include
#include "AttackMaps.hpp"
#include <array>

namespace yak::magic {

static constexpr int MAX_PERMUTATIONS = 4096;

template<PieceType T>
static constexpr bool Failed = false;

/*
 * Occupancy mask for magic bitboard calculation, creates a bitboard of a rook or bishop attack,
 * where each ray doesn't include the outer edge of the board
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
 * Generates a compile-time static table of occupancy mask bitboards.
 */
template<PieceType Type, Square S = A1, Bitboard ...Masks>
struct OccupancyMasks : OccupancyMasks<Type, static_cast<Square>(static_cast<int>(S) + 1), Masks..., occupancyMask<Type>(S)> {};

template<PieceType Type, Bitboard ...Masks>
struct OccupancyMasks<Type, NULL_SQUARE, Masks...>
{
  static constexpr std::array<Bitboard, 64> value = { Masks... };
};

constexpr auto transform(Bitboard board, Bitboard magic, int bits) -> int
{
  return static_cast<int>( (board * magic) >> (64 - bits));
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

template<PieceType Type>
constexpr auto singleSquareAttack(int square, Bitboard blockers) -> Bitboard
{
  Bitboard attacks{0ULL};

  if constexpr (Type == PieceType::ROOK)
  {
    attacks |= attackmap::blockedRay<Direction::NORTH>(static_cast<Square>(square), blockers);
    attacks |= attackmap::blockedRay<Direction::SOUTH>(static_cast<Square>(square), blockers);
    attacks |= attackmap::blockedRay<Direction::EAST>(static_cast<Square>(square), blockers);
    attacks |= attackmap::blockedRay<Direction::WEST>(static_cast<Square>(square), blockers);
  }
  else if constexpr (Type == PieceType::BISHOP)
  {
    attacks |= attackmap::blockedRay<Direction::NORTH_EAST>(static_cast<Square>(square), blockers);
    attacks |= attackmap::blockedRay<Direction::NORTH_WEST>(static_cast<Square>(square), blockers);
    attacks |= attackmap::blockedRay<Direction::SOUTH_EAST>(static_cast<Square>(square), blockers);
    attacks |= attackmap::blockedRay<Direction::SOUTH_WEST>(static_cast<Square>(square), blockers);
  }
  else
  {
    static_assert(Type == PieceType::ROOK, "The attacks function only works for ROOKS and BISHOPS");
  }

  return attacks;
}


} // namespace yak::magic
