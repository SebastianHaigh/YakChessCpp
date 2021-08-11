#include "movegen.h"

namespace move_generation {

void PseudoLegalMove::execute() {
    piece_to_move->make_move(source, target);
    move_made = 1;
}

void PseudoLegalMove::undo() {
    if (move_made) {
        piece_to_move->make_move(source, target);
        move_made = 0;
    }
}
PseudoLegalMoves MoveGenerator::generate_pseudo_legal_moves() {
    if (white_to_move()) {
        return pseudo_legal_moves(*white_pieces, *black_pieces);
    } else {
        return pseudo_legal_moves(*black_pieces, *white_pieces);
    }
}

PseudoLegalMoves MoveGenerator::pseudo_legal_moves(pieces::ChessMen friendly_pieces, pieces::ChessMen opponent_pieces) {
    auto pawns = friendly_pieces.get_pawns();

    // Pushes should be good as is. They just need to be serialised and place in 
    // PseudoLegalMove objects.

    auto single_pushes = pawns.single_pushes();
    auto double_pushes = pawns.double_pushes();

    // For the captures we need to determine which of the enemy pieces is being 
    // captured. A way to do this could be add a method to chessmen that return 
    // a pointer to the piece that is on a given sqaure
    auto west_captures = pawns.west_captures();
    auto east_captures = pawns.east_captures();

    return PseudoLegalMoves();
}


bool MoveGenerator::white_to_move() {
    return side_to_move();
}


} // namespace move_generation