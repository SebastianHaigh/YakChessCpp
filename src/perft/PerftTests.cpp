#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <board.h>
#include <perft/Perft.h>

#include <memory>

#define CATCH_CONFIG_ENABLE_BENCHMARKING

namespace yak {

TEST_CASE("Perft Test")
{
  PerftResult result;

  Board board{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };

  SECTION("One Move")
  {
    result = perft(board, 1);
    CHECK(result.m_total == 20);
    CHECK(result.m_captures == 0);
  };

  SECTION("Two Moves")
  {
    result = perft(board, 2);
    CHECK(result.m_total == 400);
    CHECK(result.m_captures == 0);
  };

  SECTION("Three Moves")
  {
    result = perft(board, 3);
    CHECK(result.m_total == 8902);
    CHECK(result.m_captures == 34);
  };

  SECTION("Four Moves")
  {
    result = perft(board, 4);
    CHECK(result.m_total == 197281);
    CHECK(result.m_captures == 1576);
  };

  SECTION("Five Moves")
  {
    result = perft(board, 5);
    CHECK(result.m_total == 4865609);
  };

  SECTION("Six Moves")
  {
    result = perft(board, 6);
    CHECK(result.m_total == 119060324);
  };

  SECTION("Seven Moves")
  {
    result = perft(board, 7);
    CHECK(result.m_total == 3195901860);
  };

  // Currently takes too long to run
  //
  /* SECTION("Eight Moves") */
  /* { */
  /*   result = perft(board, 8); */
  /*   CHECK(result.m_total == 84998978956); */
  /* }; */

}

TEST_CASE("Perft Test", "[benchmark]")
{
  PerftResult result;

  Board board{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };

  BENCHMARK("One Move")
  {
    result = perft(board, 1);
    CHECK(result.m_total == 20);
    CHECK(result.m_captures == 0);
  };

  BENCHMARK("Two Moves")
  {
    result = perft(board, 2);
    CHECK(result.m_total == 400);
    CHECK(result.m_captures == 0);
  };

  BENCHMARK("Three Moves")
  {
    result = perft(board, 3);
    CHECK(result.m_total == 8902);
    CHECK(result.m_captures == 34);
  };

  BENCHMARK("Four Moves")
  {
    result = perft(board, 4);
    CHECK(result.m_total == 197281);
    CHECK(result.m_captures == 1576);
  };

  BENCHMARK("Five Moves")
  {
    result = perft(board, 5);
    CHECK(result.m_total == 4865609);
  };

  BENCHMARK("Six Moves")
  {
    result = perft(board, 6);
    CHECK(result.m_total == 119060324);
  };

  BENCHMARK("Seven Moves")
  {
    result = perft(board, 7);
    CHECK(result.m_total == 3195901860);
  };
}

} // namespace yak
