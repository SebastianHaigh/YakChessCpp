#ifndef GENERATION_PIECES_H_
#define GENERATION_PIECES_H_

#include "bitboard.h"
#include "move.h"
#include "pieces.h"
#include "pawns.h"
#include "types.h"

namespace yak::move {

template<PieceColour Colour>
void generateEpCaptures(Move *moveList,
                        int &moveCounter,
                        Bitboard pawnPositions,
                        Bitboard epTarget)
{
  Bitboard sources = pawns::pawnWestAttackSource<Colour>(epTarget) & pawnPositions;
  Bitboard targets = pawns::pawnWestAttackTarget<Colour>(sources);
  while (sources)
  {
    *moveList++ = makeEpCapture(bitboard::popLS1B(sources), bitboard::popLS1B(targets));
    moveCounter++;
  }

  sources = pawns::pawnEastAttackSource<Colour>(epTarget) & pawnPositions;
  targets = pawns::pawnEastAttackTarget<Colour>(sources);
  while (sources)
  {
    *moveList++ = makeEpCapture(bitboard::popLS1B(sources), bitboard::popLS1B(targets));
    moveCounter++;
  }
};

} // namespace yak::move

#endif // GENERATION_PIECES_H_
