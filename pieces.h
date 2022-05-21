#ifndef YAK_PIECES_H_
#define YAK_PIECES_H_

#include "bitboard.h"
#include "attackmaps.h"
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

class Piece {
    public:
        virtual std::vector<std::pair<Square, Square>> quiet_moves(Bitboard piece_positions, Bitboard empty_squares) = 0;
        virtual std::vector<std::pair<Square, Square>> captures(Bitboard piece_positions, Bitboard occupied_squares, Bitboard opponent_pieces) = 0;
        virtual Bitboard attacks(Bitboard piece_positions, Bitboard empty_squares, Bitboard opponent_pieces) = 0;
};

class JumpingPiece : public Piece {
    private:
        attacks::AttackMap* attack_map;

    public:
        JumpingPiece(attacks::AttackMap* attack_map) : attack_map(attack_map) {}
        std::vector<std::pair<Square, Square>> quiet_moves(Bitboard piece_positions, Bitboard empty_squares) override;
        std::vector<std::pair<Square, Square>> captures(Bitboard piece_positions, Bitboard occupied_squares, Bitboard opponent_pieces) override;
        Bitboard attacks(Bitboard piece_positions, Bitboard empty_squares, Bitboard opponent_pieces) override;
};

class SlidingPiece : public Piece {
    private:
        std::vector<attacks::Ray*> rays;
    public:
        SlidingPiece(std::vector<attacks::Ray*> rays) : rays(rays) {}
        std::vector<std::pair<Square, Square>> quiet_moves(Bitboard piece_positions, Bitboard empty_squares) override;
        std::vector<std::pair<Square, Square>> captures(Bitboard piece_positions, Bitboard occupied_squares, Bitboard opponent_pieces) override;
        Bitboard attacks(Bitboard piece_positions, Bitboard empty_squares, Bitboard opponent_pieces) override;
};

class NullPiece : public Piece {
    public:
        NullPiece() {}
        std::vector<std::pair<Square, Square>> quiet_moves(Bitboard piece_positions, Bitboard empty_squares) override { return std::vector<std::pair<Square, Square>>(); };
        std::vector<std::pair<Square, Square>> captures(Bitboard piece_positions, Bitboard occupied_squares, Bitboard opponent_pieces) override { return std::vector<std::pair<Square, Square>>(); };
        Bitboard attacks(Bitboard piece_positions, Bitboard empty_squares, Bitboard opponent_pieces) override { return Bitboard{ 0 }; }
};

class StandardPieceFactory {
    private: 
        attacks::NorthRay north_ray;
        attacks::NorthEastRay north_east_ray;
        attacks::EastRay east_ray;
        attacks::SouthEastRay south_east_ray;
        attacks::SouthRay south_ray;
        attacks::SouthWestRay south_west_ray;
        attacks::WestRay west_ray;
        attacks::NorthWestRay north_west_ray;
        attacks::KnightAttacks knight_attacks;
        attacks::KingAttacks king_attacks;
    public:
        StandardPieceFactory() {}
        std::unique_ptr<Pawns> make_pawns(PieceColour colour);
        std::unique_ptr<Piece> make_piece(PieceType type);
};

} // namespace name

#endif // YAK_PIECES_H_