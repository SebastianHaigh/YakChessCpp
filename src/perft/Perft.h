#pragma once

#include <cstddef>

namespace yak {

struct PerftResult {
  size_t m_total{0};
  size_t m_captures{0};
  size_t m_ep{0};
  size_t m_castles{0};
  size_t m_promotions{0};
  size_t m_checks{0};
  size_t m_checkmates{0};
};

class Board;
PerftResult perft(Board& board, int depth);
PerftResult perftHelper(Board& board, int depth);

} // namespace yak
