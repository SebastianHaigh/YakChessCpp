#ifndef YAK_BITBOARD_H_
#define YAK_BITBOARD_H_

#include <stdint.h>
#include <iostream>
#include <stack>

typedef uint64_t Bitboard;
typedef uint64_t Square;
typedef uint64_t File;
typedef uint64_t Rank;

//  +- - - + - - - + - - - - - - - - - - - - - - - - - +
//  |  RANK | RANK  |           SQUARE INDICES          |
//  |       | INDEX |                                   |
//  + - - - + - - - + - - - - - - - - - - - - - - - - - +
//  |   8   |   7   |   56  57  58  59  60  61  62  63  |
//  |   7   |   6   |   48  49  50  51  52  53  54  55  |
//  |   6   |   5   |   40  41  42  43  44  45  46  47  |
//  |   5   |   4   |   32  33  34  35  36  37  38  39  |
//  |   4   |   3   |   24  25  26  27  28  29  30  31  |
//  |   3   |   2   |   16  17  18  19  20  21  22  23  |
//  |   2   |   1   |   8   9   10  11  12  13  14  15  |
//  |   1   |   0   |   0   1   2   3   4   5   6   7   |
//  + - - - + - - - + - - - - - - - - - - - - - - - - - +
//  |   FILE INDEX  |   0   1   2   3   4   5   6   7   |
//  | - - - - - - - + - - - - - - - - - - - - - - - - - +
//  |     FILE      |   A   B   C   D   E   F   G   H   |
//  | - - - - - - - + - - - - - - - - - - - - - - - - - +

namespace bitboard {

// Bitmasks of all ranks
const Bitboard RANK_1 = 0x00000000000000ff;
const Bitboard RANK_2 = 0x000000000000ff00;
const Bitboard RANK_3 = 0x0000000000ff0000;
const Bitboard RANK_4 = 0x00000000ff000000;
const Bitboard RANK_5 = 0x000000ff00000000;
const Bitboard RANK_6 = 0x0000ff0000000000;
const Bitboard RANK_7 = 0x00ff000000000000;
const Bitboard RANK_8 = 0xff00000000000000;

const Bitboard NOT_RANK_1 = 0xffffffffffffff00;
const Bitboard NOT_RANK_2 = 0xffffffffffff00ff;
const Bitboard NOT_RANK_3 = 0xffffffffff00ffff;
const Bitboard NOT_RANK_4 = 0xffffffff00ffffff;
const Bitboard NOT_RANK_5 = 0xffffff00ffffffff;
const Bitboard NOT_RANK_6 = 0xffff00ffffffffff;
const Bitboard NOT_RANK_7 = 0xff00ffffffffffff;
const Bitboard NOT_RANK_8 = 0x00ffffffffffffff;

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

Bitboard south_one(Bitboard board);
Bitboard north_one(Bitboard board);
Bitboard east_one(Bitboard board);
Bitboard west_one(Bitboard board);
Bitboard north_east_one(Bitboard board);
Bitboard south_east_one(Bitboard board);
Bitboard north_west_one(Bitboard board);
Bitboard south_west_one(Bitboard board);
std::stack <Square> scan_forward(Bitboard board);
std::stack <Square> scan_backward(Bitboard board);
File file_index(Square square_index);
Rank rank_index(Square square_index);
Square square_index(File file_index, Rank rank_index);
void print_board(Bitboard board);

} // namespace bitboard

#endif // YAK_BITBOARD_H_