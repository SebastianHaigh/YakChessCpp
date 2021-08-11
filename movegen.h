#ifndef YAK_MOVE_GEN_H_
#define YAK_MOVE_GEN_H_

#include "bitboard.h"
#include "pieces.h"
#include <vector>
#include <memory>

namespace move_generation {

class MoveGenerator {
    private:
        bool side_to_move = 1;
        pieces::ChessMen white_pieces;
        pieces::ChessMen black_pieces;
        PseudoLegalMoves pseudo_legal_moves(pieces::ChessMen friendly_pieces, pieces::ChessMen opponent_pieces);
        bool white_to_move();
    public:
        MoveGenerator(pieces::ChessMen white_pieces, 
            pieces::ChessMen black_pieces) 
            : white_pieces(white_pieces), black_pieces(black_pieces) {};
        PseudoLegalMoves generate_pseudo_legal_moves();
};

class PseudoLegalMoves {
    public:
        PseudoLegalMoves();
};


class Move {
    public:
        virtual ~Move();
        virtual void execute() = 0;
        virtual void undo() = 0;
};

class PseudoLegalMove : public Move {
    private:
        std::shared_ptr<pieces::Piece> piece_to_move;
        Square source;
        Square target;
        bool move_made = 0;
    public:
        PseudoLegalMove(std::shared_ptr<pieces::Piece> piece_to_move, 
            Square source, Square target) 
            : piece_to_move(piece_to_move), source(source), target(target) {};
        void execute() override;
        void undo() override;
};

} // namespace move_generation

#endif // YAK_MOVE_GEN_H_