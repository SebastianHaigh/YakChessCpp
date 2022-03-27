#ifndef YAK_BITBOARD_H_
#define YAK_BITBOARD_H_

#include <stdint.h>
#include <iostream>
#include <vector>

/// <summary>
///  Type for holding representations of the chess board.
/// </summary>
typedef uint64_t Bitboard;
typedef uint64_t Square;
typedef uint64_t File;
typedef uint64_t Rank;

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

namespace bitboard {

// Bitmasks of all ranks
const Bitboard RANK_1 = 0x00000000000000ff; /// Bitboard mask of the 1st rank.
const Bitboard RANK_2 = 0x000000000000ff00; /// Bitboard mask of the 2nd rank.
const Bitboard RANK_3 = 0x0000000000ff0000; /// Bitboard mask of the 3rd rank.
const Bitboard RANK_4 = 0x00000000ff000000; /// Bitboard mask of the 4th rank.
const Bitboard RANK_5 = 0x000000ff00000000; /// Bitboard mask of the 5th rank.
const Bitboard RANK_6 = 0x0000ff0000000000; /// Bitboard mask of the 6th rank.
const Bitboard RANK_7 = 0x00ff000000000000; /// Bitboard mask of the 7th rank.
const Bitboard RANK_8 = 0xff00000000000000; /// Bitboard mask of the 8th rank.

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
* Shifts a Bitboard south by one rank.
* 
* @param The Bitbaord to be shifted.
* @returns The shifted Bitboard.
*/
Bitboard south_one(Bitboard board);

/**
* Shifts a Bitboard north by one rank.
*
* @param The Bitbaord to be shifted.
* @returns The shifted Bitboard.
*/
Bitboard north_one(Bitboard board);

/**
* Shifts a Bitboard east by one file.
*
* @param The Bitbaord to be shifted.
* @returns The shifted Bitboard.
*/
Bitboard east_one(Bitboard board);

/**
* Shifts a Bitboard west by one file.
*
* @param The Bitbaord to be shifted.
* @returns The shifted Bitboard.
*/
Bitboard west_one(Bitboard board);
Bitboard north_east_one(Bitboard board);
Bitboard south_east_one(Bitboard board);
Bitboard north_west_one(Bitboard board);
Bitboard south_west_one(Bitboard board);
void set_square(Bitboard& board, Square square);
void set_square(Bitboard& board, Rank rank, File file);
std::vector<Square> scan_forward(Bitboard board);
std::vector<Square> scan_backward(Bitboard board);

Square first_occupied_square(Bitboard board);

/// <summary>
/// Returns the file index of the specified Square
/// </summary>
/// <param name="square_index">The index of a square on the board.</param>
/// <returns>The File that the square is on.</returns>
File file_index(Square square_index);

/// <summary>
/// Returns the rank index of the specified Square
/// </summary>
/// <param name="square_index">The index of a square on the board.</param>
/// <returns>The Rank that the square is on.</returns>
Rank rank_index(Square square_index);
Square square_index(File file_index, Rank rank_index);
Bitboard to_bitboard(Square square);
Bitboard to_bitboard(File file, Rank rank);
void print_board(Bitboard board);

Square algebraic_square_to_square_index(std::string algebraic_square);
std::string square_index_to_algebraic_square(Square square);
File algebraic_file_to_file_index(char algebraic_file);
Rank algebraic_rank_to_rank_index(char algebraic_rank);

} // namespace bitboard

#endif // YAK_BITBOARD_H_