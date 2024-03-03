#include <catch2/catch_test_macros.hpp>

#include <board.h>
#include <bitboard.h>
#include <move.h>
#include <types.h>

#include <string>
#include <iostream>
#include <stddef.h>
#include <string_view>
#include <vector>

namespace yak {

static constexpr std::string_view STANDARD_STARTING_FEN{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };

TEST_CASE("MoveTest: Double pawn push creates EP target square")
{
  Board board{ STANDARD_STARTING_FEN };
  board.makeMove(move::makeDoublePush(A2, A4));

  CHECK(board.toFen() == "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3 0 1");
}

TEST_CASE("MoveTest: Make EP Capture")
{
  Board board{ "8/8/8/8/Pp6/1P6/8/8 b KQkq a3 0 1" };
  board.makeMove(move::makeEpCapture(B4, A3));

  CHECK(board.toFen() == "8/8/8/8/8/pP6/8/8 w KQkq - 0 2");
}

TEST_CASE("MoveTest: Create pawn push move")
{
  Board board{ STANDARD_STARTING_FEN };
  board.makeMove(move::makeQuiet(A2, A3, PieceType::PAWN));

  CHECK(board.toFen() == "rnbqkbnr/pppppppp/8/8/8/P7/1PPPPPPP/RNBQKBNR b KQkq - 0 1");
}

TEST_CASE("MoveTest: Create pawn double push move")
{
  Board board{ STANDARD_STARTING_FEN };
  board.makeMove(move::makeDoublePush(A2, A4));

  CHECK(board.toFen() == "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3 0 1");
}

TEST_CASE("BoardTest: Board can return correct pawn positions")
{
  Board board{ STANDARD_STARTING_FEN };

  Bitboard expected{0}, actual{0};

  SECTION("Pawns") {
    expected = (bitboard::RANK_2 | bitboard::RANK_7);
    actual = board.get_position(PieceType::PAWN);
  }

  SECTION("Knights") {
    expected = (bitboard::RANK_1 | bitboard::RANK_8) & (bitboard::FILE_B | bitboard::FILE_G);
    actual = board.get_position(PieceType::KNIGHT);
  }

  SECTION("Bishops") {
    expected = (bitboard::RANK_1 | bitboard::RANK_8) & (bitboard::FILE_C | bitboard::FILE_F);
    actual = board.get_position(PieceType::BISHOP);
  }

  SECTION("Rooks") {
    expected = (bitboard::RANK_1 | bitboard::RANK_8) & (bitboard::FILE_A | bitboard::FILE_H);
    actual = board.get_position(PieceType::ROOK);
  }

  SECTION("Queens") {
    expected = (bitboard::RANK_1 | bitboard::RANK_8) & bitboard::FILE_D;
    actual = board.get_position(PieceType::QUEEN);
  }

  SECTION("Kings") {
    expected = (bitboard::RANK_1 | bitboard::RANK_8) & bitboard::FILE_E;
    actual = board.get_position(PieceType::KING);
  }

  SECTION("White") {
    expected = (bitboard::RANK_1 | bitboard::RANK_2);
    actual = board.get_position(PieceColour::WHITE);
  }

  SECTION("Black") {
    expected = (bitboard::RANK_7 | bitboard::RANK_8);
    actual = board.get_position(PieceColour::BLACK);
  }

  SECTION("White Pawns") {
    expected = bitboard::RANK_2;
    actual = board.getPosition(PieceColour::WHITE, PieceType::PAWN);
  }

  SECTION("White Knights") {
    expected = bitboard::RANK_1 & (bitboard::FILE_B | bitboard::FILE_G);
    actual = board.getPosition(PieceColour::WHITE, PieceType::KNIGHT);
  }

  SECTION("White Bishops") {
     expected = bitboard::RANK_1 & (bitboard::FILE_C | bitboard::FILE_F);
     actual = board.getPosition(PieceColour::WHITE, PieceType::BISHOP);
  }

  SECTION("White Rooks") {
    expected = bitboard::RANK_1 & (bitboard::FILE_A | bitboard::FILE_H);
    actual = board.getPosition(PieceColour::WHITE, PieceType::ROOK);
  }

  SECTION("White Queens") {
    expected = bitboard::RANK_1 & bitboard::FILE_D;
    actual = board.getPosition(PieceColour::WHITE, PieceType::QUEEN);
  }

  SECTION("White Kings") {
    expected = bitboard::RANK_1 & bitboard::FILE_E;
    actual = board.getPosition(PieceColour::WHITE, PieceType::KING);
  }

  SECTION("Black Pawns") {
    expected = bitboard::RANK_7;
    actual = board.getPosition(PieceColour::BLACK, PieceType::PAWN);
  }

  SECTION("Black Knights") {
    expected = bitboard::RANK_8 & (bitboard::FILE_B | bitboard::FILE_G);
    actual = board.getPosition(PieceColour::BLACK, PieceType::KNIGHT);
  }

  SECTION("Black Bishops") {
    expected = bitboard::RANK_8 & (bitboard::FILE_C | bitboard::FILE_F);
    actual = board.getPosition(PieceColour::BLACK, PieceType::BISHOP);
  }

  SECTION("Black Rooks") {
    expected = bitboard::RANK_8 & (bitboard::FILE_A | bitboard::FILE_H);
    actual = board.getPosition(PieceColour::BLACK, PieceType::ROOK);
  }

  SECTION("Black Queens") {
    expected = bitboard::RANK_8 & bitboard::FILE_D;
    actual = board.getPosition(PieceColour::BLACK, PieceType::QUEEN);
  }

  SECTION("Black Kings") {
    expected = bitboard::RANK_8 & bitboard::FILE_E;
    actual = board.getPosition(PieceColour::BLACK, PieceType::KING);
  }

  SECTION("Empty") {
    expected = bitboard::RANK_3 | bitboard::RANK_4 | bitboard::RANK_5 | bitboard::RANK_6;
    actual = board.emptySquares();
  }

  CHECK(actual == expected);
}

TEST_CASE("BoardTest: EP square loads")
{
  Board board{ "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3 0 1" };

  CHECK(board.toFen() == "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3 0 1");
}

TEST_CASE("BoardTest: Detects castling rights from fen all")
{
  std::string_view startingFen;

  struct HasCastlingRights
  {
    bool m_k_black;
    bool m_q_black;
    bool m_k_white;
    bool m_q_white;
  } expectedCastlingRights;

  SECTION("Standard starting postion")
  {
    startingFen = STANDARD_STARTING_FEN;
    expectedCastlingRights = HasCastlingRights{ true, true, true, true };
  }

  SECTION("No king side white")
  {
    startingFen = "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b Qkq a3 0 1";
    expectedCastlingRights = HasCastlingRights{ true, true, false, true };
  }

  Board board{ startingFen };

  CHECK(expectedCastlingRights.m_k_black == board.canKingSideCastle(PieceColour::BLACK));
  CHECK(expectedCastlingRights.m_k_white == board.canKingSideCastle(PieceColour::WHITE));
  CHECK(expectedCastlingRights.m_q_black == board.canQueenSideCastle(PieceColour::BLACK));
  CHECK(expectedCastlingRights.m_q_white == board.canQueenSideCastle(PieceColour::WHITE));
}

TEST_CASE("BoardTest: Castling rights change when rook is moved or is captured")
{
  Board board{ "rnbqkbnr/ppppppp1/8/8/8/8/PPPPPPP1/RNBQKBNR b KQkq - 0 1" };

  board.makeMove(move::makeCapture(H8, H1, PieceType::ROOK, PieceType::ROOK));

  CHECK(board.toFen() == "rnbqkbn1/ppppppp1/8/8/8/8/PPPPPPP1/RNBQKBNr w Qq - 0 2");
}

TEST_CASE("BoardTest: Undoing move restores state")
{
  Board board{ STANDARD_STARTING_FEN };

  board.makeMove(move::makeQuiet(E2, E4, PieceType::PAWN));

  board.undoMove();

  CHECK(board.toFen() == STANDARD_STARTING_FEN);
}

TEST_CASE("BoardTest: Make and undo all moves to depth 3", "[.tdd]")
{
  yak::Board board{ STANDARD_STARTING_FEN };

  std::vector<Move> moves = board.generateMoves();

  CHECK(moves.size() == 20);

  size_t secondMoveTotal{0};
  size_t thirdMoveTotal{0};
  size_t fourthMoveTotal{0};

  for (const auto& move : moves)
  {
    std::cout << "making a first move WHITE" << std::endl;
    yak::Board::MoveResult makeResult = board.makeMove(move);
    CHECK(makeResult == yak::Board::MoveResult::SUCCESS);

    std::vector<Move> secondMoves = board.generateMoves();

    secondMoveTotal += secondMoves.size();

    for (const auto& secondMove : secondMoves)
    {
      yak::Board::MoveResult makeResult = board.makeMove(secondMove);
      CHECK(makeResult == yak::Board::MoveResult::SUCCESS);

      std::vector<Move> thirdMoves = board.generateMoves();
      thirdMoveTotal += thirdMoves.size();

      for (const auto& thirdMove : thirdMoves)
      {
        yak::Board::MoveResult makeResult = board.makeMove(thirdMove);
        CHECK(makeResult == yak::Board::MoveResult::SUCCESS);

        std::vector<Move> fourthMoves = board.generateMoves();
        fourthMoveTotal += fourthMoves.size();

        for (const auto& fourthMove : fourthMoves)
        {
          yak::Board::MoveResult makeResult = board.makeMove(fourthMove);
          CHECK(makeResult == yak::Board::MoveResult::SUCCESS);

          yak::Board::MoveResult undoResult = board.undoMove();
          CHECK(makeResult == yak::Board::MoveResult::SUCCESS);
        }

        yak::Board::MoveResult undoResult = board.undoMove();
        CHECK(makeResult == yak::Board::MoveResult::SUCCESS);
      }

      yak::Board::MoveResult undoResult = board.undoMove();
      CHECK(makeResult == yak::Board::MoveResult::SUCCESS);
    }

    yak::Board::MoveResult undoResult = board.undoMove();
    CHECK(makeResult == yak::Board::MoveResult::SUCCESS);
  }

  CHECK(secondMoveTotal == 400);
  CHECK(thirdMoveTotal == 8902);
  CHECK(fourthMoveTotal == 197281);
  CHECK(board.toFen() == STANDARD_STARTING_FEN);
}

TEST_CASE("BoardTest: Calculates all attacked squares")
{
  Board board{ STANDARD_STARTING_FEN};

  CHECK(board.attacked_by(PieceColour::WHITE) == bitboard::RANK_3);
  CHECK(board.attacked_by(PieceColour::BLACK) == bitboard::RANK_6);
}

TEST_CASE("BoardTest: Calculates all attacked squares 2")
{
  Board board{ "r1bqkbnr/pppp1ppp/2n5/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1" };

  const Bitboard attackedByPawns = bitboard::static_bitboard<A3, B3, C3, D3, E3, G3, H3, F5>::value;
  const Bitboard attackedByKnights = bitboard::static_bitboard<D4, E5, G5, H4, G1>::value;
  const Bitboard attackedByBishops = bitboard::static_bitboard<A6, B5, D3, E2, F1, B3, D5, E6, F7>::value;
  const Bitboard attackedByKings = bitboard::static_bitboard<E2, F1>::value;

  const Bitboard attackedByWhite = attackedByPawns | attackedByKnights | attackedByBishops | attackedByKings;

  CHECK(attackedByWhite == board.attacked_by(PieceColour::WHITE));
}

TEST_CASE("BoardTest: Can detect check")
{
  Board board{ "rnbqkbnr/ppp2ppp/3p4/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1" };
  CHECK(board.isCheck());
}

TEST_CASE("BoardTest: Can detect not check")
{
  Board board{ "rnbqkbnr/pp3ppp/2pp4/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1" };
  CHECK_FALSE(board.isCheck());
}

TEST_CASE("BoardMoveGenerationTests: Detects Legal Captures With White Pawns")
{
  //   Test Board       W Pawn Targets
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . 1 . . . . . .
  // . . p . . . . .    . 1 1 . . . . .
  // . P . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .

  Board board{ "8/8/8/8/8/2p5/1P6/8 w - - 0 1" };

  std::vector<Move> moves = board.generateMoves();

  CHECK(moves.size() == 3);
}

TEST_CASE("BoardMoveGenerationTests: Detects legal captures with black pawns")
{
  //   Test Board       W Pawn Targets
  // . . . . . . . .    . . . . . . . .
  // . P . . . . . .    . . . . . . . .
  // . . p . . . . .    . 1 1 . . . . .
  // . . . . . . . .    . 1 . . . . . .
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .

  Board board{ "8/1p6/2P5/8/8/8/8/8 b - - 0 1" };

  std::vector<Move> moves = board.generateMoves();

  CHECK(moves.size() == 3);
}

TEST_CASE("BoardMoveGenerationTests: Kiwipete")
{
  Board board{ "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - " };

  std::vector<Move> moves = board.generateMoves();

  CHECK(moves.size() == 48);
}

TEST_CASE("BoardMoveGenerationTests: Can generate and make ep capture")
{
  //  Before Capture     After Capture
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .
  // P p . . . . . .    . . . . . . . .
  // . P . . . . . .    p P . . . . . .
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .

  Board board{ "8/8/8/8/Pp6/1P6/8/8 b - a3 0 1" };

  std::vector<Move> moves = board.generateMoves();

  for (auto move : moves)
  {
    if (move.enPassant)
    {
      board.makeMove(move);
      break;
    }
  }

  CHECK(board.toFen() == "8/8/8/8/8/pP6/8/8 w - - 0 2");
}

TEST_CASE("BoardMoveGenerationTests: Can castle king side white only king and rook")
{
  Board board{ "8/8/8/8/8/8/8/4K2R w K - 0 1" };

  std::vector<Move> moves = board.generateMoves();

  for (auto move : moves)
  {
    if (move.castle != PieceType::NULL_PIECE)
    {
      board.makeMove(move);
      break;
    }
  }

  CHECK(board.toFen() == "8/8/8/8/8/8/8/5RK1 b - - 0 1");
}

TEST_CASE("BoardMoveGenerationTests: Can castle king side white")
{
  Board board{ "rnbqkbnr/ppp2ppp/3p4/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1" };

  std::vector<Move> moves = board.generateMoves();

  for (auto move : moves)
  {
    if (move.castle != PieceType::NULL_PIECE)
    {
      board.makeMove(move);
      break;
    }
  }

  CHECK(board.toFen() == "rnbqkbnr/ppp2ppp/3p4/4p3/2B1P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 0 1");
}

TEST_CASE("BoardMoveGenerationTests: Can castle king side black")
{
  Board board{ "rnbqk2r/ppp2pbp/3p1np1/3Pp3/2B1P3/5N2/PPP2PPP/RNBQ1RK1 b kq - 0 1" };

  std::vector<Move> moves = board.generateMoves();

  for (auto move : moves)
  {
    if (move.castle != PieceType::NULL_PIECE)
    {
      board.makeMove(move);
      break;
    }
  }

  CHECK(board.toFen() == "rnbq1rk1/ppp2pbp/3p1np1/3Pp3/2B1P3/5N2/PPP2PPP/RNBQ1RK1 w - - 0 2");
}

TEST_CASE("BoardMoveGenerationTests: CanCastleQueenSideWhite")
{
  Board board{ "rnbqk2r/ppp2pbp/3p1np1/3Pp3/2B1P3/2N1B3/PPPQ1PPP/R3K1NR w KQkq - 0 1" };

  std::vector<Move> moves = board.generateMoves();

  for (auto move : moves)
  {
    if (move.castle != PieceType::NULL_PIECE)
    {
      board.makeMove(move);
      break;
    }
  }

  CHECK(board.toFen() == "rnbqk2r/ppp2pbp/3p1np1/3Pp3/2B1P3/2N1B3/PPPQ1PPP/2KR2NR b kq - 0 1");
}

TEST_CASE("BoardMoveGenerationTests: Can castle queen side black")
{
  Board board{ "r3kbnr/ppp2ppp/3pb3/4p1q1/4Pn2/3P4/PPP2PPP/RNBQKBNR b KQkq - 0 1" };

  std::vector<Move> moves = board.generateMoves();
  bool foundCastle{false};

  for (auto& move : moves)
  {
    if (move.castle != PieceType::NULL_PIECE)
    {
      yak::Board::MoveResult result = board.makeMove(move);
      CHECK(result == yak::Board::MoveResult::SUCCESS);
      foundCastle = true;
      break;
    }
  }

  CHECK(foundCastle);
  CHECK(board.toFen() == "2kr1bnr/ppp2ppp/3pb3/4p1q1/4Pn2/3P4/PPP2PPP/RNBQKBNR w KQ - 0 2");
}

TEST_CASE("BoardMoveGenerationTests: Pawn can promote")
{
  Board board{ "8/P7/8/8/8/8/8/8 w - - 0 1" };

  std::vector<Move> moves = board.generateMoves();

  board.makeMove(moves[0]);
  CHECK(board.toFen() == "N7/8/8/8/8/8/8/8 b - - 0 1");
  board.undoMove();

  board.makeMove(moves[1]);
  CHECK(board.toFen() == "B7/8/8/8/8/8/8/8 b - - 0 1");
  board.undoMove();

  board.makeMove(moves[2]);
  CHECK(board.toFen() == "R7/8/8/8/8/8/8/8 b - - 0 1");
  board.undoMove();

  board.makeMove(moves[3]);
  CHECK(board.toFen() == "Q7/8/8/8/8/8/8/8 b - - 0 1");
}

TEST_CASE("BoardMoveGenerationTests: Detects all moves in position")
{
  std::string_view fen{ };
  size_t expectedNumMoves{ 0 };

  SECTION("Position 1") {
    fen = STANDARD_STARTING_FEN;
    expectedNumMoves = 20;
  }

  SECTION("Position 2") {
    // r n b q k b n r
    // p p p . . p p p
    // . . . p . . . .
    // . . . . p . . .
    // . . . . P . . .
    // . . . . . N . .
    // P P P P . P P P
    // R N B Q K B . R
    fen = "rnbqkbnr/ppp2ppp/3p4/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 0 1";
    expectedNumMoves = 27;
  }

  SECTION("Position 3") {
    // r n b q k b n r
    // p p p . . . p p
    // . . . p . p . .
    // . . . . p . . .
    // . . . P P . . .
    // . . . . . N . .
    // P P P . . P P P
    // R N B Q K B . R
    fen = "rnbqkbnr/ppp3pp/3p1p2/4p3/3PP3/5N2/PPP2PPP/RNBQKB1R w KQkq - 0 1";
    expectedNumMoves = 36;
  }

  SECTION("Position 4") {
    // r n b q k b n r
    // p p . . . . p p
    // . . p p . p . .
    // . B . . p . . .
    // . . . P P . . .
    // . . . . . N . .
    // P P P . . P P P
    // R N B Q K . . R
    fen = "rnbqkbnr/pp4pp/2pp1p2/1B2p3/3PP3/5N2/PPP2PPP/RNBQK2R w KQkq - 0 1";
    expectedNumMoves = 41;
  }

  Board board{fen};
  std::vector<Move> moves = board.generateMoves();
  CHECK(moves.size() == expectedNumMoves);
}

TEST_CASE("BoardMoveGenerationTests: Detects legal moves when in check")
{
  // r n b q k b n r
  // p p p . . p p p
  // . . . p . . . .
  // . B . . p . . .
  // . . . . P . . .
  // . . . . . N . .
  // P P P P . P P P
  // R N B Q K . . R

  Board board{ "rnbqkbnr/ppp2ppp/3p4/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1" };

  std::vector<Move> moves = board.generateMoves();

  // The only legal moves are the six that remove the check: c6, Nc6, Nd7, Bd7, Qc7, Ke7
  CHECK(moves.size() == 6);
}

TEST_CASE("BoardMoveGenerationTests: Detects a situation that is not checkmate")
{
  // r n b q k b n r
  // p p p . . p p p
  // . . . p . . . .
  // . B . . p . . .
  // . . . . P . . .
  // . . . . . N . .
  // P P P P . P P P
  // R N B Q K . . R

  Board board{ "rnbqkbnr/ppp2ppp/3p4/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1" };

  CHECK_FALSE(board.isCheckmate());
}

TEST_CASE("BoardMoveGenerationTests: Detects a situation that is checkmate")
{
  // r . . q . b k r
  // p p p . . . p p
  // . . n . B . . .
  // . . . . p . . .
  // . . . . . . . .
  // . . . . . Q . .
  // P P P P . P P P
  // R N B   K . . R

  Board board{ "r2q1bkr/ppp3pp/2n1B3/4p3/8/5Q2/PPPP1PPP/RNB1K2R b KQkq - 0 1" };

  CHECK(board.isCheckmate());
}

TEST_CASE("BoardMoveGenerationTests: Cannot castle out of check")
{
  // . . . . . . . .
  // . . . . . . . .
  // . . . . r . . .
  // . . . . . . . .
  // . . . . . . . .
  // . . . . . . . .
  // . . . . . . . .
  // . . . . K . . R

  Board board{ "8/8/4r3/8/8/8/8/4K2R w K - 0 1" };

  std::vector<Move> moves = board.generateMoves();

  for (const auto& move : moves)
  {
    CHECK(move.castle == PieceType::NULL_PIECE);
  }
}

} // namespace yak
