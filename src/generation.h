#ifndef GENERATION_PIECES_H_
#define GENERATION_PIECES_H_

// Can this be moved into the board?
// TODO I need to update this method so that it can save the piece to be capture if there is a capture
#include "bitboard.h"
#include "move.h"
#include "pieces.h"
#include "attackmaps.h"
#include "pawns.h"

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

template<PieceColour Colour, bool Promotions>
constexpr void generatePawnWestCaptures(Move *moveList,
                                        int &moveCounter,
                                        Bitboard pawnPositions,
                                        Bitboard opponentPieces)
{
  pawnPositions = Promotions ? pawns::promotablePawns<Colour>(pawnPositions) : pawns::nonPromotablePawns<Colour>(pawnPositions);
  Bitboard sources = pawns::pawnWestAttackSource<Colour>(opponentPieces) & pawnPositions;
  Bitboard targets = pawns::pawnWestAttackTarget<Colour>(sources);

  while (sources && not Promotions)
  {
    *moveList = makeCapture(bitboard::popLS1B(sources), bitboard::popLS1B(targets));
    moveCounter++;
  }

  while (sources && Promotions)
  {
    Square from = bitboard::popLS1B(sources);
    Square to = bitboard::popLS1B(targets);
    *moveList++ = makeCapturePromotion(from, to, PieceType::KNIGHT);
    *moveList++ = makeCapturePromotion(from, to, PieceType::BISHOP);
    *moveList++ = makeCapturePromotion(from, to, PieceType::ROOK);
    *moveList++ = makeCapturePromotion(from, to, PieceType::QUEEN);
    moveCounter += 4;
  }
}

template<PieceColour Colour, bool Promotions>
constexpr void generatePawnEastCaptures(Move *moveList,
                                        int &moveCounter,
                                        Bitboard pawnPositions,
                                        Bitboard opponentPieces)
{
  pawnPositions = Promotions ? pawns::promotablePawns<Colour>(pawnPositions) : pawns::nonPromotablePawns<Colour>(pawnPositions);
  Bitboard sources = pawns::pawnEastAttackSource<Colour>(opponentPieces) & pawnPositions;
  Bitboard targets = pawns::pawnEastAttackTarget<Colour>(sources);
  while (sources && !Promotions)
  {
    *moveList++ = makeCapture(bitboard::popLS1B(sources), bitboard::popLS1B(targets));
    moveCounter++;
  }

  while (sources && Promotions)
  {
    Square from = bitboard::popLS1B(sources);
    Square to = bitboard::popLS1B(targets);
    *moveList++ = makeCapturePromotion(from, to, PieceType::KNIGHT);
    *moveList++ = makeCapturePromotion(from, to, PieceType::BISHOP);
    *moveList++ = makeCapturePromotion(from, to, PieceType::ROOK);
    *moveList++ = makeCapturePromotion(from, to, PieceType::QUEEN);
    moveCounter += 4;
  }
}

template<PieceColour Colour>
void generatePawnMoves(Move *moveList,
                       int &moveCounter,
                       Bitboard pawnPositions,
                       Bitboard emptySquares,
                       Bitboard opponentPieces)
{
  /* NOT PROMOTIONS             */
  /* ----------------------------- */
  generatePawnSinglePushes<Colour, false>(&moveList[moveCounter], moveCounter, pawnPositions, emptySquares);
  generatePawnDoublePushes<Colour, false>(&moveList[moveCounter], moveCounter, pawnPositions, emptySquares);
  generatePawnWestCaptures<Colour, false>(&moveList[moveCounter], moveCounter, pawnPositions, opponentPieces);
  generatePawnEastCaptures<Colour, false>(&moveList[moveCounter], moveCounter, pawnPositions, opponentPieces);

  /* PROMOTIONS                    */
  /* ----------------------------- */
  /* generate_pawn_single_pushes<C, true>(&moveList[moveCounter], moveCounter, pawnPositions, emptySquares); */
  /* generatePawnWestCaptures<C, true>(&moveList[moveCounter], moveCounter, pawnPositions, opponentPieces); */
  /* generate_pawn_east_captures<C, true>(&moveList[moveCounter], moveCounter, pawnPositions, opponentPieces); */
};

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

template<PieceType Type>
int generatePieceMoves(Move *moveList,
                       Bitboard piecePositions,
                       Bitboard emptySquares,
                       Bitboard opponentPieces)
{
  int moveCounter{ 0 };

  while (piecePositions)
  {
    Square from = bitboard::popLS1B(piecePositions);
    Bitboard atk_bb = (Type == PieceType::KNIGHT) ? piece::KnightMap::attacks(from) :
                      (Type == PieceType::BISHOP) ? attackmap::BishopMap::attacks(from, ~emptySquares) :
                      (Type == PieceType::ROOK) ? attackmap::RookMap::attacks(from, ~emptySquares) :
                      (Type == PieceType::QUEEN) ? attackmap::QueenMap::attacks(from, ~emptySquares) :
                      (Type == PieceType::KING) ? piece::KingMap::attacks(from) : Bitboard{0};

    Bitboard quiet = atk_bb & emptySquares;
    while (quiet)
    {
      *moveList++ = makeQuiet(from, bitboard::popLS1B(quiet));
      ++moveCounter;
    }

    Bitboard capture = atk_bb & opponentPieces;

    while (capture)
    {
      *moveList++ = makeCapture(from, bitboard::popLS1B(capture));
      ++moveCounter;
    }
  }

  return moveCounter;
}

} // namespace yak::move

#endif // GENERATION_PIECES_H_
