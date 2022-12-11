#include "bitboard.h"
#include <string>

namespace yak {

namespace bitboard {

Square pop_LS1B(Bitboard &board)
{
  Square idx = LS1B(board);
  board &= board - 1;
  return idx;
}

Square pop_MS1B(Bitboard &board)
{
  Square idx = MS1B(board);
  board &= ~(Bitboard{1} << idx);
  return idx;
}

void set_square(Bitboard &board, Square square)
{
  Bitboard piece_to_set{1};
  board |= piece_to_set << square;
}

void set_square(Bitboard &board, Rank rank, File file)
{
  set_square(board, square_index(file, rank));
}

std::vector<Square> scan_backward(Bitboard board)
{
  std::vector<Square> serialised_board;

  while (board)
    serialised_board.push_back(pop_MS1B(board));

  return serialised_board;
}

std::vector<Square> scan_forward(Bitboard board)
{
  std::vector<Square> serialised_board;
  while (board)
  {
    serialised_board.push_back(pop_LS1B(board));
  }
  return serialised_board;
}

Square first_occupied_square(Bitboard board)
{
  for (Square square = 0; square < 64; square++)
  {
    if (board & to_bitboard(square))
    {
      return square;
    }
  }
  return Square(64);
}

File file_index(Square square_index)
{
  if (square_index >= 0 && square_index <= 63)
  {
    return square_index & 7;
  }
  else
  {
    return -1;
  }
}

File file_index(char algebraic_file)
{
  return toupper(algebraic_file) - 'A';
}

File file_index(std::string algebraic_square)
{
  return file_index(algebraic_square[0]);
}

Rank rank_index(Square square_index)
{
  if (square_index >= 0 && square_index <= 63)
  {
    return square_index >> 3;
  }
  else
  {
    return -1;
  }
}

Rank rank_index(char algebraic_rank)
{
  return algebraic_rank - '1';
}

Rank rank_index(std::string algebraic_square)
{
  return file_index(algebraic_square[1]);
}

Square square_index(File file_index, Rank rank_index)
{
  return (8 * rank_index) + file_index;
}

Square square_index(std::string square)
{
  File file = file_index(square[0]);
  Rank rank = rank_index(square[1]);
  return square_index(file, rank);
}

Bitboard to_bitboard(Square square)
{
  return Bitboard(1) << square;
}

Bitboard to_bitboard(File file, Rank rank)
{
  return to_bitboard(square_index(file, rank));
};

Bitboard to_bitboard(std::string square)
{
  return Bitboard(1) << square_index(square);
}

void print_board(Bitboard board)
{
  std::vector<std::vector<bool>> board_vector(8, std::vector<bool>(8, false));

  for (Rank rank = 0; rank < 8; rank++)
  {
    for (File file = 0; file < 8; file++)
    {
      board_vector[rank][file] = (board & to_bitboard(file, rank)) > 0;
    }
  }

  for (auto rit = board_vector.rbegin(); rit != board_vector.rend(); ++rit)
  {
    //for (File file = 0; file < 8; file++)
    for (auto file : *rit)
    {
      if (file)
      {
        std::cout << " 1";
      }
      else
      {
        std::cout << " 0";
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

}

std::string to_algebraic(Square square)
{
  std::string files{"abcdefgh"};
  return files[file_index(square)] + std::to_string(rank_index(square) + 1);
}

std::string to_algebraic(File file_index, Rank rank_index)
{
  std::string files{"abcdefgh"};
  return files[file_index] + std::to_string(rank_index + 1);
}

} // namespace bitboard

} // namespace yak