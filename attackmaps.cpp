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

Bitboard Ray::get(Square serialised_piece, Bitboard occupied_squares) {

    Bitboard ray = 0;
    std::vector<Square> blockers;

    blockers = get_blockers(serialised_piece, occupied_squares);
    if (!blockers.empty()){
        ray = ray_maps[serialised_piece] ^ ray_maps[blockers[0]];
    } else {
        ray = ray_maps[serialised_piece] ^ bitboard::EMPTY;
    }
    
    return ray;
}

std::vector<Square> Ray::get_blockers(Square serialised_piece, 
                                     Bitboard occupied_squares) {

    std::vector<Square> blockers;

    return blockers;
}

std::vector<Square> PositiveRay::get_blockers(Square serialised_piece,
                                             Bitboard occupied_squares) {

    Bitboard pieces_in_ray;
    std::vector<Square> blockers;

    pieces_in_ray = ray_maps[serialised_piece] & occupied_squares;
    blockers = bitboard::scan_forward(pieces_in_ray);

    return blockers;
}

std::vector<Square> NegativeRay::get_blockers(Square serialised_piece,
                                             Bitboard occupied_squares) {

    Bitboard pieces_in_ray;
    std::vector<Square> blockers;

    pieces_in_ray = ray_maps[serialised_piece] & occupied_squares;
    blockers = bitboard::scan_backward(pieces_in_ray);

    return blockers;
}

NorthRay::NorthRay() {

    // NorthRay generation is straight forward. The attack map generation starts 
    // at a1. The north ray from a1 is all of the squares in the a file that are 
    // not a1. This can be generated as the union of the set of squares in the a 
    // file and the set of squares that are not on the 1st rank. This union is 
    // implemented as the bit-wise AND of the bitmasks FILE_A and NOT_RANK_1.
    //
    // After this has been generated is right shifted so that its source is at 
    // the requires source square.
    //
    //          ray_maps[0]                                  ray_maps[10]
    //  (FILE_A & NOT_RANK_1) << 0                   (FILE_A & NOT_RANK_1) << 10              
    //     8   1 0 0 0 0 0 0 0                           8   0 0 1 0 0 0 0 0
    //     7   1 0 0 0 0 0 0 0                           7   0 0 1 0 0 0 0 0
    //     6   1 0 0 0 0 0 0 0      RIGHT SHIFT (>>)     6   0 0 1 0 0 0 0 0
    //     5   1 0 0 0 0 0 0 0          BY 10            5   0 0 1 0 0 0 0 0
    //     4   1 0 0 0 0 0 0 0       ----------->        4   0 0 1 0 0 0 0 0
    //     3   1 0 0 0 0 0 0 0                           3   0 0 1 0 0 0 0 0
    //     2   1 0 0 0 0 0 0 0                           2   0 0 x 0 0 0 0 0
    //     1   x 0 0 0 0 0 0 0                           1   0 0 0 0 0 0 0 0
    //
    //         a b c d e f g h                               a b c d e f g h

    ray_maps[0] = bitboard::FILE_A & bitboard::NOT_RANK_1; // set map for a1
    for (size_t i = 1; i < 8; i++) {
        ray_maps[i] = bitboard::east_one(ray_maps[i - 1]);
    }

    // Now that the first rank attack maps have been generated, the maps for the 
    // remaining 56 squares of the board are generated. This is done by north 
    // shifting the attack map for the square to the south of the current square.
    for (size_t i = 8; i < 64; i++) {
        ray_maps[i] = bitboard::north_one(ray_maps[i - 8]);
    }

}

EastRay::EastRay() {

    // East/West rays are slightly more complicated to generate than the 
    // North/South rays because of wrapping that occurs due to the uint64_t 
    // representation that is used for the board.
    //
    // The generation of the attack map bitboards is done in two stages. First, 
    // the attack maps are generated for the eight squares on the 1st rank.  
    // These attack maps are then north shifted to genertate the maps for the 
    // remaining 56 squares on the 2nd to 8th ranks.
    //
    // The first rank generation begins by generating the east ray attack map 
    // for the square a1. The map for the square a1 is the entire 1st rank 
    // excluding a1 (a piece does not attack the square that it is currently 
    // on). 
    //
    // This attack map can be generated by the union of the set of all squares 
    // that are on the 1st rank and the set of all squares that are not on the 
    // a file. This union is performed as the bit-wise AND of the RANK_1 bitmask 
    // and the NOT_FILE_A bitmask.
    // 
    // The next map generated will be a2, which is the a1 bitboard shifted east 
    // by one square. This process is repeated for every square on the 1st rank.
    //
    //
    //          ray_maps[0]                                  ray_maps[4]
    //     (RANK_1 & NOT_FILE_A)                       (FILE_A & NOT_RANK_1) << 4              
    //     8   0 0 0 0 0 0 0 0                           8   0 0 0 0 0 0 0 0
    //     7   0 0 0 0 0 0 0 0  rm[1]=east_one(rm[0])    7   0 0 0 0 0 0 0 0
    //     6   0 0 0 0 0 0 0 0  rm[2]=east_one(rm[1])    6   0 0 0 0 0 0 0 0
    //     5   0 0 0 0 0 0 0 0  rm[3]=east_one(rm[2])    5   0 0 0 0 0 0 0 0
    //     4   0 0 0 0 0 0 0 0  rm[4]=east_one(rm[3])    4   0 0 0 0 0 0 0 0
    //     3   0 0 0 0 0 0 0 0       ----------->        3   0 0 0 0 0 0 0 0
    //     2   0 0 0 0 0 0 0 0                           2   0 0 0 0 0 0 0 0
    //     1   x 1 1 1 1 1 1 1                           1   0 0 0 0 x 1 1 1
    //
    //         a b c d e f g h                               a b c d e f g h
    //
    //          ray_maps[4]                                  ray_maps[4 + 8]
    //     8   0 0 0 0 0 0 0 0                           8   0 0 0 0 0 0 0 0
    //     7   0 0 0 0 0 0 0 0                           7   0 0 0 0 0 0 0 0
    //     6   0 0 0 0 0 0 0 0      north_one()          6   0 0 0 0 0 0 0 0
    //     5   0 0 0 0 0 0 0 0                           5   0 0 0 0 0 0 0 0
    //     4   0 0 0 0 0 0 0 0       ----------->        4   0 0 0 0 0 0 0 0
    //     3   0 0 0 0 0 0 0 0                           3   0 0 0 0 0 0 0 0
    //     2   0 0 0 0 0 0 0 0                           2   0 0 0 0 x 1 1 1
    //     1   0 0 0 0 x 1 1 1                           1   0 0 0 0 0 0 0 0
    //
    //         a b c d e f g h                               a b c d e f g h

    ray_maps[0] = bitboard::RANK_1 & bitboard::NOT_FILE_A; // set map for a1
    for (size_t i = 1; i < 8; i++) {
        ray_maps[i] = bitboard::east_one(ray_maps[i - 1]);
    }

    // Now that the first rank attack maps have been generated, the maps for the 
    // remaining 56 squares of the board are generated. This is done by north 
    // shifting the attack map for the square to the south of the current square.
    for (size_t i = 8; i < 64; i++) {
        ray_maps[i] = bitboard::north_one(ray_maps[i - 8]);
    }
}

SouthRay::SouthRay() {

    for (size_t i = 0; i < 64; i++) {
        ray_maps[63 - i] = (bitboard::FILE_H & bitboard::NOT_RANK_8) >> i;
    }
}

WestRay::WestRay() {

    // This implementation is similar to that for the EastRay, except instead of
    // starting at the 1st rank and square a1, the WestRay construction starts 
    // at the 8th rank and square h8.
    //
    // The entire 8th rank is generated first and then the rest of the board is 
    // generated by south shifting the ray maps for the 8th rank.
    ray_maps[63] = bitboard::RANK_8 & bitboard::NOT_FILE_H;
    for (size_t i = 1; i < 8; i++) {
        ray_maps[63 - i] = bitboard::west_one(ray_maps[64 - i]);
    }

    for (size_t i = 0; i < 56; i++) {
        ray_maps[55 - i] = bitboard::south_one(ray_maps[63 - i]);
    }
}

NorthEastRay::NorthEastRay() {
    ray_maps[0] = bitboard::DIAG_A1_H8 & bitboard::NOT_FILE_A;
    for (size_t i = 1; i < 8; i++) {
        ray_maps[i] = bitboard::east_one(ray_maps[i - 1]);
    }

    for (size_t i = 8; i < 64; i++) {
        ray_maps[i] = bitboard::north_one(ray_maps[i - 8]);
    }
}

SouthEastRay::SouthEastRay() {
    ray_maps[56] = bitboard::DIAG_A8_H1 & bitboard::NOT_FILE_A;
    for (size_t i = 1; i < 8; i++) {
        ray_maps[56 + i] = bitboard::east_one(ray_maps[55 + i]);
    }

    for (size_t i = 0; i < 56; i++) {
        ray_maps[55 - i] = bitboard::south_one(ray_maps[63 - i]);
    }
};

NorthWestRay::NorthWestRay() {
    ray_maps[7] = bitboard::DIAG_A8_H1 & bitboard::NOT_FILE_H;
    for (size_t i = 1; i < 8; i++) {
        ray_maps[7 - i] = bitboard::west_one(ray_maps[8 - i]);
    }

    for (size_t i = 8; i < 64; i++) {
        ray_maps[i] = bitboard::north_one(ray_maps[i - 8]);
    }
};

SouthWestRay::SouthWestRay() {
    ray_maps[63] = bitboard::DIAG_A1_H8 & bitboard::NOT_FILE_H;
    for (size_t i = 1; i < 8; i++) {
        ray_maps[63 - i] = bitboard::west_one(ray_maps[64 - i]);
    }

    for (size_t i = 0; i < 56; i++) {
        ray_maps[55 - i] = bitboard::south_one(ray_maps[63 - i]);
    }
};

} // namespace attacks