#ifndef YAK_PIECES_H_
#define YAK_PIECES_H_

#include "bitboard.h"
#include "attackmaps.h"

#include <iostream>
#include <intrin.h>
#include <vector>
#include <memory>
#include <stack>
#include <stdint.h>
#include <stdio.h>

enum class PieceType {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    NULL_PIECE
};

enum class PieceColour {
    BLACK,
    WHITE,
    NULL_COLOUR
};



/** faster_pieces namespace for benchmark testing. */
namespace faster {

    /**
     * \brief Target squares of a pawn single push.
     * \tparam COLOUR - the colour of the pawns.
     * \param[in] source - the source squares for which to find the targets.
     * \return The Bitboard of the target squares.
     */
    template<PieceColour C>
    Bitboard pawn_single_push_target(Bitboard source)
    {
        return (C == PieceColour::WHITE) ? bitboard::north_one(source) : 
            (C == PieceColour::BLACK) ? bitboard::south_one(source) : source;
    }

    /**
     * \brief Source squares of a pawn single push.
     * \tparam COLOUR - the colour of the pawns.
     * \param[in] target - the target squares for which to find the sources.
     * \return The Bitboard of the source squares.
     */
    template<PieceColour C>
    Bitboard pawn_single_push_source(Bitboard target)
    {
        return (C == PieceColour::WHITE) ? bitboard::south_one(target) :
            (C == PieceColour::BLACK) ? bitboard::north_one(target) : target;
    }

    /**
     * \brief The target squares for a double push from the starting rank.
     * \tparam COLOUR - the colour of the pawns.
     * \return The Bitboard of the target squares.
     */
    template<PieceColour C>
    Bitboard pawn_double_push_target()
    {
        return (C == PieceColour::WHITE) ? bitboard::RANK_4 :
            (C == PieceColour::BLACK) ? bitboard::RANK_5 : bitboard::EMPTY;
    }

    /**
     * \brief Source squares of pawns attacking to the west.
     * \tparam COLOUR - the colour of the pawns.
     * \param[in] target - the target squares for which to find the sources.
     * \return The Bitboard of the source squares.
     */
    template<PieceColour C>
    Bitboard pawn_west_attack_source(Bitboard target)
    {
        return (C == PieceColour::WHITE) ? bitboard::south_east_one(target) :
            (C == PieceColour::BLACK) ? bitboard::north_east_one(target) : target;
    }

    /**
     * \brief Target squares of pawns attacking to the west.
     * \tparam COLOUR - the colour of the pawns.
     * \param[in] source - the source squares for which to find the targets.
     * \return The Bitboard of the target squares.
     */
    template<PieceColour C>
    Bitboard pawn_west_attack_target(Bitboard source)
    {
        return (C == PieceColour::WHITE) ? bitboard::north_west_one(source) :
            (C == PieceColour::BLACK) ? bitboard::south_west_one(source) : source;
    }

    /**
     * \brief Target squares of pawns attacking to the east.
     * \tparam COLOUR - the colour of the pawns.
     * \param[in] source - the source squares for which to find the targets.
     * \return The Bitboard of the target squares.
     */
    template<PieceColour C>
    Bitboard pawn_east_attack_source(Bitboard target)
    {
        return (C == PieceColour::WHITE) ? bitboard::south_west_one(target) :
            (C == PieceColour::BLACK) ? bitboard::north_west_one(target) : target;
    }

    /**
     * \brief Target squares of pawns attacking to the east.
     * \tparam COLOUR - the colour of the pawns.
     * \param[in] source - the source squares for which to find the targets.
     * \return The Bitboard of the target squares.
     */
    template<PieceColour C>
    Bitboard pawn_east_attack_target(Bitboard source)
    {
        return (C == PieceColour::WHITE) ? bitboard::north_east_one(source) :
            (C == PieceColour::BLACK) ? bitboard::south_east_one(source) : source;
    }

    template<PieceColour C>
    Bitboard promotable_pawns(Bitboard pawn_positions)
    {
        return (C == PieceColour::WHITE) ? (bitboard::RANK_7 & pawn_positions) :
            (C == PieceColour::BLACK) ? (bitboard::RANK_2 & pawn_positions) : bitboard::EMPTY;
    }

    template<PieceColour C>
    Bitboard non_promotable_pawns(Bitboard pawn_positions)
    {
        return ~promotable_pawns<C>(pawn_positions) & pawn_positions;
    }

    struct Move {
        Square from;
        Square to;
        bool capture = false;
        bool en_passant = false;
        bool double_push = false;
        Square ep_target = 0;
        PieceType castle = PieceType::NULL_PIECE;
        PieceType promotion = PieceType::NULL_PIECE;
        std::string to_algebraic() {
            return bitboard::to_algebraic(from) + bitboard::to_algebraic(to);
        }
    };

    template<PieceColour C, bool PROMOTIONS>
    inline void generate_pawn_single_pushes(Move* move_list, int& move_counter, Bitboard pawn_positions, Bitboard empty_squares)
    {
        pawn_positions = PROMOTIONS ? promotable_pawns<C>(pawn_positions) : non_promotable_pawns<C>(pawn_positions);
        Bitboard sources = pawn_single_push_source<C>(empty_squares) & pawn_positions;
        Bitboard targets = pawn_single_push_target<C>(sources);
        while (sources && !PROMOTIONS)
        {
            *move_list++ = make_quiet(bitboard::pop_LS1B(sources), bitboard::pop_LS1B(targets));
            move_counter++;
        }

        while (sources && PROMOTIONS)
        {
            Square from = bitboard::pop_LS1B(sources);
            Square to = bitboard::pop_LS1B(targets);
            *move_list++ = make_quiet_promotion(from, to, PieceType::KNIGHT);
            *move_list++ = make_quiet_promotion(from, to, PieceType::BISHOP);
            *move_list++ = make_quiet_promotion(from, to, PieceType::ROOK);
            *move_list++ = make_quiet_promotion(from, to, PieceType::QUEEN);
            move_counter += 4;
        }
    }

    template<PieceColour C, bool PROMOTIONS>
    inline void generate_pawn_double_pushes(Move* move_list, int& move_counter, Bitboard pawn_positions, Bitboard empty_squares)
    {
        Bitboard targets = pawn_double_push_target<C>() & empty_squares;
        Bitboard sources = pawn_single_push_source<C>(targets) & empty_squares;
        sources = pawn_single_push_source<C>(sources) & pawn_positions;
        while (sources)
        {
            *move_list++ = make_double_push(bitboard::pop_LS1B(sources), bitboard::pop_LS1B(targets));
            move_counter++;
        }

        
    }

    template<PieceColour C, bool PROMOTIONS>
    inline void generate_pawn_west_captures(Move* move_list, int& move_counter, Bitboard pawn_positions, Bitboard opponent_pieces)
    {
        pawn_positions = PROMOTIONS ? promotable_pawns<C>(pawn_positions) : non_promotable_pawns<C>(pawn_positions);
        Bitboard sources = pawn_west_attack_source<C>(opponent_pieces) & pawn_positions;
        Bitboard targets = pawn_west_attack_target<C>(sources);
        while (sources && !PROMOTIONS)
        {
            *move_list++ = make_capture(bitboard::pop_LS1B(sources), bitboard::pop_LS1B(targets));
            move_counter++;
        }

        while (sources && PROMOTIONS)
        {
            Square from = bitboard::pop_LS1B(sources);
            Square to = bitboard::pop_LS1B(targets);
            *move_list++ = make_capture_promotion(from, to, PieceType::KNIGHT);
            *move_list++ = make_capture_promotion(from, to, PieceType::BISHOP);
            *move_list++ = make_capture_promotion(from, to, PieceType::ROOK);
            *move_list++ = make_capture_promotion(from, to, PieceType::QUEEN);
            move_counter += 4;
        }
    }
    
    template<PieceColour C, bool PROMOTIONS>
    inline void generate_pawn_east_captures(Move* move_list, int& move_counter, Bitboard pawn_positions, Bitboard opponent_pieces)
    {
        pawn_positions = PROMOTIONS ? promotable_pawns<C>(pawn_positions) : non_promotable_pawns<C>(pawn_positions);
        Bitboard sources = pawn_east_attack_source<C>(opponent_pieces) & pawn_positions;
        Bitboard targets = pawn_east_attack_target<C>(sources);
        while (sources && !PROMOTIONS)
        {
            *move_list++ = make_capture(bitboard::pop_LS1B(sources), bitboard::pop_LS1B(targets));
            move_counter++;
        }

        while (sources && PROMOTIONS)
        {
            Square from = bitboard::pop_LS1B(sources);
            Square to = bitboard::pop_LS1B(targets);
            *move_list++ = make_capture_promotion(from, to, PieceType::KNIGHT);
            *move_list++ = make_capture_promotion(from, to, PieceType::BISHOP);
            *move_list++ = make_capture_promotion(from, to, PieceType::ROOK);
            *move_list++ = make_capture_promotion(from, to, PieceType::QUEEN);
            move_counter += 4;
        }
    }

    template<PieceColour C>
    void generate_pawn_moves(Move* move_list, int& move_counter, Bitboard pawn_positions, Bitboard empty_squares, Bitboard opponent_pieces)
    {
        /* NOT PROMOTIONS             */
        /* ----------------------------- */
        generate_pawn_single_pushes<C, false>(&move_list[move_counter], move_counter, pawn_positions, empty_squares);
        generate_pawn_double_pushes<C, false>(&move_list[move_counter], move_counter, pawn_positions, empty_squares);
        generate_pawn_west_captures<C, false>(&move_list[move_counter], move_counter, pawn_positions, opponent_pieces);
        generate_pawn_east_captures<C, false>(&move_list[move_counter], move_counter, pawn_positions, opponent_pieces);

        /* PROMOTIONS                    */
        /* ----------------------------- */
        generate_pawn_single_pushes<C, true>(&move_list[move_counter], move_counter, pawn_positions, empty_squares);
        generate_pawn_west_captures<C, true>(&move_list[move_counter], move_counter, pawn_positions, opponent_pieces);
        generate_pawn_east_captures<C, true>(&move_list[move_counter], move_counter, pawn_positions, opponent_pieces);
    };

    template<PieceColour C>
    void generate_ep_captures(Move* move_list, int& move_counter, Bitboard pawn_positions, Bitboard ep_target)
    {
        Bitboard sources = pawn_west_attack_source<C>(ep_target) & pawn_positions;
        Bitboard targets = pawn_west_attack_target<C>(sources);
        while (sources)
        {
            *move_list++ = make_ep_capture(bitboard::pop_LS1B(sources), bitboard::pop_LS1B(targets));
            move_counter++;
        }

        sources = pawn_east_attack_source<C>(ep_target) & pawn_positions;
        targets = pawn_east_attack_target<C>(sources);
        while (sources)
        {
            *move_list++ = make_ep_capture(bitboard::pop_LS1B(sources), bitboard::pop_LS1B(targets));
            move_counter++;
        }
    };

    void jumping_piece_moves(attacks::AttackMap* atk_map, Move* move_list, int& move_counter, Bitboard piece_positions, Bitboard empty_squares, Bitboard opponent_pieces);

    void generate_sliding_piece_moves(const RookMap&, Move* move_list, int& move_counter, Bitboard piece_positions, Bitboard empty_squares, Bitboard opponent_pieces);

    void generate_sliding_piece_moves(const BishopMap&, Move* move_list, int& move_counter, Bitboard piece_positions, Bitboard empty_squares, Bitboard opponent_pieces);
    
    void generate_sliding_piece_moves(const QueenMap&, Move* move_list, int& move_counter, Bitboard piece_positions, Bitboard empty_squares, Bitboard opponent_pieces);

    /**
     * \brief Get all of the squares attacked by pawns of a given COLOUR.
     * \tparam COLOUR - The colour of the pawns.
     * \param[in] pawn_positions - Bitboard of the pawn positions.
     * \return Bitboard of the squares attack by the pawns.
     */
    template<PieceColour COLOUR>
    Bitboard pawn_attacks(Bitboard pawn_positions)
    {
        Bitboard atk = (COLOUR == PieceColour::BLACK) ? bitboard::south_east_one(pawn_positions) : bitboard::north_east_one(pawn_positions);
        atk |= (COLOUR == PieceColour::BLACK) ? bitboard::south_west_one(pawn_positions) : bitboard::north_west_one(pawn_positions);
        return atk;
    }

    /**
     * \brief Get all of the squares attacked by a piece type.
     * \tparam TYPE - The piece type.
     * \param[in] piece_positions - Bitboard of the piece positions.
     * \return Bitboard of the squares attacked by the piece type.
     */
    template<PieceType TYPE>
    Bitboard piece_attacks(Bitboard piece_positions, Bitboard occupied_squares)
    {
        // TODO: The Knight and King maps should be statically constructed, which means that eventually they can be removed from here.
        attacks::KnightAttacks knight_map;
        attacks::KingAttacks king_map;
        Bitboard atk_bb{ 0 };
        while (piece_positions) {
            if (TYPE == PieceType::KNIGHT) atk_bb |= knight_map.get(bitboard::pop_LS1B(piece_positions));
            if (TYPE == PieceType::KING) atk_bb |= king_map.get(bitboard::pop_LS1B(piece_positions));
            if (TYPE == PieceType::BISHOP) atk_bb |= BishopMap::attacks(bitboard::pop_LS1B(piece_positions), occupied_squares);
            if (TYPE == PieceType::ROOK) atk_bb |= RookMap::attacks(bitboard::pop_LS1B(piece_positions), occupied_squares);
            if (TYPE == PieceType::QUEEN) atk_bb |= QueenMap::attacks(bitboard::pop_LS1B(piece_positions), occupied_squares);
        }
        return atk_bb;
    }

    /**
     * \brief Constructs a quiet Move.
     * \param[in] from - The square from which to move.
     * \param[in] to - The square to move to.
     * \return The constructed quiet move.
     */
    inline Move make_quiet(Square from, Square to) {
        Move move;
        move.from = from; move.to = to;
        return move;
    }

    /**
     * \brief Constructs a double pawn push.
     * \param[in] from - The square from which to move.
     * \param[in] to - The square to move to.
     * \return The constructed double push move.
     * 
     * \note Use this when constructing double push moves to ensure
     * that the en passant target is properly set after making the move.
     */
    inline Move make_double_push(Square from, Square to) {
        Move move;
        move.from = from; move.to = to; move.double_push = true;
        return move;
    }

    /**
     * \brief Constructs a move with capture.
     * \param[in] from - The square from which to move.
     * \param[in] to - The square to move to.
     * \return The constructed capture move.
     */
    inline Move make_capture(Square from, Square to) {
        Move move;
        move.from = from; move.to = to; move.capture = true;
        return move;
    };

    /**
     * \brief Constructs an en passant capture.
     * \param[in] from - The square from which to move.
     * \param[in] to - The square to move to.
     * \return The constructed en passant capture.
     */
    inline Move make_ep_capture(Square from, Square to) {
        Move move;
        move.from = from; move.to = to; move.capture = true; move.en_passant = true;
        return move;
    };

    /**
     * \brief Constructs a quiet move with promotion.
     * \param[in] from - The square from which to move.
     * \param[in] to - The square to move to.
     * \param[in] type - The type of piece to promote to.
     * \return The constructed quiet promotion.
     */
    inline Move make_quiet_promotion(Square from, Square to, PieceType type) {
        Move move;
        move.from = from; move.to = to; move.promotion = type;
        return move;
    };

    /**
     * \brief Constructs a capture with promotion.
     * \param[in] from - The square from which to move.
     * \param[in] to - The square to move to.
     * \param[in] type - The type of piece to promote to.
     * \return The constructed capture promotion.
     */
    inline Move make_capture_promotion(Square from, Square to, PieceType type) {
        Move move;
        move.from = from; move.to = to; move.capture = true; move.promotion = type;
        return move;
    };

    inline Move make_kingside_castle() {
        Move move;
        move.castle = PieceType::KING;
        return move;
    };

    inline Move make_queenside_castle() {
        Move move;
        move.castle = PieceType::QUEEN;
        return move;
    };

    inline Square pop_first(Bitboard& bb) {
        unsigned long idx;
        _BitScanForward64(&idx, bb);
        bb &= bb - 1;
        return idx;
    }

    int pop_1st_bit(Bitboard* bb);

}

namespace pieces {

    PieceColour other_colour(PieceColour colour);
    PieceType fen_char_to_piece_type(const char fen_char);
    PieceColour fen_char_to_piece_colour(const char fen_char);
    char piece_to_fen_char(const PieceType type, const PieceColour colour);
    char black_piece_type_to_fen_char(const PieceType type);
    char white_piece_type_to_fen_char(const PieceType type);

class PawnTargets {
    // PawnTargets store pawn move target bitboards and their source bitboards.
    private:
        Bitboard source;
        Bitboard target;

    public:
        PawnTargets(Bitboard sources, Bitboard targets) 
            : source(sources), target(targets) {};
        const Bitboard get_target() { return target; };
        const Bitboard get_source() { return source; };
};

class Pawns {
    private:
        std::vector<std::pair<Square, Square>> zip_moves(const Bitboard& sources, const Bitboard& targets);
    public:
        Pawns() {}
        virtual Bitboard push_targets(Bitboard sources) = 0;
        virtual Bitboard push_sources(Bitboard targets) = 0;
        virtual Bitboard east_attack_targets(Bitboard sources) = 0;
        virtual Bitboard east_attack_sources(Bitboard opponent_pieces) = 0;
        virtual Bitboard west_attack_targets(Bitboard sources) = 0;
        virtual Bitboard west_attack_sources(Bitboard opponent_pieces) = 0;
        virtual Bitboard double_push_target() = 0;
        std::vector<std::pair<Square, Square>> quiet_moves(Bitboard pawn_positions, Bitboard empty_squares);
        std::vector<std::pair<Square, Square>> captures(Bitboard pawn_positions, Bitboard opponent_pieces);
        std::vector<std::pair<Square, Square>> single_push(Bitboard pawn_positions, Bitboard empty_squares);
        std::vector<std::pair<Square, Square>> double_push(Bitboard pawn_positions, Bitboard empty_squares);
        std::vector<std::pair<Square, Square>> west_captures(Bitboard pawn_positions, Bitboard opponent_pieces);
        std::vector<std::pair<Square, Square>> east_captures(Bitboard pawn_positions, Bitboard opponent_pieces);
        Bitboard all_attacked_squares(Bitboard pawn_positions);
};

class WhitePawns : public Pawns {
    public:
        WhitePawns() {}
        Bitboard push_targets(Bitboard sources) override;
        Bitboard push_sources(Bitboard targets) override;
        Bitboard east_attack_targets(Bitboard sources) override;
        Bitboard east_attack_sources(Bitboard opponent_pieces) override;
        Bitboard west_attack_targets(Bitboard sources) override;
        Bitboard west_attack_sources(Bitboard opponent_pieces) override;
        Bitboard double_push_target() override { return bitboard::RANK_4; };
};

class BlackPawns : public Pawns {
    public:
        BlackPawns() {}
        Bitboard push_targets(Bitboard sources) override;
        Bitboard push_sources(Bitboard targets) override;
        Bitboard east_attack_targets(Bitboard sources) override;
        Bitboard east_attack_sources(Bitboard opponent_pieces) override;
        Bitboard west_attack_targets(Bitboard sources) override;
        Bitboard west_attack_sources(Bitboard opponent_pieces) override;
        Bitboard double_push_target() override { return bitboard::RANK_5; };
};

} // namespace name


#endif // YAK_PIECES_H_