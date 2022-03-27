#include "bitboard.h"
#include <string>

namespace bitboard {

Bitboard south_one(Bitboard board) {
    return board >> 8;
}

Bitboard north_one(Bitboard board) {
    return board << 8;
}

Bitboard east_one(Bitboard board) {
    return (board << 1) & NOT_FILE_A;
}

Bitboard west_one(Bitboard board) {
    return (board >> 1) & NOT_FILE_H;
}

Bitboard north_east_one(Bitboard board) {
    return (board << 9) & NOT_FILE_A;
}

Bitboard south_east_one(Bitboard board) {
    return (board >> 7) & NOT_FILE_A;
}

Bitboard north_west_one(Bitboard board) {
    return (board << 7) & NOT_FILE_H;
}

Bitboard south_west_one(Bitboard board) {
    return (board >> 9) & NOT_FILE_H;
}

void set_square(Bitboard& board, Square square) {
    Bitboard piece_to_set{ 1 };
    board |= piece_to_set << square;
}

void set_square(Bitboard& board, Rank rank, File file) {
    set_square(board, square_index(file, rank));
}

std::vector<Square> scan_backward(Bitboard board) {
    std::vector<Square> serialised_board;
    for (Square square = 64; square > 0; square--) {
        if (board & to_bitboard(square - 1)) {
            serialised_board.push_back(square - 1);
        }
    }
    return serialised_board;
}

std::vector<Square> scan_forward(Bitboard board) {
    std::vector<Square> serialised_board;
    for (Square square = 0; square < 64; square++) {
        if (board & to_bitboard(square)) {
            serialised_board.push_back(square);
        }
    }
    return serialised_board;
}

Square first_occupied_square(Bitboard board) {
    for (Square square = 0; square < 64; square++) {
        if (board & to_bitboard(square)) {
            return square;
        }
    }
    return Square(64);
}

File file_index(Square square_index) {
    if (square_index >= 0 && square_index <= 63) {
        return square_index & 7;
    } else {
        return -1;
    }
}

Rank rank_index(Square square_index) {
    if (square_index >= 0 && square_index <= 63) {
        return square_index >> 3;
    } else {
        return -1;
    }
}

Square square_index(File file_index, Rank rank_index) {
    return (8 * rank_index) + file_index;
}

Bitboard to_bitboard(Square square) {
    return Bitboard(1) << square;
}

Bitboard to_bitboard(File file, Rank rank) { 
    return to_bitboard(square_index(file, rank)); 
};

void print_board(Bitboard board) { 
    std::vector<std::vector<bool>> board_vector(8, std::vector<bool>(8, false));

    for (Rank rank = 0; rank < 8; rank++) {
        for (File file = 0; file < 8; file++)
        {
            board_vector[file][rank] = (board & to_bitboard(file, rank)) > 0;
        }
    }

    for (Rank rank = 0; rank < 8; rank++) {
        for (File file = 0; file < 8; file++)
        {
            if (board_vector[file][rank]) {
                std::cout << " 1";
            } else {
                std::cout << " 0";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

}

Square algebraic_square_to_square_index(std::string algebraic_square) {
    File file = algebraic_file_to_file_index(algebraic_square[0]);
    Rank rank = algebraic_rank_to_rank_index(algebraic_square[1]);
    return square_index(file, rank);
}

std::string square_index_to_algebraic_square(Square square) {
    std::string files{ "abcdefgh" };
    return files[file_index(square)] + std::to_string(rank_index(square) + 1);
}

File algebraic_file_to_file_index(char algebraic_file) {
    switch (algebraic_file) {
        case 'a':
        case 'A':
            return 0;
        case 'b':
        case 'B':
            return 1;
        case 'c':
        case 'C':
            return 2;
        case 'd':
        case 'D':
            return 3;
        case 'e':
        case 'E':
            return 4;
        case 'f':
        case 'F':
            return 5;
        case 'g':
        case 'G':
            return 6;
        case 'h':
        case 'H':
            return 7;
        default:
            return -1;
    }
}

Rank algebraic_rank_to_rank_index(char algebraic_rank) {
    switch (algebraic_rank) {
    case '1':
        return 0;
    case '2':
        return 1;
    case '3':
        return 2;
    case '4':
        return 3;
    case '5':
        return 4;
    case '6':
        return 5;
    case '7':
        return 6;
    case '8':
        return 7;
    default:
        return -1;
    }
}

} // namespace bitboard
