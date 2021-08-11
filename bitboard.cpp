#include "bitboard.h"

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

std::stack <Square> scan_backward(Bitboard board) {
    std::stack <Square> serialised_board;
    Bitboard check = 1; 
    for (Square square = 0; square < 64; square++) {
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

std::stack <Square> scan_forward(Bitboard board) {
    std::stack <Square> backward_serialised_board;
    std::stack <Square> serialised_board;
    backward_serialised_board = scan_backward(board);
    while (!backward_serialised_board.empty()) {
        serialised_board.push(backward_serialised_board.top());
        backward_serialised_board.pop();
    }
    return serialised_board;
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

void print_board(Bitboard board) { 
    std::stack <bool> board_stack[8];
    Bitboard unity = 1;
    for (Rank rank = 0; rank < 8; rank++) {
        for (File file = 0; file < 8; file++)
        {
            board_stack[file].push((board & (unity << square_index(file, rank))));
        }
    }

    for (Rank rank = 0; rank < 8; rank++) {
        for (File file = 0; file < 8; file++)
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
