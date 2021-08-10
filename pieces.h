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

enum Colour {
    WHITE, BLACK
};

enum Type {
    PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING
};

class SourceTargetPair {
    private:
        Square source;
        Square target;

    public:
        SourceTargetPair(Square source_square, Square target_square) : source(source_square), target(target_square) {};
};

class Pieces {
    protected:
        std::shared_ptr<Bitboard> board;
        std::shared_ptr<Bitboard> empty_squares;
    public:
        Pieces(std::shared_ptr<Bitboard> board_ptr, 
              std::shared_ptr<Bitboard> empty_squares_ptr);
        virtual std::stack<SourceTargetPair> get_all_moves();
        // virtual std::stack<Move> get_quiet_moves();
        // virtual std::stack<Move> get_captures(Bitboard opponent_piece);
        virtual Bitboard get_all_attacks();
        void make_move(Square source, Square target);
};

class Pawns: protected Pieces {

    protected:
        Bitboard single_push();
        Bitboard double_push();
        Bitboard all_attack();
        // Move east_captures(Bitboard opponent_piece);
        // Move west_captures(Bitboard opponent_piece);
        virtual Bitboard targets_from_sources(Bitboard sources);
        virtual Bitboard sources_from_targets(Bitboard targets);
        virtual Bitboard east_attack();
        virtual Bitboard west_attack();
        virtual Bitboard double_push_target();
    public:
        Pawns(std::shared_ptr<Bitboard> board_ptr,
              std::shared_ptr<Bitboard> empty_squares_ptr);
        std::stack<SourceTargetPair> get_all_moves() override;
        // std::stack<Move> get_quiet_moves() override;
        // std::stack<Move> get_captures(Bitboard opponent_piece) override;
        Bitboard get_all_attacks() override;
};

class WhitePawns: public Pawns {
    protected:
        Bitboard targets_from_sources(Bitboard sources) override;
        Bitboard sources_from_targets(Bitboard targets) override;
        Bitboard east_attack() override;
        Bitboard west_attack() override;
        Bitboard double_push_target() override;
    public:
        WhitePawns(std::shared_ptr<Bitboard> board_ptr,
                   std::shared_ptr<Bitboard> empty_squares_ptr);
};

class BlackPawns: public Pawns {
    protected:
        Bitboard targets_from_sources(Bitboard sources) override;
        Bitboard sources_from_targets(Bitboard targets) override;
        Bitboard east_attack() override;
        Bitboard west_attack() override;
        Bitboard double_push_target() override;
    public:
        BlackPawns(std::shared_ptr<Bitboard> board_ptr,
                   std::shared_ptr<Bitboard> empty_squares_ptr);
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

class Move {
    private:
        Square source;
        Square target;
        Pieces* piece_to_move;

    public:
        Move(Pieces* piece, Square source_square, Square target_square) 
        : source(source_square), target(target_square), piece_to_move(piece) {};
        void execute();
};

} // namespace name

#endif // YAK_PIECES_H_