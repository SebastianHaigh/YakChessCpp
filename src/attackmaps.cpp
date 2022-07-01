#include "attackmaps.h"

namespace attacks {

    KnightAttacks::KnightAttacks() {
        for (int i = 0; i < 64; i++) {
            Bitboard knight_square = bitboard::to_bitboard(i);
            maps[i] = bitboard::north_one(bitboard::north_east_one(knight_square)); // north-north-east
            maps[i] |= bitboard::north_one(bitboard::north_west_one(knight_square)); // north-north-west
            maps[i] |= bitboard::east_one(bitboard::north_east_one(knight_square)); // north-east-east
            maps[i] |= bitboard::west_one(bitboard::north_west_one(knight_square)); // north-west_west
            maps[i] |= bitboard::east_one(bitboard::south_east_one(knight_square)); // south-east-east
            maps[i] |= bitboard::west_one(bitboard::south_west_one(knight_square)); // south-west-west
            maps[i] |= bitboard::south_one(bitboard::south_east_one(knight_square)); // south-south-east
            maps[i] |= bitboard::south_one(bitboard::south_west_one(knight_square)); // south-south-west
        }
    }


    Bitboard KnightAttacks::get(Square serialised_piece) {
        return maps[serialised_piece];
    }

    KingAttacks::KingAttacks() {
        for (int i = 0; i < 64; i++) {
            Bitboard king_square = bitboard::to_bitboard(i);
            maps[i] = bitboard::north_one(king_square);
            maps[i] |= bitboard::north_east_one(king_square);
            maps[i] |= bitboard::east_one(king_square);
            maps[i] |= bitboard::south_east_one(king_square);
            maps[i] |= bitboard::south_one(king_square);
            maps[i] |= bitboard::south_west_one(king_square);
            maps[i] |= bitboard::west_one(king_square);
            maps[i] |= bitboard::north_west_one(king_square);  
        }
    }


    Bitboard KingAttacks::get(Square serialised_piece) {
        return maps[serialised_piece];
    }



} // namespace attacks