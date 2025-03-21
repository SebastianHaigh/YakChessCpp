#include "GameState.h"

#include <types.h>
#include <move.hpp>

namespace yak {

namespace {
  static constexpr Square BLACK_KING_SQUARE{E8};
  static constexpr Square BLACK_KINGS_ROOK{H8};
  static constexpr Square BLACK_QUEENS_ROOK{A8};

  static constexpr Square WHITE_KING_SQUARE{E1};
  static constexpr Square WHITE_KINGS_ROOK{H1};
  static constexpr Square WHITE_QUEENS_ROOK{A1};
}

GameStateManager::GameStateManager()
  : m_currentState(new GameState{})
{
}

GameStateManager::~GameStateManager()
{
  GameState* state = m_currentState;
  while (state)
  {
    // first grab the next most prevState from the current prev state
    GameState* newPrevState = state->m_prevState;

    // free the memory of the prevState
    delete state;

    state = newPrevState;
  }
}

void GameStateManager::update(const Move& move)
{
  m_currentState = m_currentState->update(move);
}

const Move* const GameStateManager::pop()
{
  if (not m_currentState->m_prevState) return nullptr;

  GameState* state = m_currentState;

  m_currentState = m_currentState->m_prevState;

  delete state;

  return &m_currentState->m_move;
}

bool GameStateManager::loadFen(std::string_view fen)
{
  // Remove all current states
  GameState* state = m_currentState;
  while (state)
  {
    // first grab the next most prevState from the current prev state
    GameState* newPrevState = state->m_prevState;

    // free the memory of the prevState
    delete state;

    state = newPrevState;
  }

  // TODO (haigh) Stop creating new game states, and allocating memory, the manager can allocate
  // a large block of memory aligned to 64 byte boundaries when it starts up. Then, when we want a
  // new game state we can get a 64 byte chunk of preallocated memory from the manager, and when we
  // want to delete a state we can hand it back to the manager.
  //
  // The when a new state is requested, the manager can hand out the most resently released block
  // which is more likely to still be cached in L1.
  m_currentState = new GameState{};

  auto startOfCastlingRights = fen.find_first_of(" ");
  auto startOfEpTarget = fen.find_first_of(" ", startOfCastlingRights + 1);
  auto startOfMoveNumber = fen.find_first_of(" ", startOfEpTarget + 1);
  if (fen[0] == 'w')
  {
    m_currentState->m_side = true;
  }
  else if (fen[0] == 'b')
  {
    m_currentState->m_side = false;
  }
  else
  {
    return false;
  }

  if (fen[startOfEpTarget + 1] != '-')
  {
    m_currentState->m_epSquare = squareIndex(fen.substr(startOfEpTarget + 1, 2));
  }

  m_currentState->m_castlingRights[0] = false;
  m_currentState->m_castlingRights[1] = false;
  m_currentState->m_castlingRights[2] = false;
  m_currentState->m_castlingRights[3] = false;

  int lengthCastlingRights = startOfEpTarget - startOfCastlingRights;
  auto castling = fen.substr(startOfCastlingRights + 1, lengthCastlingRights - 1);

  if (castling.empty() || (castling.size() == 1 && castling[0] == '-'))
  {
    return true;
  }

  for (const auto &character : castling)
  {
    switch (character)
    {
      case 'k':
      {
        m_currentState->m_castlingRights[0] = true;
        break;
      }
      case 'q':
      {
        m_currentState->m_castlingRights[1] = true;
        break;
      }
      case 'K':
      {
        m_currentState->m_castlingRights[2] = true;
        break;
      }
      case 'Q':
      {
        m_currentState->m_castlingRights[3] = true;
        break;
      }
      default:
      {
        return false;
      }
    }
  }

  return true;
}

GameState::GameState()
{
}

GameState::~GameState()
{
}

std::string GameState::toFen() const
{
  std::string fen = "";

  if (m_side)
  {
    fen += 'w';
  }
  else
  {
    fen += 'b';
  }

  fen += " ";
  if (not m_castlingRights[0] && not m_castlingRights[1] && not m_castlingRights[2] && not m_castlingRights[3])
  {
    fen += "-";
  }
  else
  {
    fen += m_castlingRights[2] ? "K" : "";
    fen += m_castlingRights[3] ? "Q" : "";
    fen += m_castlingRights[0] ? "k" : "";
    fen += m_castlingRights[1] ? "q" : "";
  }
  fen += " ";

  fen += (m_epSquare == NULL_SQUARE) ? "-" : toAlgebraic(m_epSquare);
  fen += " 0 ";  // TODO (haigh) implement the half move clock
  fen += std::to_string(m_moveClock);

  return fen;
}

void GameState::toggleSideToMove()
{
  m_side = not m_side;
}

GameState* GameState::update(const Move &move)
{
  // Save the move in the current state
  m_move = move;

  GameState* newState = new GameState{};
  newState->m_prevState = this;
  newState->m_side = not m_side;

  // Update the castling rights in the new state
  bool castle = isCastle(move);
  bool isWhiteCastle = castle && m_side;
  bool isBlackCastle = castle && not m_side;

  Square fromSquare = from(move);
  Square toSquare = to(move);

  bool removesWhiteK{ false }, removesWhiteQ{ false }, removesBlackK{ false }, removesBlackQ{ false };

  // If this is a castle move, the from and to squares are set to A1, which is a white rook starting square,
  // only consider the from and to squares if this move is not a castling move
  if (not castle)
  {
    removesWhiteK = (fromSquare == WHITE_KING_SQUARE || fromSquare == WHITE_KINGS_ROOK || toSquare == WHITE_KINGS_ROOK);
    removesWhiteQ = (fromSquare == WHITE_KING_SQUARE || fromSquare == WHITE_QUEENS_ROOK || toSquare == WHITE_QUEENS_ROOK);
    removesBlackK = (fromSquare == BLACK_KING_SQUARE || fromSquare == BLACK_KINGS_ROOK || toSquare == BLACK_KINGS_ROOK);
    removesBlackQ = (fromSquare == BLACK_KING_SQUARE || fromSquare == BLACK_QUEENS_ROOK || toSquare == BLACK_QUEENS_ROOK);
  }

  newState->m_castlingRights[0] = not isBlackCastle && not removesBlackK && m_castlingRights[0];
  newState->m_castlingRights[1] = not isBlackCastle && not removesBlackQ && m_castlingRights[1];
  newState->m_castlingRights[2] = not isWhiteCastle && not removesWhiteK && m_castlingRights[2];
  newState->m_castlingRights[3] = not isWhiteCastle && not removesWhiteQ && m_castlingRights[3];

  // Update the ep square
  int possibleTargets[4] = {NULL_SQUARE, toSquare + 8, NULL_SQUARE, toSquare - 8};

  // To find the ep target square we need to find the square behind the pawn that just moved.
  // If the side that is currently moving is black (m_side = false) then we need to add 8 to the "to" square
  // If the side that is currently moving is white (m_side = true) then we need to subtract 8 from the "to" square
  // If the move is not a double push (double_push = false) then the ep square should be NULL_SQUARE
  //
  // m_side * 2 will either be 0 or 2
  // double push will either be 0 or 1
  newState->m_epSquare = static_cast<Square>(possibleTargets[static_cast<int>(m_side) * 2 + isDoublePush(move)]);

  // Update the move clocks
  newState->m_moveClock = (not m_side) ? m_moveClock + 1 : m_moveClock;
  newState->m_halfMoveClock = (isCapture(move) || isPawnMove(move)) ? 0 : m_halfMoveClock + 1;

  return newState;
}

} // namespace yak
