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
        uint64_t sources;
    
    protected:
        uint64_t targets;

    public:
        Move(uint64_t source_squares, uint64_t target_squares);
        uint64_t get_source();
        uint64_t get_target();
};

class SerialMove: public Move {
    private:
        int source;

    public:
        SerialMove(int source_square, uint64_t target_squares);
        const int get_source();
};

class Pieces {
    protected:
        virtual uint64_t pawn_targets_from_sources(uint64_t sources) = 0;
        virtual uint64_t pawn_sources_from_targets(uint64_t targets) = 0;
        virtual uint64_t pawn_east_attacks(uint64_t sources) = 0;
        virtual uint64_t pawn_west_attacks(uint64_t sources) = 0;
};

class BlackPieces: virtual protected Pieces {
    protected:
        uint64_t double_push_targets = bitboard::RANK_5;
        uint64_t pawn_targets_from_sources(uint64_t sources) override;
        uint64_t pawn_sources_from_targets(uint64_t targets) override;
        uint64_t pawn_east_attacks(uint64_t sources) override;
        uint64_t pawn_west_attacks(uint64_t sources) override;
};

class WhitePieces: virtual protected Pieces {
    protected:
        uint64_t double_push_targets = bitboard::RANK_4;
        uint64_t pawn_targets_from_sources(uint64_t sources) override;
        uint64_t pawn_sources_from_targets(uint64_t targets) override;
        uint64_t pawn_east_attacks(uint64_t sources) override;
        uint64_t pawn_west_attacks(uint64_t sources) override;
};

class Pawns {
    public:
        virtual Move single_push() = 0;
        virtual Move double_push() = 0;
        virtual uint64_t east_attack() = 0;
        virtual uint64_t west_attack() = 0;
        virtual uint64_t all_attack() = 0;
        virtual Move east_captures(uint64_t opponent_piece) = 0;
        virtual Move west_captures(uint64_t opponent_piece) = 0;
};

class BlackPawns: virtual public Pawns, protected BlackPieces {
    private:
        std::shared_ptr <uint64_t> board;
        std::shared_ptr <uint64_t> empty_squares;
    public:
        BlackPawns(std::shared_ptr <uint64_t> board_ptr, 
                   std::shared_ptr <uint64_t> empty_squares_ptr);
        Move single_push() override;
        Move double_push() override;
        uint64_t east_attack() override;
        uint64_t west_attack() override;
        uint64_t all_attack() override;
        Move east_captures(uint64_t opponent_piece) override;
        Move west_captures(uint64_t opponent_piece) override;
};

class WhitePawns: virtual public Pawns {
    private:
        std::shared_ptr <uint64_t> board;
        std::shared_ptr <uint64_t> empty_squares;
    public:
        WhitePawns(std::shared_ptr <uint64_t> board_ptr, 
                   std::shared_ptr <uint64_t> empty_squares_ptr);
        Move single_push() override;
        Move double_push() override;
        uint64_t east_attack() override;
        uint64_t west_attack() override;
        uint64_t all_attack() override;
        Move east_captures(uint64_t opponent_piece) override;
        Move west_captures(uint64_t opponent_piece) override;
};

class SlidingPieces {
    protected:
        std::shared_ptr <uint64_t> board;
        std::shared_ptr <uint64_t> empty_squares;
        std::vector <std::unique_ptr <attacks::Ray>> rays;
    public:
        SlidingPieces(std::shared_ptr <uint64_t> board_ptr, 
                      std::shared_ptr <uint64_t> empty_squares_ptr);
        std::stack <SerialMove> quite_moves();
        std::stack <SerialMove> targets();
};

class Rooks: public SlidingPieces {
    public:
        Rooks(std::shared_ptr <uint64_t> board_ptr, 
              std::shared_ptr <uint64_t> empty_squares_ptr);
};

class Bishops: public SlidingPieces {
    public:
        Bishops(std::shared_ptr <uint64_t> board_ptr, 
                std::shared_ptr <uint64_t> empty_squares_ptr);
};

class Queens: public SlidingPieces {
    public:
        Queens(std::shared_ptr <uint64_t> board_ptr, 
               std::shared_ptr <uint64_t> empty_squares_ptr);
};

} // namespace name

#endif // YAK_PIECES_H_