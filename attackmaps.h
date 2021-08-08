#ifndef YAK_ATTACK_MAPS_H_
#define YAK_ATTACK_MAPS_H_

#include <stdint.h>
#include "bitboard.h"

namespace attacks {

// Ray is a pure virtual class that serves as the base class for attack ray 
// generation. Attack rays are used to generate attack maps for the sliding 
// pieces (Bishop, Rook, and Queen).
//
// Rays are generated 
class Ray {
    protected:
        // Ray map array, contains a bitboard map of this ray for all 64 squares 
        // of the chessboard.
        Bitboard ray_maps[64] = {0};

    private:
        // Returns the square index of the first blocker in the ray extending
        // from the square index: serialised_piece.
        virtual std::stack<Square> get_blockers(Square serialised_piece, Bitboard occupied_squares);

    public:
        // Returns a bitboard of every square attacked by the serialied_piece.
        virtual Bitboard get(Square serialised_piece, Bitboard occupied_squares);
};

// PositiveRays are those that extend in the North, and East directions.
class PositiveRay: virtual public Ray {
    private:
        // Returns the square index of the first blocker in the ray extending
        // from the square index: serialised_piece.
        std::stack<Square> get_blockers(Square serialised_piece, Bitboard occupied_squares) override;
};

// NegativeRay are those that extend in the South, and West directions.
class NegativeRay: virtual public Ray {
    private:
        // Returns the square index of the first blocker in the ray extending
        // from the square index: serialised_piece.
        std::stack<Square> get_blockers(Square serialised_piece, Bitboard occupied_squares) override;
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

class NorthEastRay: public PositiveRay {
    public:
        NorthEastRay();
};

class SouthEastRay: public NegativeRay {
    public:
        SouthEastRay();
};

class NorthWestRay: public PositiveRay {
    public:
        NorthWestRay();
};

class SouthWestRay: public NegativeRay {
    public:
        SouthWestRay();
};

} // namespace attacks


#endif // YAK_ATTACK_MAPS_H_