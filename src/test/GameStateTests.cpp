#include "GameState.h"
#include "bitboard.h"
#include "pieces.h"
#include <catch2/catch_test_macros.hpp>
#include <cstdint>

namespace yak {

TEST_CASE("Side to move is toggled by state update")
{
  GameStateManager gs{};
  Move move;

  CHECK(gs->sideToMove() == PieceColour::WHITE);
  CHECK(gs->sideNotToMove() == PieceColour::BLACK);

  gs.update(move);
  CHECK(gs->sideToMove() == PieceColour::BLACK);
  CHECK(gs->sideNotToMove() == PieceColour::WHITE);

  gs.update(move);
  CHECK(gs->sideToMove() == PieceColour::WHITE);
  CHECK(gs->sideNotToMove() == PieceColour::BLACK);
}

TEST_CASE("Castling rights")
{
  GameStateManager gs{};

  bool blackKingSide{true};
  bool whiteKingSide{true};
  bool blackQueenSide{true};
  bool whiteQueenSide{true};

  CHECK(gs->canKingSideCastle());
  CHECK(gs->canKingSideCastle(PieceColour::WHITE));
  CHECK(gs->canKingSideCastle(PieceColour::BLACK));
  CHECK_FALSE(gs->canKingSideCastle(PieceColour::NULL_COLOUR));

  CHECK(gs->canQueenSideCastle());
  CHECK(gs->canQueenSideCastle(PieceColour::WHITE));
  CHECK(gs->canQueenSideCastle(PieceColour::BLACK));
  CHECK_FALSE(gs->canQueenSideCastle(PieceColour::NULL_COLOUR));

  Move move;

  // give the to and from squares non-zero default values, so they don't clash with whites queenside rook
  move.from = A2;
  move.to = A2;

  SECTION("Move from black queen side rook square")
  {
    move.from = A8;
    blackQueenSide = false;
  }

  SECTION("Move to black queen side rook square")
  {
    move.to = A8;
    blackQueenSide = false;
  }

  SECTION("Move from black king side rook square")
  {
    move.from = H8;
    blackKingSide = false;
  }

  SECTION("Move to black king side rook square")
  {
    move.to = H8;
    blackKingSide = false;
  }

  SECTION("Move from black king square")
  {
    move.from = E8;
    blackKingSide = false;
    blackQueenSide = false;
  }

  SECTION("Move from white queen side rook square")
  {
    move.from = A1;
    whiteQueenSide = false;
  }

  SECTION("Move to white queen side rook square")
  {
    move.to = A1;
    whiteQueenSide = false;
  }

  SECTION("Move from white king side rook square")
  {
    move.from = H1;
    whiteKingSide = false;
  }

  SECTION("Move to white king side rook square")
  {
    move.to = H1;
    whiteKingSide = false;
  }

  SECTION("Move from white king square")
  {
    move.from = E1;
    whiteKingSide = false;
    whiteQueenSide = false;
  }

  gs.update(move);

  CHECK(gs->canQueenSideCastle(PieceColour::BLACK) == blackQueenSide);
  CHECK(gs->canQueenSideCastle(PieceColour::WHITE) == whiteQueenSide);

  CHECK(gs->canKingSideCastle(PieceColour::BLACK) == blackKingSide);
  CHECK(gs->canKingSideCastle(PieceColour::WHITE) == whiteKingSide);
}

TEST_CASE("Game stack tests")
{
  GameStateManager gs{};
  Move move;

  CHECK(gs->sideToMove() == PieceColour::WHITE);
  CHECK(gs->sideNotToMove() == PieceColour::BLACK);

  move.from = E1; // This move will remove the white castling rights
  gs.update(move);
  CHECK(gs->sideToMove() == PieceColour::BLACK);
  CHECK(gs->sideNotToMove() == PieceColour::WHITE);
  CHECK(gs->canQueenSideCastle(PieceColour::BLACK));
  CHECK(gs->canKingSideCastle(PieceColour::BLACK));
  CHECK_FALSE(gs->canQueenSideCastle(PieceColour::WHITE));
  CHECK_FALSE(gs->canKingSideCastle(PieceColour::WHITE));

  move.from = E8; // This move will remove the black castling rights
  gs.update(move);
  CHECK(gs->sideToMove() == PieceColour::WHITE);
  CHECK(gs->sideNotToMove() == PieceColour::BLACK);
  CHECK_FALSE(gs->canQueenSideCastle(PieceColour::BLACK));
  CHECK_FALSE(gs->canKingSideCastle(PieceColour::BLACK));
  CHECK_FALSE(gs->canQueenSideCastle(PieceColour::WHITE));
  CHECK_FALSE(gs->canKingSideCastle(PieceColour::WHITE));

  const Move* move_p = gs.pop();
  REQUIRE(move_p);
  CHECK(move_p->from == E8);
  CHECK(gs->sideToMove() == PieceColour::BLACK);
  CHECK(gs->sideNotToMove() == PieceColour::WHITE);
  CHECK(gs->canQueenSideCastle(PieceColour::BLACK));
  CHECK(gs->canKingSideCastle(PieceColour::BLACK));
  CHECK_FALSE(gs->canQueenSideCastle(PieceColour::WHITE));
  CHECK_FALSE(gs->canKingSideCastle(PieceColour::WHITE));

  move_p =gs.pop();
  REQUIRE(move_p);
  CHECK(move_p->from == E1);
  CHECK(gs->sideToMove() == PieceColour::WHITE);
  CHECK(gs->sideNotToMove() == PieceColour::BLACK);
  CHECK(gs->canQueenSideCastle(PieceColour::BLACK));
  CHECK(gs->canKingSideCastle(PieceColour::BLACK));
  CHECK(gs->canQueenSideCastle(PieceColour::WHITE));
  CHECK(gs->canKingSideCastle(PieceColour::WHITE));

  move_p = gs.pop();
  CHECK_FALSE(move_p);
}

TEST_CASE("Check that the ep square works")
{
  GameStateManager gs{};
  Move move;

  move.to = A4;
  move.doublePush = true;

  gs.update(move);
  CHECK(gs->sideToMove() == PieceColour::BLACK);
  CHECK(gs->sideNotToMove() == PieceColour::WHITE);
  CHECK(gs->canQueenSideCastle(PieceColour::BLACK));
  CHECK(gs->canKingSideCastle(PieceColour::BLACK));
  CHECK(gs->canQueenSideCastle(PieceColour::WHITE));
  CHECK(gs->canKingSideCastle(PieceColour::WHITE));
  CHECK(gs->epTargetSquare() == A3);

  move.to = A5;
  gs.update(move);
  CHECK(gs->sideToMove() == PieceColour::WHITE);
  CHECK(gs->sideNotToMove() == PieceColour::BLACK);
  CHECK(gs->canQueenSideCastle(PieceColour::BLACK));
  CHECK(gs->canKingSideCastle(PieceColour::BLACK));
  CHECK(gs->canQueenSideCastle(PieceColour::WHITE));
  CHECK(gs->canKingSideCastle(PieceColour::WHITE));
  CHECK(gs->epTargetSquare() == A6);

  move.to = F7;
  move.doublePush = false;
  gs.update(move);
  CHECK(gs->sideToMove() == PieceColour::BLACK);
  CHECK(gs->sideNotToMove() == PieceColour::WHITE);
  CHECK(gs->canQueenSideCastle(PieceColour::BLACK));
  CHECK(gs->canKingSideCastle(PieceColour::BLACK));
  CHECK(gs->canQueenSideCastle(PieceColour::WHITE));
  CHECK(gs->canKingSideCastle(PieceColour::WHITE));
  CHECK(gs->epTargetSquare() == NULL_SQUARE);

  const Move* move_p = gs.pop();
  REQUIRE(move_p);
  CHECK(move_p->to == F7);
  CHECK_FALSE(move_p->doublePush);
  CHECK(gs->sideToMove() == PieceColour::WHITE);
  CHECK(gs->sideNotToMove() == PieceColour::BLACK);
  CHECK(gs->sideNotToMove() == PieceColour::BLACK);
  CHECK(gs->canQueenSideCastle(PieceColour::BLACK));
  CHECK(gs->canKingSideCastle(PieceColour::BLACK));
  CHECK(gs->canQueenSideCastle(PieceColour::WHITE));
  CHECK(gs->canKingSideCastle(PieceColour::WHITE));
  CHECK(gs->epTargetSquare() == A6);

  move_p = gs.pop();
  REQUIRE(move_p);
  CHECK(move_p->to == A5);
  CHECK(move_p->doublePush);
  CHECK(gs->sideToMove() == PieceColour::BLACK);
  CHECK(gs->sideNotToMove() == PieceColour::WHITE);
  CHECK(gs->canQueenSideCastle(PieceColour::BLACK));
  CHECK(gs->canKingSideCastle(PieceColour::BLACK));
  CHECK(gs->canQueenSideCastle(PieceColour::WHITE));
  CHECK(gs->canKingSideCastle(PieceColour::WHITE));
  CHECK(gs->epTargetSquare() == A3);

  move_p = gs.pop();
  REQUIRE(move_p);
  CHECK(move_p->to == A4);
  CHECK(move_p->doublePush);
  CHECK(gs->sideToMove() == PieceColour::WHITE);
  CHECK(gs->sideNotToMove() == PieceColour::BLACK);
  CHECK(gs->canQueenSideCastle(PieceColour::BLACK));
  CHECK(gs->canKingSideCastle(PieceColour::BLACK));
  CHECK(gs->canQueenSideCastle(PieceColour::WHITE));
  CHECK(gs->canKingSideCastle(PieceColour::WHITE));
  CHECK(gs->epTargetSquare() == NULL_SQUARE);

  move_p = gs.pop();
  REQUIRE_FALSE(move_p);
}

TEST_CASE("Check that the move clock works")
{
  GameStateManager gs{};
  Move move;

  CHECK(gs->sideToMove() == PieceColour::WHITE);
  CHECK(gs->sideNotToMove() == PieceColour::BLACK);
  CHECK(gs->moveClock() == 1);
  CHECK(gs->halfMoveClock() == 0);

  gs.update(move);
  CHECK(gs->sideToMove() == PieceColour::BLACK);
  CHECK(gs->sideNotToMove() == PieceColour::WHITE);
  CHECK(gs->moveClock() == 1);
  CHECK(gs->halfMoveClock() == 1);

  gs.update(move);
  CHECK(gs->sideToMove() == PieceColour::WHITE);
  CHECK(gs->sideNotToMove() == PieceColour::BLACK);
  CHECK(gs->moveClock() == 2);
  CHECK(gs->halfMoveClock() == 2);

  gs.update(move);
  CHECK(gs->sideToMove() == PieceColour::BLACK);
  CHECK(gs->sideNotToMove() == PieceColour::WHITE);
  CHECK(gs->moveClock() == 2);
  CHECK(gs->halfMoveClock() == 3);

  const Move* move_p = gs.pop();
  CHECK(move_p);
  CHECK(gs->sideToMove() == PieceColour::WHITE);
  CHECK(gs->sideNotToMove() == PieceColour::BLACK);
  CHECK(gs->moveClock() == 2);
  CHECK(gs->halfMoveClock() == 2);

  move_p = gs.pop();
  CHECK(move_p);
  CHECK(gs->sideToMove() == PieceColour::BLACK);
  CHECK(gs->sideNotToMove() == PieceColour::WHITE);
  CHECK(gs->moveClock() == 1);
  CHECK(gs->halfMoveClock() == 1);

  move_p = gs.pop();
  CHECK(move_p);
  CHECK(gs->sideToMove() == PieceColour::WHITE);
  CHECK(gs->sideNotToMove() == PieceColour::BLACK);
  CHECK(gs->moveClock() == 1);
  CHECK(gs->halfMoveClock() == 0);

  move_p = gs.pop();
  REQUIRE_FALSE(move_p);
}

TEST_CASE("Half move clock is reset by pawn moves and captures")
{
  GameStateManager gs{};
  Move move;

  CHECK(gs->sideToMove() == PieceColour::WHITE);
  CHECK(gs->sideNotToMove() == PieceColour::BLACK);
  CHECK(gs->moveClock() == 1);
  CHECK(gs->halfMoveClock() == 0);

  move.pawnMove = true;
  gs.update(move);
  CHECK(gs->sideToMove() == PieceColour::BLACK);
  CHECK(gs->sideNotToMove() == PieceColour::WHITE);
  CHECK(gs->moveClock() == 1);
  CHECK(gs->halfMoveClock() == 0);

  move.pawnMove = false;
  gs.update(move);
  CHECK(gs->sideToMove() == PieceColour::WHITE);
  CHECK(gs->sideNotToMove() == PieceColour::BLACK);
  CHECK(gs->moveClock() == 2);
  CHECK(gs->halfMoveClock() == 1);

  gs.update(move);
  CHECK(gs->sideToMove() == PieceColour::BLACK);
  CHECK(gs->sideNotToMove() == PieceColour::WHITE);
  CHECK(gs->moveClock() == 2);
  CHECK(gs->halfMoveClock() == 2);

  gs.update(move);
  CHECK(gs->sideToMove() == PieceColour::WHITE);
  CHECK(gs->sideNotToMove() == PieceColour::BLACK);
  CHECK(gs->moveClock() == 3);
  CHECK(gs->halfMoveClock() == 3);

  move.capture = true;
  gs.update(move);
  CHECK(gs->sideToMove() == PieceColour::BLACK);
  CHECK(gs->sideNotToMove() == PieceColour::WHITE);
  CHECK(gs->moveClock() == 3);
  CHECK(gs->halfMoveClock() == 0);

  move.capture = false;
  gs.update(move);
  CHECK(gs->sideToMove() == PieceColour::WHITE);
  CHECK(gs->sideNotToMove() == PieceColour::BLACK);
  CHECK(gs->moveClock() == 4);
  CHECK(gs->halfMoveClock() == 1);

  const Move* move_p = gs.pop();
  CHECK(move_p);
  CHECK(gs->sideToMove() == PieceColour::BLACK);
  CHECK(gs->sideNotToMove() == PieceColour::WHITE);
  CHECK(gs->moveClock() == 3);
  CHECK(gs->halfMoveClock() == 0);

  move_p = gs.pop();
  REQUIRE(move_p);
  CHECK(move_p->capture);
  CHECK(gs->sideToMove() == PieceColour::WHITE);
  CHECK(gs->sideNotToMove() == PieceColour::BLACK);
  CHECK(gs->moveClock() == 3);
  CHECK(gs->halfMoveClock() == 3);

  move_p = gs.pop();
  CHECK(move_p);
  CHECK(gs->sideToMove() == PieceColour::BLACK);
  CHECK(gs->sideNotToMove() == PieceColour::WHITE);
  CHECK(gs->moveClock() == 2);
  CHECK(gs->halfMoveClock() == 2);

  move_p = gs.pop();
  CHECK(move_p);
  CHECK(gs->sideToMove() == PieceColour::WHITE);
  CHECK(gs->sideNotToMove() == PieceColour::BLACK);
  CHECK(gs->moveClock() == 2);
  CHECK(gs->halfMoveClock() == 1);

  move_p = gs.pop();
  CHECK(move_p);
  CHECK(gs->sideToMove() == PieceColour::BLACK);
  CHECK(gs->sideNotToMove() == PieceColour::WHITE);
  CHECK(gs->moveClock() == 1);
  CHECK(gs->halfMoveClock() == 0);

  move_p = gs.pop();
  REQUIRE(move_p);
  CHECK(move_p->pawnMove);
  CHECK(gs->sideToMove() == PieceColour::WHITE);
  CHECK(gs->sideNotToMove() == PieceColour::BLACK);
  CHECK(gs->moveClock() == 1);
  CHECK(gs->halfMoveClock() == 0);
}

} // namespace yak
