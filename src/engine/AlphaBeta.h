#pragma once

namespace yak { class Board; }

namespace yak::engine {

int evaluate(Board& board);

int alphaBeta(Board& board, int alpha, int beta, int depth, bool maximise);

} // namespace yak::engine
