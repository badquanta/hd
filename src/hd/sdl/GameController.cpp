/**
 * @file Controller.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-05-05
 *
 * @copyright GNU-GPL 3.0
 *
 */
#include "hd/sdl/GameController.hpp"
#include "hd/Error.hpp"

namespace hd::sdl {
  bool GameController::IsGameController(int aIndex){
    return SDL_IsGameController (aIndex) == SDL_TRUE;
  }

  bool GameController::GetButtonPressed(SDL_GameControllerButton aBtn){
    return (SDL_GameControllerGetButton (*this, aBtn) == 1);
  }

  bool GameController::GetButtonReleased(SDL_GameControllerButton aBtn){
    return (!GetButtonPressed(aBtn));
  }

  SDL_GameControllerButtonBind
  GameController::GetBindForButton (SDL_GameControllerButton aButton)
  {
    assert (*this);
    return SDL_GameControllerGetBindForButton (*this, aButton);
  }
  /**
   * @brief Get the Bind For Axis object
   *
   * @param aAxis
   * @return SDL_GameControllerButtonBind
   */
  SDL_GameControllerButtonBind
  GameController::GetBindForAxis (SDL_GameControllerAxis aAxis)
  {
    assert (*this);
    return SDL_GameControllerGetBindForAxis (*this, aAxis);
  }
  /**
   * @brief
   *
   * @param aAxis
   * @return Sint16
   */
  Sint16
  GameController::GetAxis (SDL_GameControllerAxis aAxis)
  {
    assert (*this);
    return SDL_GameControllerGetAxis (*this, aAxis);
  }
  /**
   * @brief
   * @see https://wiki.libsdl.org/SDL_GameControllerGetAttached
   * @return true
   * @return false
   */
  bool
  GameController::IsAttached ()
  {
    assert (*this);
    return SDL_GameControllerGetAttached (*this);
  }
  /**
   * @brief calls SDL_GameControllerOpen and wraps the returned pointer into a shared_ptr GameController.
   *
   * @param aIndex index of the game controller to open.
   * @note if called with a previously opened GameController
   * @return Controller::s_ptr
   */
  GameController::s_ptr
  GameController::Open (int aIndex)
  {
    GameController::s_ptr controller;
    if (m_Opened.find (aIndex) != m_Opened.end ()) {
      controller = m_Opened.at (aIndex).lock ();
    }
    if (controller == NULL) {
      SDL_GameController *opened = SDL_GameControllerOpen (aIndex);
      if (opened == NULL) {
        hdError ("Failed to open GameController#%d because: %s",
                 aIndex,
                 SDL_GetError ());
        return NULL;
      }
      controller = s_ptr (opened, SDL_GameControllerClose);
      m_Opened[aIndex] = controller;
      m_OpenedByPtr[opened] = controller;
    }
    return controller;
  }
  /**
   * @brief Destroy the Game Controller:: Game Controller object
   * @note erases weak_ptr entries for index and pointer
   */
  GameController::~GameController ()
  {
    m_Opened.erase (m_Index);
    m_OpenedByPtr.erase (*this);
  }
}