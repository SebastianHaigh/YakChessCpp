#ifndef YAK_MOVE_GEN_H_
#define YAK_MOVE_GEN_H_

#include "bitboard.h"
#include "pieces.h"
#include "board.h"
#include <vector>
#include <memory>
#include <utility>

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
    //  effect on the underlying board representation.
    /*
    class Move {
        public:
            virtual ~Move() = default;
            virtual void execute(Board& board) = 0;
            virtual void undo(Board& board) = 0;
    };

    class PseudoLegalMove : public Move {
        private:
            PieceType type_of_piece;
            PieceColour colour_of_piece;
            Square source;
            Square target;
            bool move_made = 0;

        public:
            PseudoLegalMove(PieceType type, PieceColour colour, Square source, Square target)
                : type_of_piece(type), colour_of_piece(colour), source(source), target(target){};
            void execute(Board& board) override;
            void undo(Board & board) override;
    };

    class PseudoLegalCapture : public Move {
        private:
            PieceType piece_to_move;
            PieceType piece_to_capture;
            Square source;
            Square target;
            
            bool move_made = 0;

        public:
            PseudoLegalCapture(PieceType piece_to_move, Square source, Square target, PieceType piece_to_capture)
                : piece_to_move(piece_to_move), source(source), target(target), piece_to_capture(piece_to_capture){};
            void execute(Board& board) override;
            void undo(Board& board) override;
    };

    class DoublePushMove : public Move {
        private:
            PieceType type_of_piece;
            PieceColour colour_of_piece;
            Square source;
            Square target;

            bool move_made = 0;

        public:
            DoublePushMove(PieceColour colour_of_piece, Square source, Square target)
                : type_of_piece(PieceType::PAWN), colour_of_piece(colour_of_piece), source(source), target(target) {};
            void execute(Board& board) override;
            void undo(Board& board) override;
    };

    class EpCapture : public Move {
        private:
            PieceColour colour_of_piece;
            Square source;
            Square target;

            bool move_made = 0;
        public:
            EpCapture(PieceColour colour_of_piece, Square source, Square target)
                : colour_of_piece(colour_of_piece), source(source), target(target) {};
            void execute(Board& board) override;
            void undo(Board& board) override;
    };
    */
    /**
    * Class for generating lists of all possible moves on the board
    */
    class MoveGenerator {
        private:
            bool side_to_move;
            std::shared_ptr<Board> board;

            std::vector<Move> process_quiet_moves(Board& board, pieces::PawnTargets targets, std::vector<Move> move_stack);
            std::vector<Move> process_captures(Board& board, pieces::PawnTargets targets, std::vector<Move> move_stack);
            std::vector<Move> piece_moves_and_captures(Board& board, PieceType type, PieceColour friendly, PieceColour enemy, std::vector<Move> move_stack);
            std::vector<Move> process_ep_captures(Board& board, pieces::PawnTargets targets, std::vector<Move> move_stack, PieceColour move_colour);
            std::vector<Move> pseudo_legal_moves(Board& board, PieceColour friendly, PieceColour enemy);
            std::vector<std::pair<Square, Square>> zip(std::vector<Square> sources, std::vector<Square> targets);
            pieces::StandardPieceFactory factory;
            bool target_is_promotion(Square square) { return (square < 8 || square > 55) ? 1 : 0;  }

        public:
            MoveGenerator(std::shared_ptr<Board> board)
                : side_to_move(1), board(board) {};
            std::vector<Move> generate_pseudo_legal_moves(Board& board);
    };

} // namespace move_generation

#endif // YAK_MOVE_GEN_H_