# YakChessCpp

A chess move generator written in C++.

This project requires C++20. You can use the provided Dockerfile to build a container with all necessary dependencies.

## Build instructions

From the project root directory, run:

```
cmake -B build
cd build
make -jN
```

Where `N` is the number of parallel build jobs.

Tests can be run either with `make test` or `ctest`. Most of the tests are very
fast, but PerftTests can take a long time to complete (~300 seconds on my
machine).

## Running PerftExt
`PerftExt` is the primary executable available for testing move generation.

After building this can be found in `build/src/perft`.

Running it with a specific position (FEN) and depth will print the number of legal moves possible up to the given depth.

```
> ./build/src/perft/PerftExt 6 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
Provided FEN: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1

 a2a3 4463267
 b2b3 5310358
 c2c3 5417640
 d2d3 8073082
 e2e3 9726018
 f2f3 4404141
 g2g3 5346260
 h2h3 4463070
 a2a4 5363555
 b2b4 5293555
 c2c4 5866666
 d2d4 8879566
 e2e4 9771632
 f2f4 4890429
 g2g4 5239875
 h2h4 5385554
 b1a3 4856835
 b1c3 5708064
 g1f3 5723523
 g1h3 4877234

 Total moves: 119060324
```

## Future Development

### Move Generation Optimisation

Currently, the move generator follows these steps:

1. Generate pseudo-legal moves (moves that follow chess rules but may leave the king in check).
2. Verify legality by making the move and checking if the moving side is in check.

This extra verification step introduces unnecessary overhead and is currently the main performance bottleneck. Future optimizations will focus on generating only legal moves directly to eliminate redundant legality checks.
