#include <gtest/gtest.h>

#include "../movegen.h"
#include "../pieces.h"
#include "../board.h"
#include "../bitboard.h"

#include <iostream>

TEST(MoveGenTest, CanCreateObject) {
    // Arrange
    auto board = std::make_shared<Board>();

    auto move_generator = move_generation::MoveGenerator(board);
    
    // Act
    auto move_stack = move_generator.generate_pseudo_legal_moves(*board);

    // Assert
    EXPECT_EQ(move_stack.size(), 0);
}
TEST(MoveTest, DoublePawnPushCreatesEpTargetSquare) {
    // Arrange
    std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
    std::string expected{ "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3 0 1" };
    Board board(fen);
    Move move = Move(8, 24, PieceType::PAWN);
    move.set_double_push();

    // Act
    board.make_move(move);

    // Assert
    EXPECT_EQ(board.to_fen(), expected);

}

TEST(MoveTest, CanMakeEpCapture) {
    // Arrange
    std::string fen{ "8/8/8/8/Pp6/1P6/8/8 b KQkq a3 0 1" };
    std::string expected{ "8/8/8/8/8/pP6/8/8 w KQkq - 0 2" };
    auto board = Board(fen);
    Move move(Square(25), Square(16), PieceType::PAWN);
    move.set_en_passant(bitboard::to_bitboard(Square(24)));

    // Act
    board.make_move(move);


    // Assert
    EXPECT_EQ(board.to_fen(), expected);
}


TEST(MoveTest, FirstMoveOfTheGameHas20LegalMoves) {
    // Arrange
    std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
    auto board = std::make_shared<Board>(fen);
    auto move_generator = move_generation::MoveGenerator(board);

    // Act
    auto move_stack = move_generator.generate_pseudo_legal_moves(*board);

    // Assert
    EXPECT_EQ(move_stack.size(), 20);
}

TEST(MoveGenTest, ThisPositionHasSevenMoves) {
    // Arrange
    std::string fen{ "8/7P/8/8/8/8/8/k6K w - - 0 1" };
    auto board = std::make_shared<Board>(fen);
    auto move_generator = move_generation::MoveGenerator(board);

    // Act
    auto move_stack = move_generator.generate_pseudo_legal_moves(*board);
    std::string move_list = "";
    for (auto move : move_stack) {
        move_list += move.to_algebraic() + ", ";
    }
    // Assert
    EXPECT_EQ(move_stack.size(), 7);
}

TEST(MoveTest, CanConvertPawnMoveToAlgebraic) {
    // Arrange
    Move move(Square(25), Square(16), PieceType::PAWN);
    std::string expected = "a3";

    // Act
    std::string actual = move.to_algebraic();

    // Assert
    EXPECT_EQ(actual, expected);
}

TEST(MoveFactoryTest, CanCreatePawnPushMove) {
    // Arrange
    MoveFactory mf = MoveFactory();
    std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
    std::string expected{ "rnbqkbnr/pppppppp/8/8/8/P7/1PPPPPPP/RNBQKBNR b KQkq - 0 1" };
    Board board = Board(fen);

    // Act
    Move pawn_push = mf.create_pawn_push(8, 16);

    // Assert
    board.make_move(pawn_push);
    EXPECT_EQ(board.to_fen(), expected);
}

TEST(MoveFactoryTest, CanCreatePawnDoublePushMove) {
    // Arrange
    MoveFactory mf = MoveFactory();
    std::string fen{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
    std::string expected{ "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3 0 1" };
    Board board = Board(fen);

    // Act
    Move pawn_push = mf.create_pawn_push(8, 24);

    // Assert
    board.make_move(pawn_push);
    EXPECT_EQ(board.to_fen(), expected);
}