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

    PieceType fen_char_to_piece_type(const char fen_char);
    PieceColour fen_char_to_piece_colour(const char fen_char);
    

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
        virtual Bitboard pawn_standard_starting_position() = 0;
        virtual Bitboard pawn_push_targets(Bitboard sources) = 0;
        virtual Bitboard pawn_push_sources(Bitboard targets) = 0;
        virtual Bitboard pawn_east_attack_targets(Bitboard sources) = 0;
        virtual Bitboard pawn_east_attack_sources() = 0;
        virtual Bitboard pawn_west_attack_targets(Bitboard sources) = 0;
        virtual Bitboard pawn_west_attack_sources() = 0;
        virtual Bitboard pawn_double_push_target() = 0;
        virtual void make_move(Bitboard source_and_target_bitboard) = 0;
};

class Black : public Colour {
    private:
        std::shared_ptr<Bitboard> this_colour_pieces;
        std::shared_ptr<Bitboard> opponent_pieces;
    public:
        Black(std::shared_ptr<Bitboard> this_colour_pieces, 
            std::shared_ptr<Bitboard> opponent_pieces)
            : this_colour_pieces(this_colour_pieces), opponent_pieces(opponent_pieces) {};
        Bitboard pawn_standard_starting_position() override;
        Bitboard pawn_push_targets(Bitboard sources) override;
        Bitboard pawn_push_sources(Bitboard targets) override;
        Bitboard pawn_east_attack_targets(Bitboard sources) override;
        Bitboard pawn_east_attack_sources() override;
        Bitboard pawn_west_attack_targets(Bitboard sources) override;
        Bitboard pawn_west_attack_sources() override;
        Bitboard pawn_double_push_target() override;
        void make_move(Bitboard source_and_target_bitboard) override;
};

class White : public Colour {
    private:
        std::shared_ptr<Bitboard> this_colour_pieces;
        std::shared_ptr<Bitboard> opponent_pieces;
    public:
        White(std::shared_ptr<Bitboard> this_colour_pieces, 
            std::shared_ptr<Bitboard> opponent_pieces)
            : this_colour_pieces(this_colour_pieces), opponent_pieces(opponent_pieces) {};
        Bitboard pawn_standard_starting_position() override;
        Bitboard pawn_push_targets(Bitboard sources) override;
        Bitboard pawn_push_sources(Bitboard targets) override;
        Bitboard pawn_east_attack_targets(Bitboard sources) override;
        Bitboard pawn_east_attack_sources() override;
        Bitboard pawn_west_attack_targets(Bitboard sources) override;
        Bitboard pawn_west_attack_sources() override;
        Bitboard pawn_double_push_target() override;
        void make_move(Bitboard source_and_target_bitboard) override;
};

class Piece {
    public:
        virtual ~Piece() = default;
        virtual Bitboard current_position() = 0;
        virtual void make_move(Square source, Square target) = 0;
        virtual void make_capture(Square target) = 0;
        virtual bool has_piece_on_square(Square square_index) = 0;
        virtual bool is_null() = 0;
};

class NullPiece : public Piece {
    public:
        NullPiece() = default;
        ~NullPiece() = default;
        Bitboard current_position() override { return Bitboard(0); };
        void make_move(Square source, Square target) override {};
        void make_capture(Square target) override {};
        bool has_piece_on_square(Square square_index) override { return 0; };
        bool is_null() override { return 1; };
};

class Pawns : public Piece {

    private:
        Bitboard board;
        std::shared_ptr<Bitboard> empty_squares;
        std::shared_ptr<Colour> colour;
        Bitboard single_push();
        Bitboard double_push();
        Bitboard all_attack();
    public:
        Pawns(std::shared_ptr<Bitboard> empty_squares,
              std::shared_ptr<Colour> colour) 
              : board(Bitboard(0)), empty_squares(empty_squares) , colour(colour) {};
        Pawns(Bitboard board, std::shared_ptr<Bitboard> empty_squares,
              std::shared_ptr<Colour> colour) 
              : board(board), empty_squares(empty_squares) , colour(colour) {};
        ~Pawns() = default;
        PawnTargets single_pushes();
        PawnTargets double_pushes();
        PawnTargets west_captures();
        PawnTargets east_captures();
        void standard_starting_position();
        Bitboard current_position() override;
        void make_move(Square source, Square target) override;
        void make_capture(Square target) override;
        bool has_piece_on_square(Square square_index) override;
        bool is_null() override { return 0; };
        Bitboard all_attacked_squares();
};


class ChessMen {
    private:
        std::shared_ptr<Pawns> pawns;

    public:
        ChessMen(std::shared_ptr<Pawns> pawns) : pawns(pawns) {};
        std::shared_ptr<Pawns> get_pawns() { return pawns; };
        std::shared_ptr<Piece> get_piece(Square square_index);
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