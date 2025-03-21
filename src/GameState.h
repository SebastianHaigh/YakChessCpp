#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include "bitboard.h"
#include "types.h"

namespace yak {

class GameState;
class GameStateManager
{
public:
  GameStateManager();
  ~GameStateManager();

  GameState* operator->()
  {
    return m_currentState;
  };

  const GameState* operator->() const
  {
    return m_currentState;
  };

  void update(const Move& move);
  const Move* const pop();

  bool loadFen(std::string_view fen);

private:
  GameState* m_currentState;
};

class GameState
{
friend GameStateManager;
public:
  GameState();
  explicit GameState(const std::string& fen);
  ~GameState();

  std::string toFen() const;

  /*!
   * \brief Update the game state based on a given move.
   * \param[in] move - The move to be made.
   */
  GameState* update(const Move &move);
  GameState* pop();

  inline GameState* getPrevState()
  {
    return m_prevState;
  }

  inline const GameState* getPrevState() const
  {
    return m_prevState;
  }

  /*!
   * \brief Check if a side can castle on the king m_side.
   * \param[in] colour - The colour of the side to check.
   * \return true if the m_side can castle on the king side, false otherwise.
   */
  inline bool canKingSideCastle(PieceColour colour) const
  {
    if (colour == PieceColour::NULL_COLOUR) return false;

    return m_castlingRights[static_cast<bool>(colour) * 2];
  }

  /*!
   * \brief Check if the current side to move can king side castle.
   * \return true if the side to move can king side castle, false otherwise.
   */
  inline bool canKingSideCastle() const
  {
    return m_castlingRights[m_side * 2];
  }

  /*!
   * \brief Check if a side can castle on the queen side.
   * \param[in] colour - The colour of the side to check.
   * \return true if the side can castle on the queen side, false otherwise.
   */
  inline bool canQueenSideCastle(PieceColour colour) const
  {
    if (colour == PieceColour::NULL_COLOUR) return false;

    return m_castlingRights[1 + (static_cast<bool>(colour) * 2)];
  }

  /*!
   * \brief Check if the current side to move can queen side castle.
   * \return true if the side to move can queen side castle, false otherwise.
   */
  inline bool canQueenSideCastle() const
  {
    return m_castlingRights[1 + (m_side * 2)];
  }

  template<PieceType T>
  bool can_castle() const
  {
    return (T == PieceType::KING) ? canKingSideCastle() :
           (T == PieceType::QUEEN) ? canQueenSideCastle() : false;
  }

  inline PieceColour sideToMove() const
  {
    return m_colours[m_side];
  }

  inline PieceColour sideNotToMove() const
  {
    return m_colours[not m_side];
  }

  inline Square epTargetSquare() const
  {
    return m_epSquare;
  }

  inline uint16_t moveClock() const
  {
    return m_moveClock;
  }

  inline uint8_t halfMoveClock() const
  {
    return m_halfMoveClock;
  }

  inline Bitboard epTarget() const
  {
    return bitboard::createBitboard(m_epSquare);
  }

private:
  void parseFen(const std::string& fen);
  void toggleSideToMove();

  // TODO make sure that this all fits into 64 bytes
  bool m_side{true};  // default side to move is white   // 1 byte
  bool m_castlingRights[4] = { true, true, true, true }; // 4 bytes
  uint16_t m_moveClock = 1;
  uint8_t m_halfMoveClock = 0;
  Square m_epSquare = NULL_SQUARE;

  GameState* m_prevState = nullptr;  // 8 bytes

  // The move that was applied in order to get to the next state
  Move m_move;
  PieceColour m_colours[2] = {PieceColour::BLACK, PieceColour::WHITE};
};

} // namespace yak
