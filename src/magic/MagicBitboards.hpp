#pragma once

#include <types.h>
#include "MagicCommon.h"
#include <GeneratedMagics.h>
#include <array>

namespace yak::magic {

template<PieceType Type>
constexpr auto numberOfBits(Square square)
{
  auto mask = OccupancyMasks<Type>::value[square];
  return bitboard::countSetBits(mask);
}

template<PieceType Type>
constexpr auto numberOfPermutations(Square square)
{
  return (1 << numberOfBits<Type>(square));
}

template<PieceType Type>
struct Magics
{
  static_assert(Failed<Type>, "Magics can only be used for rook and bishop");
};

template<>
struct Magics<PieceType::ROOK>
{
  static constexpr std::array<Bitboard, 64> value = ROOK_MAGICS;
};

template<>
struct Magics<PieceType::BISHOP>
{
  static constexpr std::array<Bitboard, 64> value = BISHOP_MAGICS;
};

template<PieceType Type, Square S>
consteval auto buildMagicMap() -> std::array<Bitboard, MAX_PERMUTATIONS>
{
  std::array<Bitboard, MAX_PERMUTATIONS> map{};

  auto magic = Magics<Type>::value[S];

  for (int i = 0; i < numberOfPermutations<Type>(S); ++i)
  {
    auto blocker_bb = maskPermutation(OccupancyMasks<Type>::value[S], i);
    auto index = transform(blocker_bb,
                           magic,
                           numberOfBits<Type>(S));
    map[index] = singleSquareAttack<Type>(static_cast<int>(S), blocker_bb);
  }

  return map;
}

template<PieceType Type, Square S, std::array<Bitboard, MAX_PERMUTATIONS>... B>
struct MagicMapImpl : MagicMapImpl<Type, static_cast<Square>(S + 1), B..., buildMagicMap<Type, S>()> {};

template<PieceType Type, std::array<Bitboard, MAX_PERMUTATIONS>... B>
struct MagicMapImpl<Type, H8, B...>
{
  static constexpr std::array<std::array<Bitboard, MAX_PERMUTATIONS>, 64> value { B..., buildMagicMap<Type, H8>() };
};

template<PieceType Type>
struct MagicMaps : MagicMapImpl<Type, A1> {};

template<PieceType Type>
constexpr auto MagicBitboards(Square square, Bitboard blocker_bb) -> Bitboard
{
  const auto mask = OccupancyMasks<Type>::value[square];
  const auto index = transform(blocker_bb & mask, Magics<Type>::value[square], numberOfBits<Type>(square));

  return MagicMaps<Type>::value[square][index];
}

} // namespace yak::magic
