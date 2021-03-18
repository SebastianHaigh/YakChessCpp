#ifndef YAK_ATTACK_MAPS_H_
#define YAK_ATTACK_MAPS_H_

#include <stdint.h>
#include "bitboard.h"

namespace attacks {

// Ray is a pure virtual class that serves as the base class for attack ray 
// generation. Attack rays are used to generate attack maps for the sliding 
// pieces (Bishop, Rook, and Queen).
class Ray {
    public:
        virtual uint64_t get(int serialised_piece, uint64_t occupied_squares) = 0;
};

// PositiveRays are those that extend in the North, and West directions.
class PositiveRay: virtual public Ray {
    protected:

        // Ray map array, contains a bitboard map of this ray for all 64 squares 
        // of the chessboard.
        uint64_t ray_maps[64] = {0};

    private:

        // Returns the square index of the first blocker in the ray extending
        // from the square index: serialised_piece.
        int get_blocker(int serialised_piece, uint64_t occupied_squares);
    public:

        // Returns a bitboard of every square attacked by the serialied_piece.
        uint64_t get(int serialised_piece, uint64_t occupied_squares) override;
};

// NegativeRay are those that extend in the South, and East directions.
class NegativeRay: virtual public Ray {
    protected:

        // Ray map array, contains a bitboard map of this ray for all 64 squares 
        // of the chessboard.
        uint64_t ray_maps[64] = {0};
    private:

        // Returns the square index of the first blocker in the ray extending
        // from the square index: serialised_piece.
        int get_blocker(int serialised_piece, uint64_t occupied_squares);
    public:

        // Returns a bitboard of every square attacked by the serialied_piece.
        uint64_t get(int serialised_piece, uint64_t occupied_squares) override;
};


// NorthRay provides attack rays extending to the north of a sliding piece, 
// where north refers to the direction on the board from the 1st rank to the 
// 8th rank.
// Example:
//     std::unique_ptr <NorthRay> ray = std::new_unique <NorthRay>();
//     int square = 19;
//     uitn64_t occupied_squares = bitboard::RANK_3 & bitboard::FILE_D;
//     bitboard::print_board(ray->get(square, occupied_squares));
//
//     0 0 0 1 0 0 0 0
//     0 0 0 1 0 0 0 0
//     0 0 0 1 0 0 0 0
//     0 0 0 1 0 0 0 0
//     0 0 0 1 0 0 0 0
//     0 0 0 0 0 0 0 0
//     0 0 0 0 0 0 0 0
//     0 0 0 0 0 0 0 0
class NorthRay: public PositiveRay {
    public:
        NorthRay();
};

// EastRay provides attack rays extending to the east of a sliding piece, 
// where east refers to the direction on the board from the a file to the h file.
// Example:
//     std::unique_ptr <EastRay> ray = std::new_unique <EastRay>();
//     int square = 19;
//     uitn64_t occupied_squares = bitboard::RANK_3 & bitboard::FILE_D;
//     bitboard::print_board(ray->get(square, occupied_squares));
//
//     0 0 0 0 0 0 0 0
//     0 0 0 0 0 0 0 0
//     0 0 0 0 0 0 0 0
//     0 0 0 0 0 0 0 0
//     0 0 0 0 0 0 0 0
//     0 0 0 0 1 1 1 1
//     0 0 0 0 0 0 0 0
//     0 0 0 0 0 0 0 0
class EastRay: public PositiveRay {
    public:
        EastRay();
};

// SouthRay provides attack rays extending to the south of a sliding piece, 
// where south refers to the direction on the board from the 8th rank to the 1st 
// rank.
// Example:
//     std::unique_ptr <SouthRay> ray = std::new_unique <SouthRay>();
//     int square = 19;
//     uitn64_t occupied_squares = bitboard::RANK_3 & bitboard::FILE_D;
//     bitboard::print_board(ray->get(square, occupied_squares));
//
//     0 0 0 0 0 0 0 0
//     0 0 0 0 0 0 0 0
//     0 0 0 0 0 0 0 0
//     0 0 0 0 0 0 0 0
//     0 0 0 0 0 0 0 0
//     0 0 0 0 0 0 0 0
//     0 0 0 1 0 0 0 0
//     0 0 0 1 0 0 0 0
class SouthRay: public NegativeRay {
    public:
        SouthRay();
};

// WestRay provides attack rays extending to the west of a sliding piece, 
// where west refers to the direction on the board from the h file to the a file.
// Example:
//     std::unique_ptr <WestRay> ray = std::new_unique <WestRay>();
//     int square = 19;
//     uitn64_t occupied_squares = bitboard::RANK_3 & bitboard::FILE_D;
//     bitboard::print_board(ray->get(square, occupied_squares));
//
//     0 0 0 0 0 0 0 0
//     0 0 0 0 0 0 0 0
//     0 0 0 0 0 0 0 0
//     0 0 0 0 0 0 0 0
//     0 0 0 0 0 0 0 0
//     1 1 1 0 0 0 0 0
//     0 0 0 0 0 0 0 0
//     0 0 0 0 0 0 0 0
class WestRay: public NegativeRay {
    public:
        WestRay();
};

} // namespace attacks


#endif // YAK_ATTACK_MAPS_H_