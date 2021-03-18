#ifndef YAK_ATTACK_MAPS_H_
#define YAK_ATTACK_MAPS_H_

#include <stdint.h>
#include "bitboard.h"

namespace attacks {

class Ray {
    public:
        virtual uint64_t get(int serialised_piece, uint64_t occupied_squares) = 0;
};

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
class NorthRay: public PositiveRay {
    public:
        NorthRay();
};

// EastRay provides attack rays extending to the east of a sliding piece, 
// where east refers to the direction on the board from the a file to the h file.
class EastRay: public PositiveRay {
    public:
        EastRay();
};

class SouthRay: public NegativeRay {
    public:
        SouthRay();
};

class WestRay: public NegativeRay {
    public:
        WestRay();
};

} // namespace attacks


#endif // YAK_ATTACK_MAPS_H_