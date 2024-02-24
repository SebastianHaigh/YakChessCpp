#include "catch2/benchmark/catch_benchmark.hpp"
#include "perft/Perft.h"
#include <catch2/catch_test_macros.hpp>
  #include <catch2/catch_all.hpp>

#define CATCH_CONFIG_ENABLE_BENCHMARKING

namespace yak {

TEST_CASE("Perft Test")
{
  PerftResult result;

  BENCHMARK("One Move")
  {
    result = perft(1);
    CHECK(result.m_total == 20);
  };

  BENCHMARK("Two Moves")
  {
    result = perft(2);
    CHECK(result.m_total == 400);
  };

  BENCHMARK("Three Moves")
  {
    result = perft(3);
    CHECK(result.m_total == 8902);
  };

  BENCHMARK("Four Moves")
  {
    result = perft(4);
    /* CHECK(result.m_total == 197281); */
  };

  /* BENCHMARK("Five Moves") */
  /* { */
  /*   result = perft(5); */
  /*   /1* CHECK(result.m_total == 197281); *1/ */
  /* }; */

/*   BENCHMARK("Six Moves") */
/*   { */
/*     result = perft(6); */
/*     /1* CHECK(result.m_total == 197281); *1/ */
/*   }; */

/*   BENCHMARK("Seven Moves") */
/*   { */
/*     result = perft(7); */
/*     /1* CHECK(result.m_total == 197281); *1/ */
/*   }; */

/*   BENCHMARK("Eight Moves") */
/*   { */
/*     result = perft(8); */
/*     /1* CHECK(result.m_total == 197281); *1/ */
/*   }; */

/*   BENCHMARK("Ten Moves") */
/*   { */
/*     result = perft(10); */
/*   }; */

}

} // namespace yak
