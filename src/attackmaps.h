#ifndef YAK_ATTACK_MAPS_H_
#define YAK_ATTACK_MAPS_H_

#include <stdint.h>
#include "bitboard.h"
#include <stack>
#include <array>

namespace faster {

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
        constexpr std::array<Bitboard, 64> rayMap = ray_map<D>::value;

        Bitboard pieces_in_ray = rayMap[square] & occupied;

        if (pieces_in_ray == 0) return rayMap[square];

        Square blocker_square{ 0 };

      if (is_positive_ray<D>::value)
        blocker_square = bitboard::LS1B(pieces_in_ray);
      else
        blocker_square = bitboard::MS1B(pieces_in_ray);

      return rayMap[square] ^ rayMap[blocker_square];
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
     * \brief Attack map for a knight on a given square.
     * \tparam S - The square the the knight is on.
     */
    template<Square S>
    struct knight_map {
        static constexpr Bitboard value = shifta<Direction::NORTH>(shifta<Direction::NORTH_EAST>(bitboard::static_bitboard<S>::value))
            | shifta<Direction::NORTH>(shifta<Direction::NORTH_WEST>(bitboard::static_bitboard<S>::value))
            | shifta<Direction::EAST>(shifta<Direction::NORTH_EAST>(bitboard::static_bitboard<S>::value))
            | shifta<Direction::EAST>(shifta<Direction::SOUTH_EAST>(bitboard::static_bitboard<S>::value))
            | shifta<Direction::SOUTH>(shifta<Direction::SOUTH_EAST>(bitboard::static_bitboard<S>::value))
            | shifta<Direction::SOUTH>(shifta<Direction::SOUTH_WEST>(bitboard::static_bitboard<S>::value))
            | shifta<Direction::WEST>(shifta<Direction::SOUTH_WEST>(bitboard::static_bitboard<S>::value))
            | shifta<Direction::WEST>(shifta<Direction::NORTH_WEST>(bitboard::static_bitboard<S>::value));
    };

    /**
     * \brief Attack map for a king on a given square.
     * \tparam S - The square the the king is on.
     */
    template<Square S>
    struct king_map {
        static constexpr Bitboard value = shifta<Direction::NORTH>(bitboard::static_bitboard<S>::value)
            | shifta<Direction::EAST>(bitboard::static_bitboard<S>::value)
            | shifta<Direction::SOUTH>(bitboard::static_bitboard<S>::value)
            | shifta<Direction::WEST>(bitboard::static_bitboard<S>::value)
            | shifta<Direction::NORTH_EAST>(bitboard::static_bitboard<S>::value)
            | shifta<Direction::NORTH_WEST>(bitboard::static_bitboard<S>::value)
            | shifta<Direction::SOUTH_EAST>(bitboard::static_bitboard<S>::value)
            | shifta<Direction::SOUTH_WEST>(bitboard::static_bitboard<S>::value);
    };

} // namespace faster



#endif // YAK_ATTACK_MAPS_H_