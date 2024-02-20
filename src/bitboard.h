#ifndef YAK_BITBOARD_H_
#define YAK_BITBOARD_H_

#include <stdint.h>
#include <iostream>
#include <type_traits>
#include <vector>
#include "types.h"

#if defined _MVC_VER
#include <intrin.h>
#endif

//  + - - - + - - - + - - - - - - - - - - - - - - - - - + - - - - - - - - - - +
//  |  RANK | RANK  |           SQUARE INDICES          |   NORTH      NORTH  |
//  |       | INDEX |                                   |   WEST        EAST  |
//  + - - - + - - - + - - - - - - - - - - - - - - - - - +     \         /     |
//  |   8   |   7   |   56  57  58  59  60  61  62  63  |      \ NORTH /      |
//  |   7   |   6   |   48  49  50  51  52  53  54  55  |       \  |  /       |
//  |   6   |   5   |   40  41  42  43  44  45  46  47  |  W     \ | /     E  |
//  |   5   |   4   |   32  33  34  35  36  37  38  39  |  E _____\|/_____ A  |
//  |   4   |   3   |   24  25  26  27  28  29  30  31  |  S      /|\      S  |
//  |   3   |   2   |   16  17  18  19  20  21  22  23  |  T     / | \     T  |
//  |   2   |   1   |   8   9   10  11  12  13  14  15  |       /  |  \       |
//  |   1   |   0   |   0   1   2   3   4   5   6   7   |      / SOUTH \      |
//  + - - - + - - - + - - - - - - - - - - - - - - - - - +     /         \     |
//  |   FILE INDEX  |   0   1   2   3   4   5   6   7   |   SOUTH      SOUTH  |
//  | - - - - - - - + - - - - - - - - - - - - - - - - - +   WEST        EAST  |
//  |     FILE      |   A   B   C   D   E   F   G   H   |                     |
//  | - - - - - - - + - - - - - - - - - - - - - - - - - + - - - - - - - - - - +

namespace yak::bitboard {

// Bitmasks of all ranks
const Bitboard RANK_1 = 0x00000000000000ff; /* Bitboard mask of the 1st rank. */
const Bitboard RANK_2 = 0x000000000000ff00; /* Bitboard mask of the 2nd rank. */
const Bitboard RANK_3 = 0x0000000000ff0000; /* Bitboard mask of the 3rd rank. */
const Bitboard RANK_4 = 0x00000000ff000000; /* Bitboard mask of the 4th rank. */
const Bitboard RANK_5 = 0x000000ff00000000; /* Bitboard mask of the 5th rank. */
const Bitboard RANK_6 = 0x0000ff0000000000; /* Bitboard mask of the 6th rank. */
const Bitboard RANK_7 = 0x00ff000000000000; /* Bitboard mask of the 7th rank. */
const Bitboard RANK_8 = 0xff00000000000000; /* Bitboard mask of the 8th rank. */

const Bitboard NOT_RANK_1 = 0xffffffffffffff00; /// Bitboard mask of all squares not on the 1st rank.
const Bitboard NOT_RANK_2 = 0xffffffffffff00ff; /// Bitboard mask of all squares not on the 2nd rank.
const Bitboard NOT_RANK_3 = 0xffffffffff00ffff; /// Bitboard mask of all squares not on the 3rd rank.
const Bitboard NOT_RANK_4 = 0xffffffff00ffffff; /// Bitboard mask of all squares not on the 4th rank.
const Bitboard NOT_RANK_5 = 0xffffff00ffffffff; /// Bitboard mask of all squares not on the 5th rank.
const Bitboard NOT_RANK_6 = 0xffff00ffffffffff; /// Bitboard mask of all squares not on the 6th rank.
const Bitboard NOT_RANK_7 = 0xff00ffffffffffff; /// Bitboard mask of all squares not on the 7th rank.
const Bitboard NOT_RANK_8 = 0x00ffffffffffffff; /// Bitboard mask of all squares not on the 8th rank.

// Bitmasks of all files
const Bitboard FILE_A = 0x0101010101010101;
const Bitboard FILE_B = 0x0202020202020202;
const Bitboard FILE_C = 0x0404040404040404;
const Bitboard FILE_D = 0x0808080808080808;
const Bitboard FILE_E = 0x1010101010101010;
const Bitboard FILE_F = 0x2020202020202020;
const Bitboard FILE_G = 0x4040404040404040;
const Bitboard FILE_H = 0x8080808080808080;

const Bitboard NOT_FILE_A = 0xfefefefefefefefe;
const Bitboard NOT_FILE_B = 0xfdfdfdfdfdfdfdfd;
const Bitboard NOT_FILE_C = 0xfbfbfbfbfbfbfbfb;
const Bitboard NOT_FILE_D = 0xf7f7f7f7f7f7f7f7;
const Bitboard NOT_FILE_E = 0xefefefefefefefef;
const Bitboard NOT_FILE_F = 0xdfdfdfdfdfdfdfdf;
const Bitboard NOT_FILE_G = 0xbfbfbfbfbfbfbfbf;
const Bitboard NOT_FILE_H = 0x7f7f7f7f7f7f7f7f;

// Diagonal Bitmasks
const Bitboard DIAG_A1_H8 = 0x8040201008040201;
const Bitboard DIAG_A8_H1 = 0x0102040810204080;

const Bitboard UNIVERSAL = 0xffffffffffffffff;
const Bitboard EMPTY = 0x0000000000000000;

/**
 * \brief Shifts a Bitboard by one square in a specified direction.
 * \tparam D - The direction in which to shift.
 * \param[in] board - The bitboard to be shifted.
 * \return The shifted bitboard.
 */
template<Direction D>
constexpr Bitboard shift(Bitboard board)
{
  if (D == Direction::NORTH) return board << 8;
  if (D == Direction::SOUTH) return board >> 8;
  if (D == Direction::EAST) return (board << 1) & bitboard::NOT_FILE_A;
  if (D == Direction::WEST) return (board >> 1) & bitboard::NOT_FILE_H;
  if (D == Direction::NORTH_EAST) return (board << 9) & bitboard::NOT_FILE_A;
  if (D == Direction::SOUTH_EAST) return (board >> 7) & bitboard::NOT_FILE_A;
  if (D == Direction::NORTH_WEST) return (board << 7) & bitboard::NOT_FILE_H;
  if (D == Direction::SOUTH_WEST) return (board >> 9) & bitboard::NOT_FILE_H;
  return board;
};

/**
 * \brief Returns the index of the most significant 1 bit (MS1B).
 * \param[in] board - A non-zero bitboard.
 * \return A Square corresponding to the index of the MS1B.
 */
inline Square MS1B(Bitboard board)
{
#if defined __GNUC__
  int idx = __builtin_clzll(board); // Returns number of leading zeros
  return static_cast<Square>(63 - idx);
#elif defined _MSC_VER
  unsigned long idx;
  _BitScanReverse64(&idx, board);
  return static_cast<Square>(idx);
#else
  return 0;
#endif
}

/**
 * \brief Returns the index of the least significant 1 bit (LS1B).
 * \param[in] board - A non-zero bitboard.
 * \return A Square corresponding to the index of the LS1B.
 */
inline Square LS1B(Bitboard board)
{
#if defined __GNUC__
  int idx = __builtin_ffsll(board) - 1;
  return static_cast<Square>(idx);
#elif defined _MSC_VER
  unsigned long idx;
  _BitScanForward64(&idx, board);
  return static_cast<Square>(idx);
#else
  return 0;
#endif
}

/**
 * \brief Clears the least significant 1 bit (LS1B).
 * \param[in] board - A reference to a non-zero bitboard.
 * \return A Square corresponding to the index of the bit that was cleared.
 */
Square popLS1B(Bitboard &board);

/**
 * \brief Clears the most significant 1 bit (MS1B).
 * \param[in] board - A reference to a non-zero bitboard.
 * \return A Square corresponding to the index of the bit that was cleared.
 */
Square popMS1B(Bitboard &board);

constexpr auto countSetBits(Bitboard board) -> int
{
  int setBits{0};
  while (board)
  {
    board &= board - 1;
    ++setBits;
  }

  return setBits;
}

void setSquare(Bitboard& board, const Square& square);
void setSquare(Bitboard &board, const Rank& rank, const File& file);

std::vector<Square> scanForward(Bitboard board);
std::vector<Square> scanBackward(Bitboard board);

Bitboard toBitboard(const Square& square);
Bitboard toBitboard(const File& file, const Rank& rank);
Bitboard toBitboard(std::string_view algebraic_square);

/**
 * \brief Provides single and multi piece bitboards at compile time.
 * \tparam S... - The squares on which the pieces are placed.
 */
template<Square... S>
struct static_bitboard
{
  static constexpr Bitboard value = 0;
};

template<Square S, Square... Sp>
struct static_bitboard<S, Sp...>
{
  static constexpr Bitboard value = (Bitboard{1} << S) | static_bitboard<Sp...>::value;
};

template<Square S1, Square S2>
struct static_bitboard<S1, S2>
{
  static constexpr Bitboard value = (Bitboard{1} << S1) | (Bitboard{1} << S2);
};

template<typename... Squares>
constexpr auto createBitboard(Squares ...squares) -> Bitboard
{
  static_assert((... && (std::is_same_v<std::decay_t<Squares>, Square>)));

  Bitboard board{0ULL};

  (..., setSquare(board, squares));

  return board;
}

template<typename Rank, typename File>
constexpr auto createBitboard(Rank rank, File file) -> Bitboard
{
  static_assert(std::is_same_v<std::decay_t<Rank>, Rank>);
  static_assert(std::is_same_v<std::decay_t<File>, File>);

  Bitboard board{0ULL};

  setSquare(board, rank, file);

  return board;
}

void print_board(Bitboard board);
std::string to_string(Bitboard board);

} // namespace yak::bitboard

#endif // YAK_BITBOARD_H_
