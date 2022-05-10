#pragma once
/*
 * holodeck - maybe it will be a game or a game engine
 *
 */

#include "hd/EngineComponent.hpp"
#include "hd/evt/ControllerDispatch.hpp"
#include <map>
namespace hd::sdl {
  /**
   * @brief Representation of an SDL component
   * @todo SDL_ControllerEventState
   */
  class GameController : public EngineComponent<GameController, void> {
  public: // Class static components
    /** SDL_GameControllerNameForIndex **/
    static const char* Name (int);
    /** SDL_IsGameController **/
    static bool IsGameController (int);
    /** SDL_GameControllerOpen **/
    static s_ptr Open (int);
    /** SDL_GameControllerAddMapping **/
    /** SDL_GameControllerAddMappingsFromFile **/
    /** SDL_GameControllerAddMappingsFromRW **/
    /** SDL_GameControllerAxis **/

    /** SDL_GameControllerButton **/
    /** SDL_GameControllerEventState **/
    /** SDL_GameControllerFromInstanceID **/
  public: // instance methods
    virtual void Free () override;
    /** SDL_GameControllerGetAttached **/
    bool IsAttached ();
    /** SDL_GameControllerGetAxis **/
    Sint16 GetAxis (SDL_GameControllerAxis);
    /** SDL_GameControllerGetAxisFromString **/

    /** SDL_GameControllerGetBindForAxis **/
    SDL_GameControllerButtonBind GetBindForAxis (SDL_GameControllerAxis);
    /** SDL_GameControllerGetBindForButton **/
    SDL_GameControllerButtonBind GetBindForButton (SDL_GameControllerButton);
    /** SDL_GameControllerGetButton **/
    bool GetButtonPressed (SDL_GameControllerButton);
    bool GetButtonReleased(SDL_GameControllerButton);
    /** SDL_GameControllerGetButtonFromString **/

    /** SDL_GameControllerGetJoystick **/
    /** SDL_GameControllerGetStringForAxis **/
    /** SDL_GameControllerGetStringForButton **/
    /** SDL_GameControllerMapping **/
    /** SDL_GameControllerMappingForGUID **/
    /** SDL_GameControllerName **/

    /** SDL_GameControllerUpdate **/


    /**
     *
     */

  public: // Input Events
    evt::ControllerDispatch input;
    evt::SDL_EventDispatch::Handle inputHandle;

  private: // static caches
    static std::map<int, std::weak_ptr<GameController> > m_Opened;
    static std::map<SDL_GameController *, std::weak_ptr<GameController> >
        m_OpenedByPtr;

  private: //
    SDL_GameController *m_Controller;
    int m_Index;

  private: // constructors
    GameController (int, SDL_GameController *);

  public: // destructors
    /** SDL_GameControllerClose **/
    ~GameController ();
  };
}