#include <catch2/catch_test_macros.hpp>

#include <board.h>
#include <bitboard.h>
#include <move.hpp>
#include <types.h>

#include <stddef.h>
#include <string_view>
#include <vector>
#include <algorithm>

namespace yak {

static constexpr std::string_view STANDARD_STARTING_FEN{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };

std::vector<Move>::const_iterator findFirstCastlingMove(std::vector<Move>::const_iterator begin_p,
                                                        std::vector<Move>::const_iterator end_p)
{
  return std::find_if(begin_p, end_p,
                      [] (const Move& move)
                      {
                        return (isCastle(move));
                      });
}

std::vector<Move>::const_iterator findFirstEpMove(std::vector<Move>::const_iterator begin_p,
                                                  std::vector<Move>::const_iterator end_p)
{
  return std::find_if(begin_p, end_p,
                      [] (const Move& move)
                      {
                        return isEnPassant(move);
                      });
}

TEST_CASE("MoveTest 1")
{
  Board board{ STANDARD_STARTING_FEN };
  CHECK(board.toFen() == STANDARD_STARTING_FEN);

  // Undo restores state
  board.makeMove(makeQuiet(E2, E4, PieceType::PAWN));
  board.undoMove();
  CHECK(board.toFen() == STANDARD_STARTING_FEN);

  // Double pawn push
  board.makeMove(makeDoublePush(A2, A4));
  CHECK(board.toFen() == "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3 0 1");

  // Single pawn push
  board.reset(STANDARD_STARTING_FEN);
  CHECK(board.toFen() == STANDARD_STARTING_FEN);
  board.makeMove(makeQuiet(A2, A3, PieceType::PAWN));
  CHECK(board.toFen() == "rnbqkbnr/pppppppp/8/8/8/P7/1PPPPPPP/RNBQKBNR b KQkq - 0 1");

  // EP capture
  board.reset("8/8/8/8/Pp6/1P6/8/8 b KQkq a3 0 1");
  CHECK(board.toFen() == "8/8/8/8/Pp6/1P6/8/8 b KQkq a3 0 1");
  board.makeMove(makeEpCapture(B4, A3));
  CHECK(board.toFen() == "8/8/8/8/8/pP6/8/8 w KQkq - 0 2");
}

TEST_CASE("BoardTest: Board can return correct piece positions")
{
  Board board{ STANDARD_STARTING_FEN };

  Bitboard expected{0}, actual{0};

  {
    expected = (bitboard::RANK_2 | bitboard::RANK_7);
    actual = board.get_position(PieceType::PAWN);
    CHECK(actual == expected);
  }

  {
    expected = (bitboard::RANK_1 | bitboard::RANK_8) & (bitboard::FILE_B | bitboard::FILE_G);
    actual = board.get_position(PieceType::KNIGHT);
    CHECK(actual == expected);
  }

  {
    expected = (bitboard::RANK_1 | bitboard::RANK_8) & (bitboard::FILE_C | bitboard::FILE_F);
    actual = board.get_position(PieceType::BISHOP);
    CHECK(actual == expected);
  }

  {
    expected = (bitboard::RANK_1 | bitboard::RANK_8) & (bitboard::FILE_A | bitboard::FILE_H);
    actual = board.get_position(PieceType::ROOK);
    CHECK(actual == expected);
  }

  {
    expected = (bitboard::RANK_1 | bitboard::RANK_8) & bitboard::FILE_D;
    actual = board.get_position(PieceType::QUEEN);
    CHECK(actual == expected);
  }

  {
    expected = (bitboard::RANK_1 | bitboard::RANK_8) & bitboard::FILE_E;
    actual = board.get_position(PieceType::KING);
    CHECK(actual == expected);
  }

  {
    expected = (bitboard::RANK_1 | bitboard::RANK_2);
    actual = board.get_position(PieceColour::WHITE);
    CHECK(actual == expected);
  }

  {
    expected = (bitboard::RANK_7 | bitboard::RANK_8);
    actual = board.get_position(PieceColour::BLACK);
    CHECK(actual == expected);
  }

  {
    expected = bitboard::RANK_2;
    actual = board.getPosition(PieceColour::WHITE, PieceType::PAWN);
    CHECK(actual == expected);
  }

  {
    expected = bitboard::RANK_1 & (bitboard::FILE_B | bitboard::FILE_G);
    actual = board.getPosition(PieceColour::WHITE, PieceType::KNIGHT);
    CHECK(actual == expected);
  }

  {
     expected = bitboard::RANK_1 & (bitboard::FILE_C | bitboard::FILE_F);
     actual = board.getPosition(PieceColour::WHITE, PieceType::BISHOP);
    CHECK(actual == expected);
  }

  {
    expected = bitboard::RANK_1 & (bitboard::FILE_A | bitboard::FILE_H);
    actual = board.getPosition(PieceColour::WHITE, PieceType::ROOK);
    CHECK(actual == expected);
  }

  {
    expected = bitboard::RANK_1 & bitboard::FILE_D;
    actual = board.getPosition(PieceColour::WHITE, PieceType::QUEEN);
    CHECK(actual == expected);
  }

  {
    expected = bitboard::RANK_1 & bitboard::FILE_E;
    actual = board.getPosition(PieceColour::WHITE, PieceType::KING);
    CHECK(actual == expected);
  }

  {
    expected = bitboard::RANK_7;
    actual = board.getPosition(PieceColour::BLACK, PieceType::PAWN);
    CHECK(actual == expected);
  }

  {
    expected = bitboard::RANK_8 & (bitboard::FILE_B | bitboard::FILE_G);
    actual = board.getPosition(PieceColour::BLACK, PieceType::KNIGHT);
    CHECK(actual == expected);
  }

  {
    expected = bitboard::RANK_8 & (bitboard::FILE_C | bitboard::FILE_F);
    actual = board.getPosition(PieceColour::BLACK, PieceType::BISHOP);
    CHECK(actual == expected);
  }

  {
    expected = bitboard::RANK_8 & (bitboard::FILE_A | bitboard::FILE_H);
    actual = board.getPosition(PieceColour::BLACK, PieceType::ROOK);
    CHECK(actual == expected);
  }

  {
    expected = bitboard::RANK_8 & bitboard::FILE_D;
    actual = board.getPosition(PieceColour::BLACK, PieceType::QUEEN);
    CHECK(actual == expected);
  }

  {
    expected = bitboard::RANK_8 & bitboard::FILE_E;
    actual = board.getPosition(PieceColour::BLACK, PieceType::KING);
    CHECK(actual == expected);
  }

  {
    expected = bitboard::RANK_3 | bitboard::RANK_4 | bitboard::RANK_5 | bitboard::RANK_6;
    actual = board.emptySquares();
    CHECK(actual == expected);
  }
}

TEST_CASE("Castling rights")
{
  Board board{ STANDARD_STARTING_FEN };
  CHECK(board.toFen() == STANDARD_STARTING_FEN);

  CHECK(board.canKingSideCastle(PieceColour::BLACK));
  CHECK(board.canKingSideCastle(PieceColour::WHITE));
  CHECK(board.canQueenSideCastle(PieceColour::BLACK));
  CHECK(board.canQueenSideCastle(PieceColour::WHITE));

  board.reset("rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b Qkq a3 0 1");
  CHECK(board.toFen() == "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b Qkq a3 0 1");
  CHECK(board.canKingSideCastle(PieceColour::BLACK));
  CHECK_FALSE(board.canKingSideCastle(PieceColour::WHITE));
  CHECK(board.canQueenSideCastle(PieceColour::BLACK));
  CHECK(board.canQueenSideCastle(PieceColour::WHITE));

  board.reset("rnbqkbnr/ppppppp1/8/8/8/8/PPPPPPP1/RNBQKBNR b KQkq - 0 1");
  CHECK(board.toFen() == "rnbqkbnr/ppppppp1/8/8/8/8/PPPPPPP1/RNBQKBNR b KQkq - 0 1");
  board.makeMove(makeCapture(H8, H1, PieceType::ROOK, PieceType::ROOK));
  CHECK(board.toFen() == "rnbqkbn1/ppppppp1/8/8/8/8/PPPPPPP1/RNBQKBNr w Qq - 0 2");
}

TEST_CASE("BoardTest: Make and undo all moves to depth 3")
{
  yak::Board board{ STANDARD_STARTING_FEN };

  std::vector<Move> moves = board.generateMoves();

  CHECK(moves.size() == 20);

  size_t secondMoveTotal{0};
  size_t thirdMoveTotal{0};
  size_t fourthMoveTotal{0};

  for (const auto& move : moves)
  {
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

TEST_CASE("Attack square calculation")
{
  Board board{ STANDARD_STARTING_FEN};
  CHECK(board.toFen() == STANDARD_STARTING_FEN);

  CHECK(board.attacked_by(PieceColour::WHITE) == bitboard::RANK_3);
  CHECK(board.attacked_by(PieceColour::BLACK) == bitboard::RANK_6);

  board.reset("r1bqkbnr/pppp1ppp/2n5/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1");
  CHECK(board.toFen() == "r1bqkbnr/pppp1ppp/2n5/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1");
  const Bitboard attackedByPawns = bitboard::static_bitboard<A3, B3, C3, D3, E3, G3, H3, F5>::value;
  const Bitboard attackedByKnights = bitboard::static_bitboard<D4, E5, G5, H4, G1>::value;
  const Bitboard attackedByBishops = bitboard::static_bitboard<A6, B5, D3, E2, F1, B3, D5, E6, F7>::value;
  const Bitboard attackedByKings = bitboard::static_bitboard<E2, F1>::value;
  const Bitboard attackedByWhite = attackedByPawns | attackedByKnights | attackedByBishops | attackedByKings;
  CHECK(attackedByWhite == board.attacked_by(PieceColour::WHITE));
}

TEST_CASE("Move generation")
{
  std::vector<Move> moves;
  Board board{ "8/8/8/8/8/8/PPPPPPPP/8 w - - 0 1" };

  //   Test Board       W Pawn Targets
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    1 1 1 1 1 1 1 1
  // . . . . . . . .    1 1 1 1 1 1 1 1
  // P P P P P P P P    . . . . . . . .
  // . . . . . . . .    . . . . . . . .
  moves = board.generateMoves();
  CHECK(moves.size() == 16);

  //   Test Board       W Pawn Targets
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . 1 . . . . . .
  // . . p . . . . .    . 1 1 . . . . .
  // . P . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .
  board.reset("8/8/8/8/8/2p5/1P6/8 w - - 0 1");
  moves = board.generateMoves();
  CHECK(moves.size() == 3);

  //   Test Board       B Pawn Targets
  // . . . . . . . .    . . . . . . . .
  // . P . . . . . .    . . . . . . . .
  // . . p . . . . .    . 1 1 . . . . .
  // . . . . . . . .    . 1 . . . . . .
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .
  board.reset("8/1p6/2P5/8/8/8/8/8 b - - 0 1");
  moves = board.generateMoves();
  CHECK(moves.size() == 3);

  board.reset("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
  moves = board.generateMoves();
  CHECK(moves.size() == 48);
  std::string_view fen{ };
  size_t expectedNumMoves{ 0 };

  board.reset(STANDARD_STARTING_FEN);
  moves = board.generateMoves();
  CHECK(moves.size() == 20);

  // r n b q k b n r
  // p p p . . p p p
  // . . . p . . . .
  // . . . . p . . .
  // . . . . P . . .
  // . . . . . N . .
  // P P P P . P P P
  // R N B Q K B . R
  board.reset("rnbqkbnr/ppp2ppp/3p4/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 0 1");
  moves = board.generateMoves();
  CHECK(moves.size() == 27);

  // r n b q k b n r
  // p p p . . . p p
  // . . . p . p . .
  // . . . . p . . .
  // . . . P P . . .
  // . . . . . N . .
  // P P P . . P P P
  // R N B Q K B . R
  board.reset("rnbqkbnr/ppp3pp/3p1p2/4p3/3PP3/5N2/PPP2PPP/RNBQKB1R w KQkq - 0 1");
  moves = board.generateMoves();
  CHECK(moves.size() == 36);

  // r n b q k b n r
  // p p . . . . p p
  // . . p p . p . .
  // . B . . p . . .
  // . . . P P . . .
  // . . . . . N . .
  // P P P . . P P P
  // R N B Q K . . R
  board.reset("rnbqkbnr/pp4pp/2pp1p2/1B2p3/3PP3/5N2/PPP2PPP/RNBQK2R w KQkq - 0 1");
  moves = board.generateMoves();
  CHECK(moves.size() == 41);

  // . . . . . . . .
  // . . . . . . . .
  // . . . . r . . .
  // . . . . . . . .
  // . . . . . . . .
  // . . . . . . . .
  // . . . . . . . .
  // . . . . K . . R
  board.reset("8/8/4r3/8/8/8/8/4K2R w K - 0 1");
  moves = board.generateMoves();
  // Cannot castle out of check
  CHECK(findFirstCastlingMove(moves.cbegin(), moves.cend()) == moves.cend());

  // r n b q k b n r
  // p p p . . p p p
  // . . . p . . . .
  // . B . . p . . .
  // . . . . P . . .
  // . . . . . N . .
  // P P P P . P P P
  // R N B Q K . . R
  board.reset("rnbqkbnr/ppp2ppp/3p4/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1");
  moves = board.generateMoves();
  // The only legal moves are the six that remove the check: c6, Nc6, Nd7, Bd7, Qc7, Ke7
  CHECK(moves.size() == 6);

  // r n b q k b n r
  // p p p . . p p p
  // . . . p . . . .
  // . B . . p . . .
  // . . . . P . . .
  // . . . . . N . .
  // P P P P . P P P
  // R N B Q K . . R
  board.reset("rnbqkbnr/ppp2ppp/3p4/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1");
  CHECK_FALSE(board.isCheckmate());

  // r . . q . b k r
  // p p p . . . p p
  // . . n . B . . .
  // . . . . p . . .
  // . . . . . . . .
  // . . . . . Q . .
  // P P P P . P P P
  // R N B   K . . R
  board.reset("r2q1bkr/ppp3pp/2n1B3/4p3/8/5Q2/PPPP1PPP/RNB1K2R b KQkq - 0 1");
  CHECK(board.isCheckmate());

  board.reset("rnbqkbnr/ppp2ppp/3p4/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1");
  CHECK(board.isCheck());

  board.reset("rnbqkbnr/pp3ppp/2pp4/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1");
  CHECK_FALSE(board.isCheck());
}

TEST_CASE("Move making")
{
  std::vector<Move> moves;
  std::vector<Move>::const_iterator move_p;
  Board board{ "8/8/8/8/Pp6/1P6/8/8 b - a3 0 1" };

  //  Before Capture     After Capture
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .
  // P p . . . . . .    . . . . . . . .
  // . P . . . . . .    p P . . . . . .
  // . . . . . . . .    . . . . . . . .
  // . . . . . . . .    . . . . . . . .

  moves = board.generateMoves();
  move_p = findFirstEpMove(moves.cbegin(), moves.cend());
  REQUIRE(move_p != moves.cend());
  CHECK(board.makeMove(*move_p) == Board::MoveResult::SUCCESS);
  CHECK(board.toFen() == "8/8/8/8/8/pP6/8/8 w - - 0 2");
  board.undoMove();
  CHECK(board.toFen() == "8/8/8/8/Pp6/1P6/8/8 b - a3 0 1");

  // White king side castle
  board.reset("8/8/8/8/8/8/8/4K2R w K - 0 1");
  moves = board.generateMoves();
  move_p = findFirstCastlingMove(moves.cbegin(), moves.cend());
  REQUIRE(move_p != moves.cend());
  CHECK(board.makeMove(*move_p) == Board::MoveResult::SUCCESS);
  CHECK(board.toFen() == "8/8/8/8/8/8/8/5RK1 b - - 0 1");
  board.undoMove();
  CHECK(board.toFen() == "8/8/8/8/8/8/8/4K2R w K - 0 1");

  board.reset("rnbqkbnr/ppp2ppp/3p4/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1");
  moves = board.generateMoves();
  move_p = findFirstCastlingMove(moves.cbegin(), moves.cend());
  REQUIRE(move_p != moves.cend());
  CHECK(board.makeMove(*move_p) == Board::MoveResult::SUCCESS);
  CHECK(board.toFen() == "rnbqkbnr/ppp2ppp/3p4/4p3/2B1P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 0 1");
  board.undoMove();
  CHECK(board.toFen() == "rnbqkbnr/ppp2ppp/3p4/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1");

  // Black king side castle
  board.reset("rnbqk2r/ppp2pbp/3p1np1/3Pp3/2B1P3/5N2/PPP2PPP/RNBQ1RK1 b kq - 0 1");
  moves = board.generateMoves();
  move_p = findFirstCastlingMove(moves.cbegin(), moves.cend());
  REQUIRE(move_p != moves.cend());
  CHECK(board.makeMove(*move_p) == Board::MoveResult::SUCCESS);
  CHECK(board.toFen() == "rnbq1rk1/ppp2pbp/3p1np1/3Pp3/2B1P3/5N2/PPP2PPP/RNBQ1RK1 w - - 0 2");
  board.undoMove();
  CHECK(board.toFen() == "rnbqk2r/ppp2pbp/3p1np1/3Pp3/2B1P3/5N2/PPP2PPP/RNBQ1RK1 b kq - 0 1");

  // White queen side castle
  board.reset("rnbqk2r/ppp2pbp/3p1np1/3Pp3/2B1P3/2N1B3/PPPQ1PPP/R3K1NR w KQkq - 0 1");
  moves = board.generateMoves();
  move_p = findFirstCastlingMove(moves.cbegin(), moves.cend());
  REQUIRE(move_p != moves.cend());
  CHECK(board.makeMove(*move_p) == Board::MoveResult::SUCCESS);
  CHECK(board.toFen() == "rnbqk2r/ppp2pbp/3p1np1/3Pp3/2B1P3/2N1B3/PPPQ1PPP/2KR2NR b kq - 0 1");
  board.undoMove();
  CHECK(board.toFen() == "rnbqk2r/ppp2pbp/3p1np1/3Pp3/2B1P3/2N1B3/PPPQ1PPP/R3K1NR w KQkq - 0 1");

  // Black queen side castle
  board.reset("r3kbnr/ppp2ppp/3pb3/4p1q1/4Pn2/3P4/PPP2PPP/RNBQKBNR b KQkq - 0 1");
  moves = board.generateMoves();
  move_p = findFirstCastlingMove(moves.cbegin(), moves.cend());
  REQUIRE(move_p != moves.cend());
  CHECK(board.makeMove(*move_p) == Board::MoveResult::SUCCESS);
  CHECK(board.toFen() == "2kr1bnr/ppp2ppp/3pb3/4p1q1/4Pn2/3P4/PPP2PPP/RNBQKBNR w KQ - 0 2");
  board.undoMove();
  CHECK(board.toFen() == "r3kbnr/ppp2ppp/3pb3/4p1q1/4Pn2/3P4/PPP2PPP/RNBQKBNR b KQkq - 0 1");

  // Pawn promotions
  board.reset("8/P7/8/8/8/8/8/8 w - - 0 1");
  moves = board.generateMoves();

  board.makeMove(moves[0]);
  CHECK(board.toFen() == "N7/8/8/8/8/8/8/8 b - - 0 1");
  board.undoMove();
  CHECK(board.toFen() == "8/P7/8/8/8/8/8/8 w - - 0 1");

  board.makeMove(moves[1]);
  CHECK(board.toFen() == "B7/8/8/8/8/8/8/8 b - - 0 1");
  board.undoMove();
  CHECK(board.toFen() == "8/P7/8/8/8/8/8/8 w - - 0 1");

  board.makeMove(moves[2]);
  CHECK(board.toFen() == "R7/8/8/8/8/8/8/8 b - - 0 1");
  board.undoMove();
  CHECK(board.toFen() == "8/P7/8/8/8/8/8/8 w - - 0 1");

  board.makeMove(moves[3]);
  CHECK(board.toFen() == "Q7/8/8/8/8/8/8/8 b - - 0 1");
  board.undoMove();
  CHECK(board.toFen() == "8/P7/8/8/8/8/8/8 w - - 0 1");
}

} // namespace yak
