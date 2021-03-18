#ifndef YAK_PIECES_H_
#define YAK_PIECES_H_

#include "bitboard.h"
#include "attackmaps.h"
#include <vector>
#include <memory>
#include <stdint.h>
#include <stdio.h>

namespace pieces {

class Move {
    private:
        Bitboard sources;
    
    protected:
        Bitboard targets;

    public:
        Move(Bitboard source_squares, Bitboard target_squares);
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
        virtual Bitboard pawn_targets_from_sources(Bitboard sources) = 0;
        virtual Bitboard pawn_sources_from_targets(Bitboard targets) = 0;
        virtual Bitboard pawn_east_attacks(Bitboard sources) = 0;
        virtual Bitboard pawn_west_attacks(Bitboard sources) = 0;
};

class BlackPieces: virtual protected Pieces {
    protected:
        Bitboard double_push_targets = bitboard::RANK_5;
        Bitboard pawn_targets_from_sources(Bitboard sources) override;
        Bitboard pawn_sources_from_targets(Bitboard targets) override;
        Bitboard pawn_east_attacks(Bitboard sources) override;
        Bitboard pawn_west_attacks(Bitboard sources) override;
};

class WhitePieces: virtual protected Pieces {
    protected:
        Bitboard double_push_targets = bitboard::RANK_4;
        Bitboard pawn_targets_from_sources(Bitboard sources) override;
        Bitboard pawn_sources_from_targets(Bitboard targets) override;
        Bitboard pawn_east_attacks(Bitboard sources) override;
        Bitboard pawn_west_attacks(Bitboard sources) override;
};

class Pawns {
    public:
        virtual Move single_push() = 0;
        virtual Move double_push() = 0;
        virtual Bitboard east_attack() = 0;
        virtual Bitboard west_attack() = 0;
        virtual Bitboard all_attack() = 0;
        virtual Move east_captures(Bitboard opponent_piece) = 0;
        virtual Move west_captures(Bitboard opponent_piece) = 0;
};

class BlackPawns: virtual public Pawns, protected BlackPieces {
    private:
        std::shared_ptr <Bitboard> board;
        std::shared_ptr <Bitboard> empty_squares;
    public:
        BlackPawns(std::shared_ptr <Bitboard> board_ptr, 
                   std::shared_ptr <Bitboard> empty_squares_ptr);
        Move single_push() override;
        Move double_push() override;
        Bitboard east_attack() override;
        Bitboard west_attack() override;
        Bitboard all_attack() override;
        Move east_captures(Bitboard opponent_piece) override;
        Move west_captures(Bitboard opponent_piece) override;
};

class WhitePawns: virtual public Pawns {
    private:
        std::shared_ptr <Bitboard> board;
        std::shared_ptr <Bitboard> empty_squares;
    public:
        WhitePawns(std::shared_ptr <Bitboard> board_ptr, 
                   std::shared_ptr <Bitboard> empty_squares_ptr);
        Move single_push() override;
        Move double_push() override;
        Bitboard east_attack() override;
        Bitboard west_attack() override;
        Bitboard all_attack() override;
        Move east_captures(Bitboard opponent_piece) override;
        Move west_captures(Bitboard opponent_piece) override;
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