#include <catch2/catch_test_macros.hpp>

#include <GameState.h>
#include <types.h>

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
  setFrom(move, A2);
  setTo(move, A2);

  SECTION("Move from black queen side rook square")
  {
    setFrom(move, A8);
    blackQueenSide = false;
  }

  SECTION("Move to black queen side rook square")
  {
    setTo(move, A8);
    blackQueenSide = false;
  }

  SECTION("Move from black king side rook square")
  {
    setFrom(move, H8);
    blackKingSide = false;
  }

  SECTION("Move to black king side rook square")
  {
    setTo(move, H8);
    blackKingSide = false;
  }

  SECTION("Move from black king square")
  {
    setFrom(move, E8);
    blackKingSide = false;
    blackQueenSide = false;
  }

  SECTION("Move from white queen side rook square")
  {
    setFrom(move, A1);
    whiteQueenSide = false;
  }

  SECTION("Move to white queen side rook square")
  {
    setTo(move, A1);
    whiteQueenSide = false;
  }

  SECTION("Move from white king side rook square")
  {
    setFrom(move, H1);
    whiteKingSide = false;
  }

  SECTION("Move to white king side rook square")
  {
    setTo(move, H1);
    whiteKingSide = false;
  }

  SECTION("Move from white king square")
  {
    setFrom(move, E1);
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

  setFrom(move, E1); // This move will remove the white castling rights
  gs.update(move);
  CHECK(gs->sideToMove() == PieceColour::BLACK);
  CHECK(gs->sideNotToMove() == PieceColour::WHITE);
  CHECK(gs->canQueenSideCastle(PieceColour::BLACK));
  CHECK(gs->canKingSideCastle(PieceColour::BLACK));
  CHECK_FALSE(gs->canQueenSideCastle(PieceColour::WHITE));
  CHECK_FALSE(gs->canKingSideCastle(PieceColour::WHITE));

  setFrom(move, E8); // This move will remove the black castling rights
  gs.update(move);
  CHECK(gs->sideToMove() == PieceColour::WHITE);
  CHECK(gs->sideNotToMove() == PieceColour::BLACK);
  CHECK_FALSE(gs->canQueenSideCastle(PieceColour::BLACK));
  CHECK_FALSE(gs->canKingSideCastle(PieceColour::BLACK));
  CHECK_FALSE(gs->canQueenSideCastle(PieceColour::WHITE));
  CHECK_FALSE(gs->canKingSideCastle(PieceColour::WHITE));

  const Move* move_p = gs.pop();
  REQUIRE(move_p);
  CHECK(from(*move_p) == E8);
  CHECK(gs->sideToMove() == PieceColour::BLACK);
  CHECK(gs->sideNotToMove() == PieceColour::WHITE);
  CHECK(gs->canQueenSideCastle(PieceColour::BLACK));
  CHECK(gs->canKingSideCastle(PieceColour::BLACK));
  CHECK_FALSE(gs->canQueenSideCastle(PieceColour::WHITE));
  CHECK_FALSE(gs->canKingSideCastle(PieceColour::WHITE));

  move_p =gs.pop();
  REQUIRE(move_p);
  CHECK(from(*move_p) == E1);
  CHECK(gs->sideToMove() == PieceColour::WHITE);
  CHECK(gs->sideNotToMove() == PieceColour::BLACK);
  CHECK(gs->canQueenSideCastle(PieceColour::BLACK));
  CHECK(gs->canKingSideCastle(PieceColour::BLACK));
  CHECK(gs->canQueenSideCastle(PieceColour::WHITE));
  CHECK(gs->canKingSideCastle(PieceColour::WHITE));

  move_p = gs.pop();
  CHECK_FALSE(move_p);
}

TEST_CASE("Check that the EP square works")
{
  GameStateManager state{};
  Move move;

  setDoublePush(move, A2, A4);

  state.update(move);
  CHECK(state->sideToMove() == PieceColour::BLACK);
  CHECK(state->sideNotToMove() == PieceColour::WHITE);
  CHECK(state->canQueenSideCastle(PieceColour::BLACK));
  CHECK(state->canKingSideCastle(PieceColour::BLACK));
  CHECK(state->canQueenSideCastle(PieceColour::WHITE));
  CHECK(state->canKingSideCastle(PieceColour::WHITE));
  CHECK(state->epTargetSquare() == A3);

  setDoublePush(move, A7, A5);
  state.update(move);
  CHECK(state->sideToMove() == PieceColour::WHITE);
  CHECK(state->sideNotToMove() == PieceColour::BLACK);
  CHECK(state->canQueenSideCastle(PieceColour::BLACK));
  CHECK(state->canKingSideCastle(PieceColour::BLACK));
  CHECK(state->canQueenSideCastle(PieceColour::WHITE));
  CHECK(state->canKingSideCastle(PieceColour::WHITE));
  CHECK(state->epTargetSquare() == A6);

  setTo(move, F7);
  move.doublePush = false;
  state.update(move);
  CHECK(state->sideToMove() == PieceColour::BLACK);
  CHECK(state->sideNotToMove() == PieceColour::WHITE);
  CHECK(state->canQueenSideCastle(PieceColour::BLACK));
  CHECK(state->canKingSideCastle(PieceColour::BLACK));
  CHECK(state->canQueenSideCastle(PieceColour::WHITE));
  CHECK(state->canKingSideCastle(PieceColour::WHITE));
  CHECK(state->epTargetSquare() == NULL_SQUARE);

  const Move* move_p = state.pop();
  REQUIRE(move_p);
  CHECK(to(*move_p) == F7);
  CHECK_FALSE(move_p->doublePush);
  CHECK(state->sideToMove() == PieceColour::WHITE);
  CHECK(state->sideNotToMove() == PieceColour::BLACK);
  CHECK(state->sideNotToMove() == PieceColour::BLACK);
  CHECK(state->canQueenSideCastle(PieceColour::BLACK));
  CHECK(state->canKingSideCastle(PieceColour::BLACK));
  CHECK(state->canQueenSideCastle(PieceColour::WHITE));
  CHECK(state->canKingSideCastle(PieceColour::WHITE));
  CHECK(state->epTargetSquare() == A6);

  move_p = state.pop();
  REQUIRE(move_p);
  CHECK(to(*move_p) == A5);
  CHECK(move_p->doublePush);
  CHECK(state->sideToMove() == PieceColour::BLACK);
  CHECK(state->sideNotToMove() == PieceColour::WHITE);
  CHECK(state->canQueenSideCastle(PieceColour::BLACK));
  CHECK(state->canKingSideCastle(PieceColour::BLACK));
  CHECK(state->canQueenSideCastle(PieceColour::WHITE));
  CHECK(state->canKingSideCastle(PieceColour::WHITE));
  CHECK(state->epTargetSquare() == A3);

  move_p = state.pop();
  REQUIRE(move_p);
  CHECK(to(*move_p) == A4);
  CHECK(move_p->doublePush);
  CHECK(state->sideToMove() == PieceColour::WHITE);
  CHECK(state->sideNotToMove() == PieceColour::BLACK);
  CHECK(state->canQueenSideCastle(PieceColour::BLACK));
  CHECK(state->canKingSideCastle(PieceColour::BLACK));
  CHECK(state->canQueenSideCastle(PieceColour::WHITE));
  CHECK(state->canKingSideCastle(PieceColour::WHITE));
  CHECK(state->epTargetSquare() == NULL_SQUARE);

  move_p = state.pop();
  REQUIRE_FALSE(move_p);
}

TEST_CASE("Check that the move clock works")
{
  GameStateManager state{};
  Move move;

  CHECK(state->sideToMove() == PieceColour::WHITE);
  CHECK(state->sideNotToMove() == PieceColour::BLACK);
  CHECK(state->moveClock() == 1);
  CHECK(state->halfMoveClock() == 0);

  state.update(move);
  CHECK(state->sideToMove() == PieceColour::BLACK);
  CHECK(state->sideNotToMove() == PieceColour::WHITE);
  CHECK(state->moveClock() == 1);
  CHECK(state->halfMoveClock() == 1);

  state.update(move);
  CHECK(state->sideToMove() == PieceColour::WHITE);
  CHECK(state->sideNotToMove() == PieceColour::BLACK);
  CHECK(state->moveClock() == 2);
  CHECK(state->halfMoveClock() == 2);

  state.update(move);
  CHECK(state->sideToMove() == PieceColour::BLACK);
  CHECK(state->sideNotToMove() == PieceColour::WHITE);
  CHECK(state->moveClock() == 2);
  CHECK(state->halfMoveClock() == 3);

  const Move* move_p = state.pop();
  CHECK(move_p);
  CHECK(state->sideToMove() == PieceColour::WHITE);
  CHECK(state->sideNotToMove() == PieceColour::BLACK);
  CHECK(state->moveClock() == 2);
  CHECK(state->halfMoveClock() == 2);

  move_p = state.pop();
  CHECK(move_p);
  CHECK(state->sideToMove() == PieceColour::BLACK);
  CHECK(state->sideNotToMove() == PieceColour::WHITE);
  CHECK(state->moveClock() == 1);
  CHECK(state->halfMoveClock() == 1);

  move_p = state.pop();
  CHECK(move_p);
  CHECK(state->sideToMove() == PieceColour::WHITE);
  CHECK(state->sideNotToMove() == PieceColour::BLACK);
  CHECK(state->moveClock() == 1);
  CHECK(state->halfMoveClock() == 0);

  move_p = state.pop();
  REQUIRE_FALSE(move_p);
}

TEST_CASE("Half move clock is reset by pawn moves and captures")
{
  GameStateManager state{};
  Move move;

  CHECK(state->sideToMove() == PieceColour::WHITE);
  CHECK(state->sideNotToMove() == PieceColour::BLACK);
  CHECK(state->moveClock() == 1);
  CHECK(state->halfMoveClock() == 0);

  move.pawnMove = true;
  state.update(move);
  CHECK(state->sideToMove() == PieceColour::BLACK);
  CHECK(state->sideNotToMove() == PieceColour::WHITE);
  CHECK(state->moveClock() == 1);
  CHECK(state->halfMoveClock() == 0);

  move.pawnMove = false;
  state.update(move);
  CHECK(state->sideToMove() == PieceColour::WHITE);
  CHECK(state->sideNotToMove() == PieceColour::BLACK);
  CHECK(state->moveClock() == 2);
  CHECK(state->halfMoveClock() == 1);

  state.update(move);
  CHECK(state->sideToMove() == PieceColour::BLACK);
  CHECK(state->sideNotToMove() == PieceColour::WHITE);
  CHECK(state->moveClock() == 2);
  CHECK(state->halfMoveClock() == 2);

  state.update(move);
  CHECK(state->sideToMove() == PieceColour::WHITE);
  CHECK(state->sideNotToMove() == PieceColour::BLACK);
  CHECK(state->moveClock() == 3);
  CHECK(state->halfMoveClock() == 3);

  move.capture = true;
  state.update(move);
  CHECK(state->sideToMove() == PieceColour::BLACK);
  CHECK(state->sideNotToMove() == PieceColour::WHITE);
  CHECK(state->moveClock() == 3);
  CHECK(state->halfMoveClock() == 0);

  move.capture = false;
  state.update(move);
  CHECK(state->sideToMove() == PieceColour::WHITE);
  CHECK(state->sideNotToMove() == PieceColour::BLACK);
  CHECK(state->moveClock() == 4);
  CHECK(state->halfMoveClock() == 1);

  const Move* move_p = state.pop();
  CHECK(move_p);
  CHECK(state->sideToMove() == PieceColour::BLACK);
  CHECK(state->sideNotToMove() == PieceColour::WHITE);
  CHECK(state->moveClock() == 3);
  CHECK(state->halfMoveClock() == 0);

  move_p = state.pop();
  REQUIRE(move_p);
  CHECK(move_p->capture);
  CHECK(state->sideToMove() == PieceColour::WHITE);
  CHECK(state->sideNotToMove() == PieceColour::BLACK);
  CHECK(state->moveClock() == 3);
  CHECK(state->halfMoveClock() == 3);

  move_p = state.pop();
  CHECK(move_p);
  CHECK(state->sideToMove() == PieceColour::BLACK);
  CHECK(state->sideNotToMove() == PieceColour::WHITE);
  CHECK(state->moveClock() == 2);
  CHECK(state->halfMoveClock() == 2);

  move_p = state.pop();
  CHECK(move_p);
  CHECK(state->sideToMove() == PieceColour::WHITE);
  CHECK(state->sideNotToMove() == PieceColour::BLACK);
  CHECK(state->moveClock() == 2);
  CHECK(state->halfMoveClock() == 1);

  move_p = state.pop();
  CHECK(move_p);
  CHECK(state->sideToMove() == PieceColour::BLACK);
  CHECK(state->sideNotToMove() == PieceColour::WHITE);
  CHECK(state->moveClock() == 1);
  CHECK(state->halfMoveClock() == 0);

  move_p = state.pop();
  REQUIRE(move_p);
  CHECK(move_p->pawnMove);
  CHECK(state->sideToMove() == PieceColour::WHITE);
  CHECK(state->sideNotToMove() == PieceColour::BLACK);
  CHECK(state->moveClock() == 1);
  CHECK(state->halfMoveClock() == 0);
}

} // namespace yak
