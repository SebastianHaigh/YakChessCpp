#ifndef GENERATION_PIECES_H_
#define GENERATION_PIECES_H_

#include "bitboard.h"
#include "move.h"
#include "pieces.h"
#include "attackmaps.h"
#include "pawns.h"
#include "types.h"

namespace yak::move {

template<PieceColour Colour, bool Promotions>
constexpr void generatePawnSinglePushes(Move *moveList,
                                        int &moveCounter,
                                        Bitboard pawnPositions,
                                        Bitboard emptySquares)
{
  pawnPositions = Promotions ? pawns::promotablePawns<Colour>(pawnPositions) : pawns::nonPromotablePawns<Colour>(pawnPositions);
  Bitboard sources = pawns::pawnSinglePushSource<Colour>(emptySquares) & pawnPositions;
  Bitboard targets = pawns::pawnSinglePushTarget<Colour>(sources);

  while (sources && not Promotions)
  {
    *moveList++ = makeQuiet(bitboard::popLS1B(sources), bitboard::popLS1B(targets));
    moveCounter++;
  }

  while (sources && Promotions)
  {
    Square from = bitboard::popLS1B(sources);
    Square to = bitboard::popLS1B(targets);
    *moveList++ = makeQuietPromotion(from, to, PieceType::KNIGHT);
    *moveList++ = makeQuietPromotion(from, to, PieceType::BISHOP);
    *moveList++ = makeQuietPromotion(from, to, PieceType::ROOK);
    *moveList++ = makeQuietPromotion(from, to, PieceType::QUEEN);
    moveCounter += 4;
  }
}

template<PieceColour Colour, bool Promotions>
constexpr void generatePawnDoublePushes(Move *moveList,
                                        int &moveCounter,
                                        Bitboard pawnPositions,
                                        Bitboard emptySquares)
{
  Bitboard targets = pawns::pawnDoublePushTarget<Colour>() & emptySquares;
  Bitboard sources = pawns::pawnSinglePushSource<Colour>(targets) & emptySquares;
  sources = pawns::pawnSinglePushSource<Colour>(sources) & pawnPositions;
  while (sources)
  {
    *moveList++ = makeDoublePush(bitboard::popLS1B(sources), bitboard::popLS1B(targets));
    moveCounter++;
  }
}

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
