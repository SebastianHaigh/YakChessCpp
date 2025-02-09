#include <catch2/catch_test_macros.hpp>

#include <attackmaps.h>
#include <types.h>
#include <bitboard.h>

#include <vector>

namespace yak::attackmap {

TEST_CASE("Ray generation")
{
  Bitboard occupied_bb, attackRay_bb;
  std::vector<Square> expectedRay;

  SECTION("North ray")
  {
    //   No blocker        non-adjacent         adjacent
    //                       blocker             blocker
    // 1 . . . . . . .    . . . . . . . .    . . . . . . . .
    // 1 . . . . . . .    B . . . . . . .    . . . . . . . .
    // 1 . . . . . . .    1 . . . . . . .    . . . . . . . .
    // 1 . . . . . . .    1 . . . . . . .    . . . . . . . .
    // 1 . . . . . . .    1 . . . . . . .    B . . . . . . .
    // x . . . . . . .    x . . . . . . .    x . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .

    constexpr Square square{ A3 };

    SECTION("No blocker")
    {
      occupied_bb = bitboard::static_bitboard<square>::value;
      expectedRay = std::vector<Square>{ A4, A5, A6, A7, A8 };
    }

    SECTION("Non-adjacent blocker")
    {
      occupied_bb = bitboard::static_bitboard<square, A7>::value;
      expectedRay = std::vector<Square>{ A4, A5, A6, A7 };
    }

    SECTION("Adjacent blocker")
    {
      occupied_bb = bitboard::static_bitboard<square, A4>::value;
      expectedRay = std::vector<Square>{ A4 };
    }

    attackRay_bb = blockedRay<Direction::NORTH>(square, occupied_bb);
  }

  SECTION("North-East ray")
  {
    //   No blocker        non-adjacent         adjacent
    //                       blocker             blocker
    // . . . . . 1 . .    . . . . . . . .    . . . . . . . .
    // . . . . 1 . . .    . . . . B . . .    . . . . . . . .
    // . . . 1 . . . .    . . . 1 . . . .    . . . . . . . .
    // . . 1 . . . . .    . . 1 . . . . .    . . . . . . . .
    // . 1 . . . . . .    . 1 . . . . . .    . B . . . . . .
    // x . . . . . . .    x . . . . . . .    x . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .

    constexpr Square square{ A3 };

    SECTION("No blocker")
    {
      occupied_bb = bitboard::static_bitboard<square>::value;
      expectedRay = std::vector<Square>{ B4, C5, D6, E7, F8 };
    }

    SECTION("Non-adjacent blocker")
    {
      occupied_bb = bitboard::static_bitboard<square, E7>::value;
      expectedRay = std::vector<Square>{ B4, C5, D6, E7 };
    }

    SECTION("Adjacent blocker")
    {
      occupied_bb = bitboard::static_bitboard<square, B4>::value;
      expectedRay = std::vector<Square>{ B4 };
    }

    attackRay_bb = blockedRay<Direction::NORTH_EAST>(square, occupied_bb);
  }

  SECTION("East ray")
  {
    //   No blocker        non-adjacent         adjacent
    //                       blocker             blocker
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // x 1 1 1 1 1 1 1    x 1 1 1 1 B . .    x B . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .

    constexpr Square square{ A6 };

    SECTION("No blocker")
    {
      occupied_bb = bitboard::static_bitboard<square>::value;
      expectedRay = std::vector<Square>{ B6, C6, D6, E6, F6, G6, H6 };
    }

    SECTION("Non-adjacent blocker")
    {
      occupied_bb = bitboard::static_bitboard<square, F6>::value;
      expectedRay = std::vector<Square>{ B6, C6, D6, E6, F6 };
    }

    SECTION("Adjacent blocker")
    {
      occupied_bb = bitboard::static_bitboard<square, B6>::value;
      expectedRay = std::vector<Square>{ B6 };
    }

    attackRay_bb = blockedRay<Direction::EAST>(square, occupied_bb);
  }

  SECTION("South-East ray")
  {
    //   No blocker        non-adjacent         adjacent
    //                       blocker             blocker
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // x . . . . . . .    x . . . . . . .    x . . . . . . .
    // . 1 . . . . . .    . 1 . . . . . .    . B . . . . . .
    // . . 1 . . . . .    . . 1 . . . . .    . . . . . . . .
    // . . . 1 . . . .    . . . 1 . . . .    . . . . . . . .
    // . . . . 1 . . .    . . . . B . . .    . . . . . . . .
    // . . . . . 1 . .    . . . . . . . .    . . . . . . . .

    constexpr Square square{ A6 };

    SECTION("No blocker")
    {
      occupied_bb = bitboard::static_bitboard<square>::value;
      expectedRay = std::vector<Square>{ F1, E2, D3, C4, B5 };
    }

    SECTION("Non-adjacent blocker")
    {
      occupied_bb = bitboard::static_bitboard<square, E2>::value;
      expectedRay = std::vector<Square>{ E2, D3, C4, B5 };
    }

    SECTION("Adjacent blocker")
    {
      occupied_bb = bitboard::static_bitboard<square, B5>::value;
      expectedRay = std::vector<Square>{ B5 };
    }

    attackRay_bb = blockedRay<Direction::SOUTH_EAST>(square, occupied_bb);
  }

  SECTION("South ray")
  {
    //   No blocker        non-adjacent         adjacent
    //                       blocker             blocker
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // x . . . . . . .    x . . . . . . .    x . . . . . . .
    // 1 . . . . . . .    1 . . . . . . .    B . . . . . . .
    // 1 . . . . . . .    1 . . . . . . .    . . . . . . . .
    // 1 . . . . . . .    1 . . . . . . .    . . . . . . . .
    // 1 . . . . . . .    B . . . . . . .    . . . . . . . .
    // 1 . . . . . . .    . . . . . . . .    . . . . . . . .

    constexpr Square square{ A6 };

    SECTION("No blocker")
    {
      occupied_bb = bitboard::static_bitboard<square>::value;
      expectedRay = std::vector<Square>{ A1, A2, A3, A4, A5 };
    }

    SECTION("Non-adjacent blocker")
    {
      occupied_bb = bitboard::static_bitboard<square, A2>::value;
      expectedRay = std::vector<Square>{ A2, A3, A4, A5 };
    }

    SECTION("Adjacent blocker")
    {
      occupied_bb = bitboard::static_bitboard<square, A5>::value;
      expectedRay = std::vector<Square>{ A5 };
    }

    attackRay_bb = blockedRay<Direction::SOUTH>(square, occupied_bb);
  }

  SECTION("South-West ray")
  {
    //   No blocker        non-adjacent         adjacent
    //                       blocker             blocker
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . x    . . . . . . . x    . . . . . . . x
    // . . . . . . 1 .    . . . . . . 1 .    . . . . . . B .
    // . . . . . 1 . .    . . . . . 1 . .    . . . . . . . .
    // . . . . 1 . . .    . . . . 1 . . .    . . . . . . . .
    // . . . 1 . . . .    . . . B . . . .    . . . . . . . .
    // . . 1 . . . . .    . . . . . . . .    . . . . . . . .

    constexpr Square square{ H6 };

    SECTION("No blocker")
    {
      occupied_bb = bitboard::static_bitboard<square>::value;
      expectedRay = std::vector<Square>{ C1, D2, E3, F4, G5 };
    }

    SECTION("Non-adjacent blocker")
    {
      occupied_bb = bitboard::static_bitboard<square, D2>::value;
      expectedRay = std::vector<Square>{ D2, E3, F4, G5 };
    }

    SECTION("Adjacent blocker")
    {
      occupied_bb = bitboard::static_bitboard<square, G5>::value;
      expectedRay = std::vector<Square>{ G5 };
    }

    attackRay_bb = blockedRay<Direction::SOUTH_WEST>(square, occupied_bb);
  }

  SECTION("West ray")
  {
    //   No blocker        non-adjacent         adjacent
    //                       blocker             blocker
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // 1 1 1 1 1 1 1 x    . . B 1 1 1 1 x    . . . . . . B x
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .

    constexpr Square square{ H6 };

    SECTION("No blocker")
    {
      occupied_bb = bitboard::static_bitboard<square>::value;
      expectedRay = std::vector<Square>{ A6, B6, C6, D6, E6, F6, G6 };
    }

    SECTION("Non-adjacent blocker")
    {
      occupied_bb = bitboard::static_bitboard<square, C6>::value;
      expectedRay = std::vector<Square>{ C6, D6, E6, F6, G6 };
    }

    SECTION("Adjacent blocker")
    {
      occupied_bb = bitboard::static_bitboard<square, G6>::value;
      expectedRay = std::vector<Square>{ G6 };
    }

    attackRay_bb = blockedRay<Direction::WEST>(square, occupied_bb);
  }

  SECTION("North-West ray")
  {
    //   No blocker        non-adjacent         adjacent
    //                       blocker             blocker
    // . . 1 . . . . .    . . . . . . . .    . . . . . . . .
    // . . . 1 . . . .    . . . B . . . .    . . . . . . . .
    // . . . . 1 . . .    . . . . 1 . . .    . . . . . . . .
    // . . . . . 1 . .    . . . . . 1 . .    . . . . . . . .
    // . . . . . . 1 .    . . . . . . 1 .    . . . . . . B .
    // . . . . . . . x    . . . . . . . x    . . . . . . . x
    // . . . . . . . .    . . . . . . . .    . . . . . . . .
    // . . . . . . . .    . . . . . . . .    . . . . . . . .

    constexpr Square square{ H3 };

    SECTION("No blocker")
    {
      occupied_bb = bitboard::static_bitboard<square>::value;
      expectedRay = std::vector<Square>{ G4, F5, E6, D7, C8 };
    }

    SECTION("Non-adjacent blocker")
    {
      occupied_bb = bitboard::static_bitboard<square, D7>::value;
      expectedRay = std::vector<Square>{ G4, F5, E6, D7 };
    }

    SECTION("Adjacent blocker")
    {
      occupied_bb = bitboard::static_bitboard<square, G4>::value;
      expectedRay = std::vector<Square>{ G4 };
    }

    attackRay_bb = blockedRay<Direction::NORTH_WEST>(square, occupied_bb);
  }

  std::vector<Square> serialisedRay = bitboard::scanForward(attackRay_bb);
  CHECK(serialisedRay == expectedRay);
}

TEST_CASE("Ray generation edge")
{
  std::vector<Bitboard> attackRays;

  SECTION("North ray")
  {
    // All squares on rank eight
    Square squares[8] = { A8, B8, C8, D8, E8, F8, G8, H8 };
    for (const auto& square : squares)
    {
      attackRays.push_back(blockedRay<Direction::NORTH>(square, bitboard::EMPTY));
    }
  }

  SECTION("North-East ray")
  {
    // All squares on file H or rank eight
    Square squares[15] = { A8, B8, C8, D8, E8, F8, G8, H8, H7, H6, H5, H4, H3, H2, H1 };
    for (const auto& square : squares)
    {
      attackRays.push_back(blockedRay<Direction::NORTH_EAST>(square, bitboard::EMPTY));
    }
  }

  SECTION("East ray")
  {
    // All squares on file H
    Square squares[8] = { H1, H2, H3, H4, H5, H6, H7, H8 };
    for (const auto& square : squares)
    {
      attackRays.push_back(blockedRay<Direction::EAST>(square, bitboard::EMPTY));
    }
  }

  SECTION("South-East ray")
  {
    // All squares on file H or rank one
    Square squares[15] = { A1, B1, C1, D1, E1, F1, G1, H1, H2, H3, H4, H5, H6, H7, H8 };
    for (const auto& square : squares)
    {
      attackRays.push_back(blockedRay<Direction::SOUTH_EAST>(square, bitboard::EMPTY));
    }
  }

  SECTION("South ray")
  {
    // All squares on rank one
    Square squares[8] = { A1, B1, C1, D1, E1, F1, G1, H1 };
    for (const auto& square : squares)
    {
      attackRays.push_back(blockedRay<Direction::SOUTH>(square, bitboard::EMPTY));
    }
  }

  SECTION("South-West ray")
  {
    // All squares on file A or rank one
    Square squares[15] = { A1, B1, C1, D1, E1, F1, G1, A2, A3, A4, A5, A6, A7, A8 };
    for (const auto& square : squares)
    {
      attackRays.push_back(blockedRay<Direction::SOUTH_WEST>(square, bitboard::EMPTY));
    }
  }

  SECTION("West ray")
  {
    // All squares on file A
    Square squares[8] = { A1, A2, A3, A4, A5, A6, A7, A8 };
    for (const auto& square : squares)
    {
      attackRays.push_back(blockedRay<Direction::WEST>(square, bitboard::EMPTY));
    }
  }

  SECTION("North-West ray")
  {
    // All squares on file A or rank eight
    Square squares[15] = { A1, A2, A3, A4, A5, A6, A7, A8, B8, C8, D8, E8, F8, G8, H8 };
    for (const auto& square : squares)
    {
      attackRays.push_back(blockedRay<Direction::NORTH_WEST>(square, bitboard::EMPTY));
    }
  }

  for (const auto& ray : attackRays)
  {
    // All returned rays are empty bitboards
    CHECK(ray == bitboard::EMPTY);
  }
}

} // namespace yak::attackmap
