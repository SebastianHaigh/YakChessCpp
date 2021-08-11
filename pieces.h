#ifndef YAK_PIECES_H_
#define YAK_PIECES_H_

#include "bitboard.h"
#include "attackmaps.h"
#include <vector>
#include <memory>
#include <stack>
#include <stdint.h>
#include <stdio.h>

namespace pieces {

class SourceTargetPair {
    private:
        Square source;
        Square target;

    public:
        SourceTargetPair(Square source_square, Square target_square) : source(source_square), target(target_square) {};
        const Square get_target() { return target; };
        const Square get_source() { return source; };
};

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

class Colour {
    public:
        virtual Bitboard pawn_push_targets(Bitboard sources) = 0;
        virtual Bitboard pawn_push_sources(Bitboard targets) = 0;
        virtual Bitboard pawn_east_attack_targets(Bitboard sources) = 0;
        virtual Bitboard pawn_east_attack_sources() = 0;
        virtual Bitboard pawn_west_attack_targets(Bitboard sources) = 0;
        virtual Bitboard pawn_west_attack_sources() = 0;
        virtual Bitboard pawn_double_push_target() = 0;
};

class Black : public Colour {
    private:
        std::shared_ptr<Bitboard> this_colour_pieces;
        std::shared_ptr<Bitboard> opponent_pieces;
    public:
        Black(std::shared_ptr<Bitboard> this_colour_pieces, 
            std::shared_ptr<Bitboard> opponent_pieces)
            : this_colour_pieces(this_colour_pieces), opponent_pieces(opponent_pieces) {};
        Bitboard pawn_push_targets(Bitboard sources) override;
        Bitboard pawn_push_sources(Bitboard targets) override;
        Bitboard pawn_east_attack_targets(Bitboard sources) override;
        Bitboard pawn_east_attack_sources() override;
        Bitboard pawn_west_attack_targets(Bitboard sources) override;
        Bitboard pawn_west_attack_sources() override;
        Bitboard pawn_double_push_target() override;
};

class White : public Colour {
    private:
        std::shared_ptr<Bitboard> this_colour_pieces;
        std::shared_ptr<Bitboard> opponent_pieces;
    public:
        White(std::shared_ptr<Bitboard> this_colour_pieces, 
            std::shared_ptr<Bitboard> opponent_pieces)
            : this_colour_pieces(this_colour_pieces), opponent_pieces(opponent_pieces) {};
        Bitboard pawn_push_targets(Bitboard sources) override;
        Bitboard pawn_push_sources(Bitboard targets) override;
        Bitboard pawn_east_attack_targets(Bitboard sources) override;
        Bitboard pawn_east_attack_sources() override;
        Bitboard pawn_west_attack_targets(Bitboard sources) override;
        Bitboard pawn_west_attack_sources() override;
        Bitboard pawn_double_push_target() override;
};


class Pawns {

    private:
        std::shared_ptr<Bitboard> board;
        std::shared_ptr<Bitboard> empty_squares;
        std::shared_ptr<Colour> colour;
        Bitboard single_push();
        Bitboard double_push();
        Bitboard all_attack();
    public:
        Pawns(std::shared_ptr<Bitboard> board,
              std::shared_ptr<Bitboard> empty_squares,
              std::shared_ptr<Colour> colour) 
              : board(board), empty_squares(empty_squares) , colour(colour) {};
        PawnTargets single_pushes();
        PawnTargets double_pushes();
        PawnTargets west_captures();
        PawnTargets east_captures();
        Bitboard all_attacked_squares();
};

// class SlidingPieces {
//     protected:
//         std::shared_ptr <Bitboard> board;
//         std::shared_ptr <Bitboard> empty_squares;
//         std::vector <std::unique_ptr <attacks::Ray>> rays;
//     public:
//         SlidingPieces(std::shared_ptr <Bitboard> board_ptr, 
//                       std::shared_ptr <Bitboard> empty_squares_ptr);
//         std::stack <Move> quite_moves();
//         std::stack <Move> targets();
// };

// class Rooks: public SlidingPieces {
//     public:
//         Rooks(std::shared_ptr <Bitboard> board_ptr, 
//               std::shared_ptr <Bitboard> empty_squares_ptr);
// };

// class Bishops: public SlidingPieces {
//     public:
//         Bishops(std::shared_ptr <Bitboard> board_ptr, 
//                 std::shared_ptr <Bitboard> empty_squares_ptr);
// };

// class Queens: public SlidingPieces {
//     public:
//         Queens(std::shared_ptr <Bitboard> board_ptr, 
//                std::shared_ptr <Bitboard> empty_squares_ptr);
// };

// class Move {
//     private:
//         Square source;
//         Square target;
//         Pieces* piece_to_move;

//     public:
//         Move(Pieces* piece, Square source_square, Square target_square) 
//         : source(source_square), target(target_square), piece_to_move(piece) {};
//         void execute();
// };

} // namespace name

#endif // YAK_PIECES_H_