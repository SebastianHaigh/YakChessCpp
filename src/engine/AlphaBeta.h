#pragma once

#include "types.h"
namespace yak { class Board; }

namespace yak::engine {

int evaluate(Board& board);

std::pair<int, Move> alphaBeta(Board& board, int depth, PieceColour us);
std::pair<int, Move> alphaBeta(Board& board, int alpha, int beta, int depth, bool maximise);

} // namespace yak::engine
