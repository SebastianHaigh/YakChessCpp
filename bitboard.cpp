#include "bitboard.h"

namespace bitboard {

uint64_t south_one(uint64_t board) {
    return board >> 8;
}

uint64_t north_one(uint64_t board) {
    return board << 8;
}

uint64_t east_one(uint64_t board) {
    return (board << 1) & NOT_FILE_A;
}

uint64_t west_one(uint64_t board) {
    return (board >> 1) & NOT_FILE_H;
}

uint64_t north_east_one(uint64_t board) {
    return (board << 9) & NOT_FILE_A;
}

uint64_t south_east_one(uint64_t board) {
    return (board >> 7) & NOT_FILE_A;
}

uint64_t north_west_one(uint64_t board) {
    return (board << 7) & NOT_FILE_H;
}

uint64_t south_west_one(uint64_t board) {
    return (board >> 9) & NOT_FILE_H;
}

std::stack <int> scan_backward(uint64_t board) {
    std::stack <int> serialised_board;
    uint64_t check = 1; 
    for (int square = 0; square < 64; square++) {
        if (board & check) {
            // The conditional will check if the union of board and check is 
            // non-zero. The check variable is an unsigned 64-bit integer, 
            // initialized as one before the loop begins. The board variable is 
            // also a 64-bit integer which is a bitboard representation of a 
            // chessboard. 
            //
            // Every iteration of the for loop, the check variable is 
            // left-shifted by one bit. Therefore, the check will always be 
            // 64-bits with exactly one non-zero bit, where the index of the 
            // non-zero bit corresponds to the iteration index square. 
            //
            // For any given square index, if (board & check) is non-zero, then 
            // it means that the chessboard represented by board has a piece on 
            // the square with index square.

            serialised_board.push(square);
        }
        check = check << 1;
    }
    return serialised_board;
}

std::stack <int> scan_forward(uint64_t board) {
    std::stack <int> backward_serialised_board;
    std::stack <int> serialised_board;
    backward_serialised_board = scan_backward(board);
    while (!backward_serialised_board.empty()) {
        serialised_board.push(backward_serialised_board.top());
        backward_serialised_board.pop();
    }
    return serialised_board;
}

int file_index(int square_index) {
    return (square_index | EMPTY) & 7;
}

int rank_index(int square_index) {
    return (square_index | EMPTY) >> 3;
}

int square_index(int file_index, int rank_index) {
    return (8 * rank_index) + file_index;
}

void print_board(uint64_t board) { 
    std::stack <bool> board_stack[8];
    uint64_t unity = 1;
    for (size_t rank = 0; rank < 8; rank++) {
        for (size_t file = 0; file < 8; file++)
        {
            board_stack[file].push((board & (unity << square_index(file, rank))));
        }
    }

    for (size_t rank = 0; rank < 8; rank++) {
        for (size_t file = 0; file < 8; file++)
        {
            if (board_stack[file].top()) {
                std::cout << " 1";
            } else {
                std::cout << " 0";
            }
            board_stack[file].pop();
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

}

} // namespace bitboard
