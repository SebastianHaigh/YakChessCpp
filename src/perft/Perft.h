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

PerftResult perft(int depth);

class Board;
PerftResult perftHelper(Board& board, int depth);

} // namespace yak
