#ifndef YAK_ATTACK_MAPS_H_
#define YAK_ATTACK_MAPS_H_

#include <stdint.h>
#include "bitboard.h"
#include <stack>

namespace faster {
    template<RayType type>
    Square get_blocker_in_ray(Bitboard ray) 
    {
        unsigned long idx;
        if (type == RayType::POSITIVE)
            _BitScanForward64(&idx, ray);
        else
            _BitScanReverse64(&idx, ray);

        return idx;
    }

    /*
     * \brief Returns the corner in which to start generating a ray map.
     * \param[in] dir - the direction of the ray.
     * \return Square index of the corner to start generating
     */
    constexpr Square map_start_index(Direction dir)
    {
        if (dir == Direction::NORTH || dir == Direction::EAST || dir == Direction::NORTH_EAST || dir == Direction::NORTH_WEST) return 0;
        if (dir == Direction::SOUTH || dir == Direction::WEST || dir == Direction::SOUTH_WEST || dir == Direction::SOUTH_EAST) return 56;
    }

    /*
     * \brief Returns the index at which to start populating the temp array.
     * \param[in] dir - the direction of the ray.
     * \return The index of the temp array (0 or 7)
     */
    constexpr Square temp_start_index(Direction dir)
    {
        if (dir == Direction::NORTH || dir == Direction::EAST || dir == Direction::NORTH_EAST || dir == Direction::SOUTH_EAST) return 0;
        if (dir == Direction::SOUTH || dir == Direction::WEST || dir == Direction::SOUTH_WEST || dir == Direction::NORTH_WEST) return 7;
    }


    /*
     * \brief Shifts the index of the temp array one place left or right depending on direction.
     * \param dir - The direction of the ray.
     * \param[in] square - the index to shift.
     * \return the shifted index.
     */
    template<Direction dir>
    constexpr Square temp_shift(Square square)
    {
        if (dir == Direction::NORTH || dir == Direction::EAST || dir == Direction::NORTH_EAST || dir == Direction::SOUTH_EAST) return square + 1;
        if (dir == Direction::SOUTH || dir == Direction::WEST || dir == Direction::SOUTH_WEST || dir == Direction::NORTH_WEST) return square - 1;
    }

    template<Direction dir>
    constexpr Square second_shift(Square square)
    {
        if (dir == Direction::NORTH || dir == Direction::EAST || dir == Direction::NORTH_EAST || dir == Direction::NORTH_WEST)
            return shift<Direction::NORTH>(square);

        return shift<Direction::SOUTH>(square);
    }

    /*
     * \brief Converts direction to ray type.
     * \param dir - the direction of the ray.
     * \return The ray type (POSITIVE or NEGATIVE).
     */
    template<Direction dir>
    constexpr RayType direction_to_type()
    {
        if (dir == Direction::NORTH || dir == Direction::EAST || dir == Direction::NORTH_EAST || dir == Direction::NORTH_WEST)
            return RayType::POSITIVE;
        else if (dir == Direction::SOUTH || dir == Direction::WEST || dir == Direction::SOUTH_EAST || dir == Direction::SOUTH_WEST)
            return RayType::NEGATIVE;
    }

    /*
     * \brief Shifts a bitboard for the first stage of ray map generation.
     * \param dir - the direction of the ray.
     * \param[in] board - the bitboard to be shifted.
     * \return The shifted bitboard.
     */
    template<Direction dir>
    Bitboard ray_gen_first_shift(Bitboard board)
    {
        if (dir == Direction::NORTH || dir == Direction::EAST || dir == Direction::NORTH_EAST || dir == Direction::SOUTH_EAST)
            return bitboard::east_one(board);

        return bitboard::west_one(board);
    }

    /*
     * \brief Shifts a bitboard for the second stage of ray map generation.
     * \param dir - the direction of the ray.
     * \param[in] board - the bitboard to be shifted.
     * \return The shifted bitboard.
     */
    template<Direction dir>
    Bitboard ray_gen_second_shift(Bitboard board)
    {
        if (dir == Direction::NORTH || dir == Direction::EAST || dir == Direction::NORTH_EAST || dir == Direction::NORTH_WEST)
            return bitboard::north_one(board);

        return bitboard::south_one(board);
    }

    template<Direction dir>
    Square shift(Square square)
    {
        if (dir == Direction::NORTH) return square + 8;
        if (dir == Direction::SOUTH) return square - 8;
        if (dir == Direction::EAST) return square + 1;
        if (dir == Direction::WEST) return square - 1;
        if (dir == Direction::NORTH_EAST) return square + 9;
        if (dir == Direction::NORTH_WEST) return square + 7;
        if (dir == Direction::SOUTH_EAST) return square - 7;
        if (dir == Direction::SOUTH_WEST) return square - 9;
    }

    template<Direction DIR>
    constexpr Bitboard ray_seed()
    {
        if (DIR == Direction::NORTH) return bitboard::FILE_A & bitboard::NOT_RANK_1;
        if (DIR == Direction::SOUTH) return bitboard::FILE_H & bitboard::NOT_RANK_8;
        if (DIR == Direction::EAST) return bitboard::RANK_1 & bitboard::NOT_FILE_A;
        if (DIR == Direction::WEST) return bitboard::RANK_8 & bitboard::NOT_FILE_H;
        if (DIR == Direction::NORTH_EAST) return bitboard::DIAG_A1_H8 & bitboard::NOT_FILE_A;
        if (DIR == Direction::NORTH_WEST) return bitboard::DIAG_A8_H1 & bitboard::NOT_FILE_H;
        if (DIR == Direction::SOUTH_EAST) return bitboard::DIAG_A8_H1 & bitboard::NOT_FILE_A;
        if (DIR == Direction::SOUTH_WEST) return bitboard::DIAG_A1_H8 & bitboard::NOT_FILE_H;
    }

    /*
     * \brief Generate a ray attack map.
     * \param dir - template parameter, the direction of the ray.
     * \param[out] out - 64 element array of Bitboard
     * \param[in] a1_ray - The seed ray from which to generate the maps.
     */
    template<Direction dir>
    void generate_ray(Bitboard out[64], Bitboard a1_ray)
    {
        // Initialise a temporary array (represents a single rank of the chess board).    
        Bitboard temp[8] = { 0 };

        // Load the first ray into temp
        temp[temp_start_index(dir)] = a1_ray;

        // Use this initial ray to populate the entire of the temp array.

        Square read_index = temp_start_index(dir);
        for (size_t i = 0; i < 7; i++) {
            Square write_index = temp_shift<dir>(read_index);               // Create write index by shifting the read index.
            temp[write_index] = ray_gen_first_shift<dir>(temp[read_index]); // Create a new ray as a shifted version of a previous ray.
            read_index = temp_shift<dir>(read_index);                       // Update the read_index.
        }

        // Copy the temp array into the map.
        Square start_square = map_start_index(dir);
        std::memcpy(&out[start_square], &temp, sizeof(Bitboard) * 8);

        // Shift the bitboards in the temp array north or south to populate the rest of the map.
        for (size_t i = 0; i < 7; i++)
        {

            temp[0] = ray_gen_second_shift<dir>(temp[0]);
            temp[1] = ray_gen_second_shift<dir>(temp[1]);
            temp[2] = ray_gen_second_shift<dir>(temp[2]);
            temp[3] = ray_gen_second_shift<dir>(temp[3]);
            temp[4] = ray_gen_second_shift<dir>(temp[4]);
            temp[5] = ray_gen_second_shift<dir>(temp[5]);
            temp[6] = ray_gen_second_shift<dir>(temp[6]);
            temp[7] = ray_gen_second_shift<dir>(temp[7]);

            start_square = second_shift<dir>(start_square);
            std::memcpy(&out[start_square], &temp, sizeof(Bitboard) * 8);
        }
    }

    class Ray {
    public:
        Ray() {}
        virtual ~Ray() {}
        virtual Bitboard get(Square square, Bitboard occupied_squares) const = 0;
    };

    /*
     * \brief A sliding piece attack ray with direction DIR.
     */
    template<Direction DIR>
    class SpecialisedRay : public Ray
    {
    private:
        Bitboard map[64];
    public:
        /* \brief Default constructor. */
        SpecialisedRay() { generate_ray<DIR>(map, ray_seed<DIR>()); }

        /* \brief Default destructor. */
        ~SpecialisedRay() = default;

        /*
         * \brief Get the attack ray from the given square upto the first occupied square.
         * \param[in] square - the square of the piece casting the ray.
         * \param[in] occupied_squares - Bitboard of all occupied_Squares on the board.
         * \return Bitboard of the computed ray.
         */
        Bitboard get(Square square, Bitboard occupied_squares) const override
        {
            Bitboard pieces_in_ray = map[square] & occupied_squares;

            if (pieces_in_ray == 0) return map[square];

            Square blocker = get_blocker_in_ray<direction_to_type<DIR>()>(pieces_in_ray);
            return map[square] ^ map[blocker];
        }
    };

    class SlidingPieceAttackMap 
    {
    public:
        SlidingPieceAttackMap() {}
        virtual ~SlidingPieceAttackMap() {}
        virtual Bitboard attacks(Square square, Bitboard occupied_squares) const = 0;
    };

    class RookAttackMap : public SlidingPieceAttackMap
    {
    private:
        std::vector<Ray*> rays;
    public:
        RookAttackMap()
        {
            rays.push_back(new SpecialisedRay<Direction::NORTH>());
            rays.push_back(new SpecialisedRay<Direction::EAST>());
            rays.push_back(new SpecialisedRay<Direction::SOUTH>());
            rays.push_back(new SpecialisedRay<Direction::WEST>());
        }

        ~RookAttackMap() 
        {
            for (auto ray : rays) 
                delete ray;
        }

        RookAttackMap(const RookAttackMap&) = delete;
        RookAttackMap& operator= (const RookAttackMap&) = delete;

        Bitboard attacks(Square square, Bitboard occupied_squares) const override
        {
            Bitboard atk_bb{ 0 };
            for (auto ray : rays)
                atk_bb |= ray->get(square, occupied_squares);

            return atk_bb;
        }
    };

    class BishopAttackMap : public SlidingPieceAttackMap
    {
    private:
        std::vector<Ray*> rays;
    public:
        BishopAttackMap()
        {
            rays.push_back(new SpecialisedRay<Direction::NORTH_EAST>());
            rays.push_back(new SpecialisedRay<Direction::NORTH_WEST>());
            rays.push_back(new SpecialisedRay<Direction::SOUTH_EAST>());
            rays.push_back(new SpecialisedRay<Direction::SOUTH_WEST>());
        }

        ~BishopAttackMap()
        {
            for (auto ray : rays)
                delete ray;
        }

        BishopAttackMap(const BishopAttackMap&) = delete;
        BishopAttackMap& operator= (const BishopAttackMap&) = delete;

        Bitboard attacks(Square square, Bitboard occupied_squares) const override
        {
            Bitboard atk_bb{ 0 };
            for (auto ray : rays)
                atk_bb |= ray->get(square, occupied_squares);

            return atk_bb;
        }
    };

    class QueenAttackMap : public SlidingPieceAttackMap
    {
    private:
        std::vector<Ray*> rays;
    public:
        QueenAttackMap()
        {
            rays.push_back(new SpecialisedRay<Direction::NORTH>());
            rays.push_back(new SpecialisedRay<Direction::EAST>());
            rays.push_back(new SpecialisedRay<Direction::SOUTH>());
            rays.push_back(new SpecialisedRay<Direction::WEST>());
            rays.push_back(new SpecialisedRay<Direction::NORTH_EAST>());
            rays.push_back(new SpecialisedRay<Direction::NORTH_WEST>());
            rays.push_back(new SpecialisedRay<Direction::SOUTH_EAST>());
            rays.push_back(new SpecialisedRay<Direction::SOUTH_WEST>());
        }

        ~QueenAttackMap()
        {
            for (auto ray : rays)
                delete ray;
        }

        QueenAttackMap(const QueenAttackMap&) = delete;
        QueenAttackMap& operator= (const QueenAttackMap&) = delete;

        Bitboard attacks(Square square, Bitboard occupied_squares) const override
        {
            Bitboard atk_bb{ 0 };
            for (auto ray : rays)
                atk_bb |= ray->get(square, occupied_squares);

            return atk_bb;
        }
    };
}

namespace attacks {

    

    class AttackMap {
    public:
        AttackMap() {}
        virtual ~AttackMap() {}
        virtual Bitboard get(Square serialised_piece) = 0;
    };

    class KnightAttacks : public AttackMap {
        private:
            Bitboard maps[64] = { 0 };

        public:
            KnightAttacks();
            Bitboard get(Square serialised_piece) override;
    };

    class KingAttacks : public AttackMap {
        private:
            Bitboard maps[64] = { 0 };
        public:
            KingAttacks();
            Bitboard get(Square serialised_piece) override;
    };

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
        

    public:
        // Returns a bitboard of every square attacked by the serialied_piece.
        Bitboard get(Square serialised_piece, Bitboard occupied_squares);
        virtual std::vector<Square> get_blockers(Square serialised_piece, Bitboard occupied_squares);
};

// PositiveRays are those that extend in the North, and East directions.
class PositiveRay: virtual public Ray {
    public:
        std::vector<Square>get_blockers(Square serialised_piece, Bitboard occupied_squares) override;
};

// NegativeRay are those that extend in the South, and West directions.
class NegativeRay: virtual public Ray {
    public:
        // Returns the square index of the first blocker in the ray extending
        // from the square index: serialised_piece.
        std::vector<Square>get_blockers(Square serialised_piece, Bitboard occupied_squares) override;
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