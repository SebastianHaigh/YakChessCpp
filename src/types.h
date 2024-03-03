#ifndef YAK_TYPES_H_
#define YAK_TYPES_H_

#include <cstdint>
#include <string>
#include <string_view>

namespace yak {

using Bitboard = uint64_t;
using File = uint64_t;
using Rank = uint64_t;

enum Square {
  A1 = 0, B1, C1, D1, E1, F1, G1, H1,
  A2, B2, C2, D2, E2, F2, G2, H2,
  A3, B3, C3, D3, E3, F3, G3, H3,
  A4, B4, C4, D4, E4, F4, G4, H4,
  A5, B5, C5, D5, E5, F5, G5, H5,
  A6, B6, C6, D6, E6, F6, G6, H6,
  A7, B7, C7, D7, E7, F7, G7, H7,
  A8, B8, C8, D8, E8, F8, G8, H8,
  NULL_SQUARE
};

enum class RayType {
  POSITIVE,
  NEGATIVE
};

enum class Direction {
  NORTH,
  EAST,
  SOUTH,
  WEST,
  NORTH_EAST,
  NORTH_WEST,
  SOUTH_EAST,
  SOUTH_WEST
};

enum class PieceType {
  PAWN = 0,
  KNIGHT,
  BISHOP,
  ROOK,
  QUEEN,
  KING,
  NULL_PIECE
};

template<typename T>
concept Type = std::is_same_v<T, PieceType>;

template<PieceType T>
concept SimplySlideable = (T == PieceType::BISHOP) || (T == PieceType::ROOK);

template<PieceType T>
concept CompoundSlideable = (T == PieceType::QUEEN);

template<PieceType T>
concept Slideable = SimplySlideable<T> || CompoundSlideable<T>;

template<PieceType T>
concept Jumpable = (T == PieceType::KNIGHT) || (T == PieceType::KING);

enum class PieceColour {
  BLACK = 0,
  WHITE,
  NULL_COLOUR
};

File fileIndex(const Square& squareIndex);
File fileIndex(char algebraicFile);
File fileIndex(std::string_view algebraic_square);

Rank rankIndex(const Square& squareIndex);
Rank rankIndex(char algebraicRank);
Rank rankIndex(std::string_view algebraicSquare);

Square squareIndex(File fileIndex, Rank rankIndex);
Square squareIndex(std::string_view square);

std::string toAlgebraic(Square square);
std::string toAlgebraic(File file_index, Rank rank_index);


// | F  | R  | O  | M  | -- | -> | T  | O  |
// | -- | -- | -- | -> | F  | L  | A  | GS |
// | MO | VE | D  | -> | CA | PT | UR | ED |
// |    |    |    |    |    |    |    |    |

// TODO this needs to be compacted into a 32 bit int
// from square 6 bits
// to square 6 bits
// flags 4 bits
// captured piece 4 bits?
struct Move
{
  uint32_t fromAndTo{ 0 };
  bool capture = false;
  bool enPassant = false;
  bool doublePush = false;
  bool pawnMove = false;
  Square epTarget = NULL_SQUARE;
  PieceType castle = PieceType::NULL_PIECE;
  PieceType promotion = PieceType::NULL_PIECE;

  std::string toAlgebraic() const;
};

inline constexpr auto from(Move const& move) -> Square
{
  // The from square is encoded in the least significant 6 bits
  return static_cast<Square>(move.fromAndTo & 0b0011'1111);
}

inline constexpr void setFrom(Move& move, Square square)
{
  uint32_t x = move.fromAndTo & ~0b0011'1111; // Get the number without the from square
  move.fromAndTo = (x | (square & 0b0011'1111));
}

inline constexpr auto to(Move const& move) -> Square
{
  // The to square is encoded in the 6 bits after the from square
  return static_cast<Square>((move.fromAndTo >> 6) & 0b0011'1111);
}

inline constexpr void setTo(Move& move, Square square)
{
  uint32_t x = move.fromAndTo & ~0b1111'1100'0000; // Get the number without the to square
  move.fromAndTo = (x | ((square << 6) & 0b1111'1100'0000));
}

inline auto Move::toAlgebraic() const -> std::string
{
  return ::yak::toAlgebraic(from(*this)) + ::yak::toAlgebraic(to(*this));
}

inline constexpr auto pieceTypeToInt(PieceType type) -> uint8_t
{
  return (static_cast<uint8_t>(type) & 0x0F);
}

inline constexpr void setMoved(Move& move, PieceType type)
{
  uint32_t x = move.fromAndTo & ~0xF0000;
  move.fromAndTo = (x | (pieceTypeToInt(type) << 16) & 0x000F0000);
}

inline constexpr auto moved(Move const& move) -> PieceType
{
  return static_cast<PieceType>((move.fromAndTo >> 16) & 0x0F);
}

inline constexpr void setCaptured(Move& move, PieceType type)
{
  uint32_t x = move.fromAndTo & ~0xF00000;
  move.fromAndTo = (x | (pieceTypeToInt(type) << 20) & 0x00F00000);
}

inline constexpr auto captured(Move const& move) -> PieceType
{
  return static_cast<PieceType>((move.fromAndTo >> 20) & 0x0F);
}

} // namespace yak

#endif // YAK_TYPES_H_
