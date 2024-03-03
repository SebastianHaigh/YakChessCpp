#include <catch2/catch_test_macros.hpp>

#include <types.h>

namespace yak {

TEST_CASE("Test setting moved and captured pieces in move")
{
  Move move;

  setMoved(move, PieceType::KING);

  CHECK(moved(move) == PieceType::KING);

  setCaptured(move, PieceType::PAWN);

  CHECK(moved(move) == PieceType::KING);
  CHECK(captured(move) == PieceType::PAWN);

  setFrom(move, A4);

  CHECK(moved(move) == PieceType::KING);
  CHECK(captured(move) == PieceType::PAWN);
  CHECK(from(move) == A4);

  setTo(move, A3);

  CHECK(moved(move) == PieceType::KING);
  CHECK(captured(move) == PieceType::PAWN);
  CHECK(from(move) == A4);
  CHECK(to(move) == A3);

  setMoved(move, PieceType::ROOK);

  CHECK(moved(move) == PieceType::ROOK);
  CHECK(captured(move) == PieceType::PAWN);
  CHECK(from(move) == A4);
  CHECK(to(move) == A3);

  setCaptured(move, PieceType::QUEEN);

  CHECK(moved(move) == PieceType::ROOK);
  CHECK(captured(move) == PieceType::QUEEN);
  CHECK(from(move) == A4);
  CHECK(to(move) == A3);
}

} // namespace yak
