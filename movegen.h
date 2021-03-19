#ifndef YAK_MOVE_GEN_H_
#define YAK_MOVE_GEN_H_

#include "bitboard.h"
#include "pieces.h"
#include <vector>
#include <memory>

namespace move_generation {

class MoveGenerator {
    private:
        std::vector<std::unique_ptr<pieces::Pieces>> white_pieces;
        std::vector<std::unique_ptr<pieces::Pieces>> black_pieces;
    public:
        MoveGenerator();
};

} // namespace move_generation

#endif // YAK_MOVE_GEN_H_