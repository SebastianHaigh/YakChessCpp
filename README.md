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
fast, but PerftTests can take a long time to complete (~30 seconds on my
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

## Design Overview

YakChessCpp uses bitboards to represent the chessboard and efficiently generate legal moves. A total of 8 bitboards are used:

- 6 bitboards for each piece type (pawn, knight, bishop, rook, queen, king).
- 2 bitboards for each color (one for white pieces, one for black pieces).

Each bitboard is a 64-bit integer, where each bit corresponds to a square on the chessboard (starting from A1 at the least significant bit).

For example, at the start of the game, the pawn bitboard looks like this:

```
0 0 0 0 0 0 0 0
1 1 1 1 1 1 1 1
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
1 1 1 1 1 1 1 1
0 0 0 0 0 0 0 0
```

while the bitboard for black pieces is:

```
1 1 1 1 1 1 1 1
1 1 1 1 1 1 1 1
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
```

Pawn move generation is handled separately using bitwise shifts. For other pieces
YakChessCpp uses precomputed lookup tables to efficiently generate moves:

- Magic bitboards for sliding pieces (bishop, rook, queen).
- Jump maps for non-sliding pieces (knight, king).

Magic bitboards are a perfect hashing technique used to quickly determine moves for
sliding pieces. Instead of dynamically computing move rays, magic bitboards allow for fast lookup
from a precomputed table.

### Build-Time Magic Number Computation

Unlike jump maps, magic bitboards require precomputed "magic numbers" to efficiently index the
lookup table. These magic numbers are difficult to find via simple mathematical formulas, so they
are computed using brute force.

Static lookup table generation process:

- Magic numbers are computed at build time using a separate executable, MagicGenerator.
- This generator finds valid magic numbers through randomized brute force.
- The generated magic numbers are stored in a generated header file (GeneratedMagics.h), which contains two arrays of 64 magic numbers (one for rooks, one for bishops).
- The static lookup table is then computed at compile time using C++ template metaprogramming, reducing runtime computations.

## Future Development

### Move Generation Optimisation

Currently, the move generator follows these steps:

1. Generate pseudo-legal moves (moves that follow chess rules but may leave the king in check).
2. Verify legality by making the move and checking if the moving side is in check.

This extra verification step introduces unnecessary overhead and is currently the main performance bottleneck. Future optimizations will focus on generating only legal moves directly to eliminate redundant legality checks.
