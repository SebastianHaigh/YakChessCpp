#include "attackmaps.h"

namespace attacks {

uint64_t Ray::get(int serialised_piece, uint64_t occupied_squares) {

    uint64_t ray = 0;
    int blocker = 0;

    blocker = get_blocker(serialised_piece, occupied_squares);
    if (blocker > 0){
        ray = ray_maps[serialised_piece] ^ ray_maps[blocker];
    } else {
        ray = ray_maps[serialised_piece] ^ bitboard::EMPTY;
    }
    
    return ray;
}

int Ray::get_blocker(int serialised_piece, uint64_t occupied_squares) {

    int blocker = -1;

    return blocker;
}

int PositiveRay::get_blocker(int serialised_piece, uint64_t occupied_squares) {

    uint64_t pieces_in_ray;
    std::stack <int> serialised_pieces_in_ray;
    int blocker = 0;

    pieces_in_ray = ray_maps[serialised_piece] & occupied_squares;
    serialised_pieces_in_ray = bitboard::scan_forward(pieces_in_ray);

    if (!serialised_pieces_in_ray.empty()) {
        blocker = serialised_pieces_in_ray.top();
    } else {
        blocker = -1;
    }

    return blocker;
}

int NegativeRay::get_blocker(int serialised_piece, uint64_t occupied_squares) {

    uint64_t pieces_in_ray;
    std::stack <int> serialised_pieces_in_ray;
    int blocker = 0;

    pieces_in_ray = ray_maps[serialised_piece] & occupied_squares;
    serialised_pieces_in_ray = bitboard::scan_backward(pieces_in_ray);

    if (!serialised_pieces_in_ray.empty()) {
        blocker = serialised_pieces_in_ray.top();
    } else {
        blocker = -1;
    }

    return blocker;
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
    //     6   1 0 0 0 0 0 0 0      RIGHT SHIFT (<<)     6   0 0 1 0 0 0 0 0
    //     5   1 0 0 0 0 0 0 0          BY 10            5   0 0 1 0 0 0 0 0
    //     4   1 0 0 0 0 0 0 0       ----------->        4   0 0 1 0 0 0 0 0
    //     3   1 0 0 0 0 0 0 0                           3   0 0 1 0 0 0 0 0
    //     2   1 0 0 0 0 0 0 0                           2   0 0 x 0 0 0 0 0
    //     1   x 0 0 0 0 0 0 0                           1   0 0 0 0 0 0 0 0
    //
    //         a b c d e f g h                               a b c d e f g h

    for (size_t i = 0; i < 64; i++) {
        ray_maps[i] = (bitboard::FILE_A & bitboard::NOT_RANK_1) << i;
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
    // The next map generated will be a2, which the a1 bitboard shifted east by 
    // one square. This will cause a wrapping issue as the end of the ray will 
    // wrap around on to the 2nd rank, i.e., the bit that was previously 
    // representing an attack on a8 will now be representing an attack on b1. 
    // To get rid of this, the shifted copy of the a1 attack map is unioned with 
    // the set of all squares that are not on the 2nd rank.
    //
    // This process is repeated for every square on the 1st rank.
    //
    //
    //          ray_maps[0]                                  ray_maps[4]
    //  (RANK_1 & NOT_FILE_A) << 0                   (FILE_A & NOT_RANK_1) << 4              
    //     8   0 0 0 0 0 0 0 0                           8   0 0 0 0 0 0 0 0
    //     7   0 0 0 0 0 0 0 0                           7   0 0 0 0 0 0 0 0
    //     6   0 0 0 0 0 0 0 0      RIGHT SHIFT (<<)     6   0 0 0 0 0 0 0 0
    //     5   0 0 0 0 0 0 0 0          BY 4             5   0 0 0 0 0 0 0 0
    //     4   0 0 0 0 0 0 0 0       ----------->        4   0 0 0 0 0 0 0 0
    //     3   0 0 0 0 0 0 0 0                           3   0 0 0 0 0 0 0 0
    //     2   0 0 0 0 0 0 0 0                           2   1 1 1 1 0 0 0 0
    //     1   x 1 1 1 1 1 1 1                           1   0 0 0 0 x 1 1 1
    //
    //         a b c d e f g h                               a b c d e f g h
    // 
    //
    //          ray_maps[0]                                  ray_maps[4]
    //  ((RANK_1 & NOT_FILE_A) << 0)                ((FILE_A & NOT_RANK_1) << 4)
    //          & NOT_RANK_2                                  & NOT_RANK_2
    //     8   0 0 0 0 0 0 0 0                           8   0 0 0 0 0 0 0 0
    //     7   0 0 0 0 0 0 0 0                           7   0 0 0 0 0 0 0 0
    //     6   0 0 0 0 0 0 0 0      RIGHT SHIFT (<<)     6   0 0 0 0 0 0 0 0
    //     5   0 0 0 0 0 0 0 0          BY 4             5   0 0 0 0 0 0 0 0
    //     4   0 0 0 0 0 0 0 0       ----------->        4   0 0 0 0 0 0 0 0
    //     3   0 0 0 0 0 0 0 0                           3   0 0 0 0 0 0 0 0
    //     2   0 0 0 0 0 0 0 0                           2   0 0 0 0 0 0 0 0
    //     1   x 1 1 1 1 1 1 1                           1   0 0 0 0 x 1 1 1
    //
    //         a b c d e f g h                               a b c d e f g h
    //
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

    for (size_t i = 0; i < 8; i++) {
        ray_maps[i] = ((bitboard::RANK_1 & bitboard::NOT_FILE_A) << i) & bitboard::NOT_RANK_2;
    }

    // Now that the first rank attack maps have been generated, the maps for the 
    // remaining 56 squares of the board are generated. This is done by north 
    // shifting the attack map for the square to the south of the current square.
    for (size_t i = 0; i < 56; i++) {
        ray_maps[i + 8] = bitboard::north_one(ray_maps[i]);
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
    for (size_t i = 0; i < 8; i++) {
        ray_maps[63 - i] = ((bitboard::RANK_8 & bitboard::NOT_FILE_H) >> i) & bitboard::NOT_RANK_7;
    }

    for (size_t i = 0; i < 56; i++) {
        ray_maps[55 - i] = ray_maps[63 - i] >> 8;
    }
}

} // namespace attacks