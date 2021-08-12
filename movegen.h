#ifndef YAK_MOVE_GEN_H_
#define YAK_MOVE_GEN_H_

#include "bitboard.h"
#include "pieces.h"
#include <stack>
#include <memory>

namespace move_generation
{
    // Abstract class Move, and its derived classes, use the command pattern for 
    // making moves and undoing moves that have been made. There are three 
    // possible move types: quiet moves, captures, and promotions (promotions 
    // are yet to be implemented).
    //
    // Example - Quiet pawn move:
    //
    //  With a std:shared_ptr<pieces::Pawns> called `white_pawns` containing 
    //  white pawns, a move is created that moves a pawn from B2 (square 9) to 
    //  B4 (square 25):
    //
    //      PseudoLegalMove pawn_move = PseudoLegalMove(white_pawns, 9, 25);
    //
    //  The move can be made by calling the `execute()` method:
    //
    //      pawn_move.execute();
    //
    //  If the move has already been made, it can be undone by calling the 
    //  `undo()` method:
    //
    //      pawn_move.undo();
    //
    //  Note that if `undo()` is called before `execute()`, `undo()` will have 
    //  no effect. 
    //  
    //  Also note that there must be a pawn on square B2 and square B4 must be 
    //  empty for the `execute()` function to have the expected and desired 
    //  effect on the underlying board representation. The pieces::Pawns class 
    //  method that will allow you to ensure that the move is at least pseudo 
    //  legal.
    class Move {
        public:
            virtual ~Move() = default;
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
                : piece_to_move(piece_to_move), source(source), target(target){};
            ~PseudoLegalMove() = default;
            void execute() override;
            void undo() override;
    };

    class PseudoLegalCapture : public Move {
        private:
            std::shared_ptr<pieces::Piece> piece_to_move;
            Square source;
            Square target;
            std::shared_ptr<pieces::Piece> piece_to_capture;
            bool move_made = 0;

        public:
            PseudoLegalCapture(std::shared_ptr<pieces::Piece> piece_to_move,
                Square source, Square target,
                std::shared_ptr<pieces::Piece> piece_to_capture)
                : piece_to_move(piece_to_move), source(source), target(target), piece_to_capture(piece_to_capture){};
            ~PseudoLegalCapture() = default;
            void execute() override;
            void undo() override;
    };

    class MoveGenerator {
        private:
            bool side_to_move;
            pieces::ChessMen white_pieces;
            pieces::ChessMen black_pieces;
            std::stack<std::shared_ptr<Move>> process_quiet_moves(std::shared_ptr<pieces::Piece> piece_to_move, 
                pieces::PawnTargets targets, std::stack<std::shared_ptr<Move>> move_stack);
            std::stack<std::shared_ptr<Move>> process_captures(std::shared_ptr<pieces::Piece> piece_to_move, 
                pieces::PawnTargets targets, std::stack<std::shared_ptr<Move>> move_stack, 
                pieces::ChessMen opponent_pieces);
            std::stack<std::shared_ptr<Move>> pseudo_legal_moves(pieces::ChessMen friendly_pieces, 
                pieces::ChessMen opponent_pieces);

        public:
            MoveGenerator(pieces::ChessMen white_pieces,
                pieces::ChessMen black_pieces)
                : side_to_move(1), white_pieces(white_pieces), black_pieces(black_pieces){};
            std::stack<std::shared_ptr<Move>> generate_pseudo_legal_moves();
    };

} // namespace move_generation

#endif // YAK_MOVE_GEN_H_