#pragma once
/*
 * holodeck - maybe it will be a game or a game engine
 *
 */

#include "hd/EngineComponent.hpp"
#include "hd/sdl/events.hpp"
#include "hd/Wrap.hpp"
#include <map>
namespace hd::sdl {
  /**
   * @brief Representation of an SDL component
   * @todo SDL_JoystickEventState
   */
  class Joystick : public WRAP_PTR<SDL_Joystick> {
  public: // Class static components
    using WRAP_PTR::WRAP_PTR;
    using WRAP_PTR::operator=;
    static bool EventState (int aState = SDL_QUERY);
    static bool EventState (bool);
    /** SDL_JoystickFromInstanceID **/
    static s_ptr FromInstanceId (SDL_JoystickID);
    /**SDL_JoystickGetGUIDFromString**/
    static SDL_JoystickGUID GetGuidFromString (std::string);
    /** SDL_JoystickGetGUIDString**/
    static std::string GetGuidString (SDL_JoystickGUID);
    /** SDL_JoystickGetDeviceGUID**/
    static SDL_JoystickGUID GetDeviceGUID (int aDeviceIndex);
    /**SDL_JoystickNameForIndex**/
    static std::string NameForIndex (int);
    /** SDL_JoystickOpen**/
    static s_ptr Open (int);
    /** SDL_JoystickUpdate **/
    static void Update ();
    /** @see SDL_NumJoysticks **/
    static int Count ();
  public: // Instance Methods

    /**SDL_JoystickGetAttached **/
    bool IsAttached ();
    /**SDL_JoystickGetAxis **/
    Sint16 GetAxis (int);
    /** SDL_JoystickGetBall**/
    bool GetBall (int aBall, int *dx, int *dy);
    /**SDL_JoystickGetButton**/
    bool GetButtonPressed (int aButton);
    bool GetButtonReleased (int aButton);
    /** SDL_JoystickGetGUID**/
    SDL_JoystickGUID GetGUID ();
    /**SDL_JoystickGetHat**/
    Uint8 GetHat (int);
    /** SDL_JoystickInstanceID**/
    SDL_JoystickID InstanceId ();
    /** SDL_JoystickName**/
    std::string Name ();
    /** SDL_JoystickNumAxes**/
    int NumAxes ();
    /** SDL_JoystickNumBalls**/
    int NumBalls ();
    /** SDL_JoystickNumButtons**/
    int NumButtons ();
    /** SDL_JoystickNumHats**/
    int NumHats ();
    /**SDL_JoystickPowerLevel
     * SDL_JoystickCurrentPowerLevel **/
    SDL_JoystickPowerLevel CurrentPowerLevel ();
  public: // Input Events
    sdl::JoyDispatch input;
    sdl::EventDispatch::Handle inputHandle;
  private: // static caches
    static std::map<int, std::weak_ptr<SDL_Joystick> > m_Opened;
    static std::map<SDL_Joystick *, std::weak_ptr<SDL_Joystick> > m_OpenedByPtr;
  private: //
    int m_Index;
  private: // constructors
    Joystick (int, SDL_Joystick *);
  public: // destructors
    ~Joystick ();

  };
}