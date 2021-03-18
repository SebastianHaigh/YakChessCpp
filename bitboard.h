#ifndef YAK_BITBOARD_H_
#define YAK_BITBOARD_H_

#include <stdint.h>
#include <iostream>
#include <stack>

namespace bitboard {

const uint64_t RANK_1 = 0x00000000000000ff;
const uint64_t RANK_2 = 0x000000000000ff00;
const uint64_t RANK_3 = 0x0000000000ff0000;
const uint64_t RANK_4 = 0x00000000ff000000;
const uint64_t RANK_5 = 0x000000ff00000000;
const uint64_t RANK_6 = 0x0000ff0000000000;
const uint64_t RANK_7 = 0x00ff000000000000;
const uint64_t RANK_8 = 0xff00000000000000;

const uint64_t NOT_RANK_1 = 0xffffffffffffff00;
const uint64_t NOT_RANK_2 = 0xffffffffffff00ff;
const uint64_t NOT_RANK_3 = 0xffffffffff00ffff;
const uint64_t NOT_RANK_4 = 0xffffffff00ffffff;
const uint64_t NOT_RANK_5 = 0xffffff00ffffffff;
const uint64_t NOT_RANK_6 = 0xffff00ffffffffff;
const uint64_t NOT_RANK_7 = 0xff00ffffffffffff;
const uint64_t NOT_RANK_8 = 0x00ffffffffffffff;

const uint64_t FILE_A = 0x0101010101010101;
const uint64_t FILE_B = 0x0202020202020202;
const uint64_t FILE_C = 0x0404040404040404;
const uint64_t FILE_D = 0x0808080808080808;
const uint64_t FILE_E = 0x1010101010101010;
const uint64_t FILE_F = 0x2020202020202020;
const uint64_t FILE_G = 0x4040404040404040;
const uint64_t FILE_H = 0x8080808080808080;

const uint64_t NOT_FILE_A = 0xfefefefefefefefe;
const uint64_t NOT_FILE_B = 0xfdfdfdfdfdfdfdfd;
const uint64_t NOT_FILE_C = 0xfbfbfbfbfbfbfbfb;
const uint64_t NOT_FILE_D = 0xf7f7f7f7f7f7f7f7;
const uint64_t NOT_FILE_E = 0xefefefefefefefef;
const uint64_t NOT_FILE_F = 0xdfdfdfdfdfdfdfdf;
const uint64_t NOT_FILE_G = 0xbfbfbfbfbfbfbfbf;
const uint64_t NOT_FILE_H = 0x7f7f7f7f7f7f7f7f;

const uint64_t UNIVERSAL = 0xffffffffffffffff;
const uint64_t EMPTY = 0x0000000000000000;

uint64_t south_one(uint64_t board);
uint64_t north_one(uint64_t board);
uint64_t east_one(uint64_t board);
uint64_t west_one(uint64_t board);
uint64_t north_east_one(uint64_t board);
uint64_t south_east_one(uint64_t board);
uint64_t north_west_one(uint64_t board);
uint64_t south_west_one(uint64_t board);
std::stack <int> scan_forward(uint64_t board);
std::stack <int> scan_backward(uint64_t board);
int file_index(int square_index);
int rank_index(int square_index);
int square_index(int file_index, int rank_index);
void print_board(uint64_t board);

} // namespace bitboard

#endif // YAK_BITBOARD_H_