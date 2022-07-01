#ifndef YAK_ATTACK_MAPS_H_
#define YAK_ATTACK_MAPS_H_

#include <stdint.h>
#include "bitboard.h"
#include <stack>
#include <array>

namespace faster {

    template<RayType type>
    Square get_blocker_in_ray(Bitboard ray) 
    {
        if (type == RayType::POSITIVE)
            return bitboard::LS1B(ray);
        else
            return bitboard::MS1B(ray);

    }

    template<Direction D>
    constexpr Bitboard shifta(Bitboard board) {
        if (D == Direction::NORTH) return board << 8;
        if (D == Direction::SOUTH) return board >> 8;
        if (D == Direction::EAST) return (board << 1) & bitboard::NOT_FILE_A;
        if (D == Direction::WEST) return (board >> 1) & bitboard::NOT_FILE_H;
        if (D == Direction::NORTH_EAST) return (board << 9) & bitboard::NOT_FILE_A;
        if (D == Direction::SOUTH_EAST) return (board >> 7) & bitboard::NOT_FILE_A;
        if (D == Direction::NORTH_WEST) return (board << 7) & bitboard::NOT_FILE_H;
        if (D == Direction::SOUTH_WEST) return (board >> 9) & bitboard::NOT_FILE_H;
        return board;
    };


    /* ------------------------------------------------------------------ */
    /* GENERATE STATIC TABLE FOR RAY MAP LOOKUP                           */
    /* ------------------------------------------------------------------ */

    /**
     * \brief Metafunction for ray generation.
     * \tparam D - Ray direction.
     * \tparam S - Square of the piece casting the ray.
     */
    template<Direction D, Square S, Bitboard B = shifta<D>(Bitboard{ 1 } << S) >
    struct ray {
        static constexpr Bitboard value = B | ray<D, S, shifta<D>(B)>::value;
    };

    /**
     * \brief Base case specialisation when bitboard is equal to 0.
     * \tparam D - Ray direction.
     * \tparam S - Square of the piece casting the ray.
     */
    template<Direction D, Square S>
    struct ray<D, S, 0> {
        static constexpr Bitboard value{ 0 };
    };

    /**
     * \brief Static table of sliding piece rays.
     * \tparam D - The direction of the ray.
     * \details
     * Usage: ray_map<Direction::NORTH>::value[42];
     */
    template<Direction D, Square S = 0, Bitboard... B>
    struct ray_map : ray_map<D, S + 1, B..., ray<D, S>::value> { };

    template<Direction D, Bitboard... B>
    struct ray_map<D, 63, B...> {
        static constexpr std::array<Bitboard, 64> value = { B... };
    };

    template<Direction D> struct is_positive_ray : std::true_type { };
    template<> struct is_positive_ray<Direction::SOUTH> : std::false_type { };
    template<> struct is_positive_ray<Direction::WEST> : std::false_type { };
    template<> struct is_positive_ray<Direction::SOUTH_EAST> : std::false_type { };
    template<> struct is_positive_ray<Direction::SOUTH_WEST> : std::false_type { };

    template<Direction D>
    Bitboard blocked_ray(Square square, Bitboard occupied)
    {
        Bitboard pieces_in_ray = ray_map<D>::value[square] & occupied;

        if (pieces_in_ray == 0) return ray_map<D>::value[square];

        Square blocker_square{ 0 };

        if (is_positive_ray<D>::value) {
            blocker_square = bitboard::LS1B(pieces_in_ray);
        }
        else {
            blocker_square = bitboard::MS1B(pieces_in_ray);
        }
        return ray_map<D>::value[square] ^ ray_map<D>::value[blocker_square];
    }

    /**
     * \brief Attack map for rooks.
     */
    class RookMap
    {
    public:
        /**
         * \brief Calculates all squares attacked by a single rook. 
         * \param[in] square - the index of the square that the rook is on.
         * \param[in] occupied_squares - a Bitboard of every occupied square.
         * \return A Bitboard of all squares attacked by the rook.
         * 
         * \summary The returned bitboard will included as attacked squares 
         * all blocking pieces, whether they are friendly or not.
         */
        static Bitboard attacks(Square square, Bitboard occupied_squares)
        {
            Bitboard atk_bb{ 0 };
            atk_bb |= blocked_ray<Direction::NORTH>(square, occupied_squares);
            atk_bb |= blocked_ray<Direction::EAST>(square, occupied_squares);
            atk_bb |= blocked_ray<Direction::SOUTH>(square, occupied_squares);
            atk_bb |= blocked_ray<Direction::WEST>(square, occupied_squares);
            return atk_bb;
        }
    };

    class BishopMap
    {
    public:
        static Bitboard attacks(Square square, Bitboard occupied_squares)
        {
            Bitboard atk_bb{ 0 };
            atk_bb |= blocked_ray<Direction::NORTH_EAST>(square, occupied_squares);
            atk_bb |= blocked_ray<Direction::NORTH_WEST>(square, occupied_squares);
            atk_bb |= blocked_ray<Direction::SOUTH_EAST>(square, occupied_squares);
            atk_bb |= blocked_ray<Direction::SOUTH_WEST>(square, occupied_squares);
            return atk_bb;
        }
    };

    class QueenMap
    {
        public:
        static Bitboard attacks(Square square, Bitboard occupied_squares)
        {
            Bitboard atk_bb{ 0 };
            atk_bb |= blocked_ray<Direction::NORTH>(square, occupied_squares);
            atk_bb |= blocked_ray<Direction::EAST>(square, occupied_squares);
            atk_bb |= blocked_ray<Direction::SOUTH>(square, occupied_squares);
            atk_bb |= blocked_ray<Direction::WEST>(square, occupied_squares);
            atk_bb |= blocked_ray<Direction::NORTH_EAST>(square, occupied_squares);
            atk_bb |= blocked_ray<Direction::NORTH_WEST>(square, occupied_squares);
            atk_bb |= blocked_ray<Direction::SOUTH_EAST>(square, occupied_squares);
            atk_bb |= blocked_ray<Direction::SOUTH_WEST>(square, occupied_squares);
            return atk_bb;
        }
    };
    /**
     * \brief Returns the corner in which to start generating a ray map.
     * \param[in] dir - the direction of the ray.
     * \return Square index of the corner to start generating
     */
    constexpr Square map_start_index(Direction dir)
    {
        if (dir == Direction::NORTH || dir == Direction::EAST || dir == Direction::NORTH_EAST || dir == Direction::NORTH_WEST) return 0;
        if (dir == Direction::SOUTH || dir == Direction::WEST || dir == Direction::SOUTH_WEST || dir == Direction::SOUTH_EAST) return 56;
    }

    /**
     * \brief Returns the index at which to start populating the temp array.
     * \param[in] dir - the direction of the ray.
     * \return The index of the temp array (0 or 7)
     */
    constexpr Square temp_start_index(Direction dir)
    {
        if (dir == Direction::NORTH || dir == Direction::EAST || dir == Direction::NORTH_EAST || dir == Direction::SOUTH_EAST) return 0;
        if (dir == Direction::SOUTH || dir == Direction::WEST || dir == Direction::SOUTH_WEST || dir == Direction::NORTH_WEST) return 7;
    }


    /**
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
    template<Direction DIR>
    Bitboard ray_gen_first_shift(Bitboard board)
    {
        if (DIR == Direction::NORTH || DIR == Direction::EAST || DIR == Direction::NORTH_EAST || DIR == Direction::SOUTH_EAST)
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
     * \tparam DIR - The direction of the ray.
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

} // namespace attacks


#endif // YAK_ATTACK_MAPS_H_