#ifndef YAK_PIECES_H_
#define YAK_PIECES_H_

#include "bitboard.h"
#include "attackmaps.h"
#include <vector>
#include <memory>
#include <stdint.h>
#include <stdio.h>

namespace pieces {

enum Colour {
    WHITE, BLACK
};

enum Type {
    PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING
};

class Move {
    private:
        Bitboard sources;
    
    protected:
        Bitboard targets;

    public:
        Move(Bitboard source_squares, Bitboard target_squares): sources(source_squares), targets(target_squares) {};
        Bitboard get_source();
        Bitboard get_target();
};

class SerialMove: public Move {
    private:
        Square source;

    public:
        SerialMove(Square source_square, Bitboard target_squares);
        const Square get_source();
};

class Pieces {
    protected:
        std::shared_ptr<Bitboard> board;
        std::shared_ptr<Bitboard> empty_squares;
        Colour colour;
    public:
        Pieces(std::shared_ptr<Bitboard> board_ptr, 
              std::shared_ptr<Bitboard> empty_squares_ptr);
        virtual std::stack<Move> get_all_moves();
        virtual std::stack<Move> get_quiet_moves();
        virtual std::stack<Move> get_captures(Bitboard opponent_piece);
        virtual Bitboard get_all_attacks();
};

class Pawns: protected Pieces {

    protected:
        Move single_push();
        Move double_push();
        Bitboard all_attack();
        Move east_captures(Bitboard opponent_piece);
        Move west_captures(Bitboard opponent_piece);
        virtual Bitboard targets_from_sources(Bitboard sources);
        virtual Bitboard sources_from_targets(Bitboard sources);
        virtual Bitboard east_attack();
        virtual Bitboard west_attack();
        virtual Bitboard double_push_target();
    public:
        Pawns(std::shared_ptr<Bitboard> board_ptr,
              std::shared_ptr<Bitboard> empty_squares_ptr);
        std::stack<Move> get_all_moves() override;
        std::stack<Move> get_quiet_moves() override;
        std::stack<Move> get_captures(Bitboard opponent_piece) override;
        Bitboard get_all_attacks() override;
};

class WhitePawns: public Pawns {
    protected:
        Bitboard targets_from_sources(Bitboard sources) override;
        Bitboard sources_from_targets(Bitboard sources) override;
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
        Bitboard sources_from_targets(Bitboard sources) override;
        Bitboard east_attack() override;
        Bitboard west_attack() override;
        Bitboard double_push_target() override;
    public:
        BlackPawns(std::shared_ptr<Bitboard> board_ptr,
                   std::shared_ptr<Bitboard> empty_squares_ptr);
};

class SlidingPieces {
    protected:
        std::shared_ptr <Bitboard> board;
        std::shared_ptr <Bitboard> empty_squares;
        std::vector <std::unique_ptr <attacks::Ray>> rays;
    public:
        SlidingPieces(std::shared_ptr <Bitboard> board_ptr, 
                      std::shared_ptr <Bitboard> empty_squares_ptr);
        std::stack <SerialMove> quite_moves();
        std::stack <SerialMove> targets();
};

class Rooks: public SlidingPieces {
    public:
        Rooks(std::shared_ptr <Bitboard> board_ptr, 
              std::shared_ptr <Bitboard> empty_squares_ptr);
};

class Bishops: public SlidingPieces {
    public:
        Bishops(std::shared_ptr <Bitboard> board_ptr, 
                std::shared_ptr <Bitboard> empty_squares_ptr);
};

class Queens: public SlidingPieces {
    public:
        Queens(std::shared_ptr <Bitboard> board_ptr, 
               std::shared_ptr <Bitboard> empty_squares_ptr);
};

} // namespace name

#endif // YAK_PIECES_H_