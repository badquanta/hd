#include "hd/sdl/Joystick.hpp"

namespace hd::sdl {
  bool Joystick::EventState(int aState){
    hdDebugCall ("%d", aState);
    int state = SDL_JoystickEventState (aState);
    if(state<0){
      hdError("Unable to get/set Joystick event state because: %s",SDL_GetError());
    }
    return state == SDL_ENABLE;
  }
  bool Joystick::EventState(bool aState){
    int state = (aState ? SDL_ENABLE : SDL_IGNORE);
    return EventState (state);
  }
  void Joystick::Update(){
    SDL_JoystickUpdate ();
  }
  /**
   * @brief
   * @see https://wiki.libsdl.org/SDL_JoystickCurrentPowerLevel
   * @return SDL_JoystickPowerLevel
   */
SDL_JoystickPowerLevel Joystick::CurrentPowerLevel(){
  assert (m_Joystick);
  return SDL_JoystickCurrentPowerLevel (m_Joystick);
}
  int
  Joystick::NumHats ()
  {
    assert (m_Joystick);
    int result = SDL_JoystickNumHats (m_Joystick);
    if (result < 0) {
      hdError ("Unable to get the number of Hats for joystick#%d because: %s",
               m_Index,
               SDL_GetError ());
    }
    return result;
  }
  int
  Joystick::NumButtons ()
  {
    assert (m_Joystick);
    int result = SDL_JoystickNumButtons (m_Joystick);
    if (result < 0) {
      hdError ("Unable to get number of axes for joystick#%d because: %s",
               m_Index,
               SDL_GetError ());
    }
    return result;
  }
  int
  Joystick::NumBalls ()
  {
    assert (m_Joystick);
    int result = SDL_JoystickNumBalls (m_Joystick);
    if (result < 0) {
      hdError ("Unable to get number of balls for joystick#%d because: %s",
               m_Index,
               SDL_GetError ());
    }
    return result;
  }
  int
  Joystick::NumAxes ()
  {
    assert (m_Joystick);
    int result = SDL_JoystickNumAxes (m_Joystick);
    if (result < 0) {
      hdError ("Unable to get number of axes for joystick#%d because: %s",
               m_Index,
               SDL_GetError ());

    }
    return result;
  }
  /**
   * @brief
   * @see https://wiki.libsdl.org/SDL_JoystickNameForIndex
   * @param aIndex
   * @return std::string
   */
  std::string
  Joystick::NameForIndex (int aIndex)
  {
    const char *name_cstr = SDL_JoystickNameForIndex (aIndex);
    if (name_cstr == NULL) {
      hdError ("Unable to get name for device index %d because: %s",
               aIndex,
               SDL_GetError ());
      return "";
    }
    std::string name_str = name_cstr;
    return name_str;
  }
  /**
   * @brief
   * @see https://wiki.libsdl.org/SDL_JoystickInstanceID
   * @return const char*
   */
  std::string
  Joystick::Name ()
  {
    assert (m_Joystick);
    const char *name_cstr = SDL_JoystickName (m_Joystick);
    if (name_cstr == NULL) {
      hdError ("Unable to get JoystickName because %s", SDL_GetError ());
      return "";
    }
    std::string name_str = name_cstr;
    return name_str;
  }
  /**
   * @brief
   * @see https://wiki.libsdl.org/SDL_JoystickInstanceID
   * @return SDL_JoystickID
   */
  SDL_JoystickID
  Joystick::InstanceId ()
  {
    assert (m_Joystick);
    return SDL_JoystickInstanceID (m_Joystick);
  }
  /**
   * @brief
   * @see https://wiki.libsdl.org/SDL_JoystickGetHat
   * @param aHatIndex
   * @return Uint8
   */
  Uint8
  Joystick::GetHat (int aHatIndex)
  {
    assert (m_Joystick);
    return SDL_JoystickGetHat (m_Joystick, aHatIndex);
  }
  /**
   * @brief
   * @see https://wiki.libsdl.org/SDL_JoystickGetGUIDString
   * @param aGuid
   * @return std::string
   */
  std::string
  Joystick::GetGuidString (SDL_JoystickGUID aGuid)
  {
    char guid_cstr[66] = { 0 };
    SDL_JoystickGetGUIDString (aGuid, guid_cstr, sizeof (guid_cstr));
    std::string guid_str = guid_cstr;
    return guid_str;
  }
  /**
   * @brief
   * @see https://wiki.libsdl.org/SDL_JoystickGetGUIDFromString
   * @param aStr
   * @return SDL_JoystickGUID
   */
  SDL_JoystickGUID
  Joystick::GetGuidFromString (std::string aStr)
  {
    return SDL_JoystickGetGUIDFromString (aStr.c_str ());
  }
  SDL_JoystickGUID
  Joystick::GetGUID ()
  {
    assert (m_Joystick);
    return SDL_JoystickGetGUID (m_Joystick);
  }
  SDL_JoystickGUID
  Joystick::GetDeviceGUID (int aDeviceIndex)
  {
    return SDL_JoystickGetDeviceGUID (aDeviceIndex);
  }
  bool
  Joystick::GetButtonPressed (int aButton)
  {
    assert (m_Joystick);
    return SDL_JoystickGetButton (m_Joystick, aButton) == 1;
  }
  bool
  Joystick::GetButtonReleased (int aButton)
  {
    assert (m_Joystick);
    return !GetButtonPressed (aButton);
  }
  bool
  Joystick::GetBall (int aBall, int *dx, int *dy)
  {
    assert (m_Joystick);
    if (SDL_JoystickGetBall (m_Joystick, aBall, dx, dy) != 0) {
      hdError ("Failed to read ball#%d for joystick#%d because: %s",
               aBall,
               m_Index,
               SDL_GetError ());
      return false;
    }
    return true;
  }

  /** @see https://wiki.libsdl.org/SDL_JoystickGetAxis
   * @todo docs says it returns 0 on failure; but it doesn't say that it is
   * impossible for it to return 0 if the joystick is perfectly centered on
   * that axis.
   * **/
  Sint16
  Joystick::GetAxis (int aAxis)
  {
    assert (m_Joystick);
    return SDL_JoystickGetAxis (m_Joystick, aAxis);
  }

  bool
  Joystick::IsAttached ()
  {
    assert (m_Joystick);
    return SDL_JoystickGetAttached (m_Joystick) == SDL_TRUE;
  }

  Joystick::s_ptr
  Joystick::FromInstanceId (SDL_JoystickID aId)
  {
    SDL_Joystick *found = SDL_JoystickFromInstanceID (aId);
    Joystick::s_ptr device = NULL;
    if (m_OpenedByPtr.find (found) != m_OpenedByPtr.end ()) {
      device = m_OpenedByPtr.at (found).lock ();
    }
    if (device == NULL) {
      hdError ("This joystick is not managed by hd.");
    }
    return device;
  }
   /** @see https://wiki.libsdl.org/SDL_JoystickClose **/
  Joystick::~Joystick ()
  {
    m_Opened.erase (m_Index);
    m_OpenedByPtr.erase (m_Joystick);
    engine->input.Joysticks[InstanceId()].Delete(inputHandle);
    Free ();
  }
  void Joystick::Free(){
    hdDebugCall (NULL);
    if (m_Free) {
      SDL_JoystickClose (m_Joystick);
    }
  }
  Joystick::Joystick (int aIndex, SDL_Joystick *aDevice)
      : m_Index (aIndex), m_Joystick (aDevice) , EngineComponent (NULL, false)
  {
    inputHandle = engine->input.Joysticks[InstanceId()].On (input.pipe);
  }
  /**
   * @brief
   * @see https://wiki.libsdl.org/SDL_NumJoysticks
   * @return int
   */
  int
  Joystick::Count ()
  {
    return SDL_NumJoysticks ();
  }

  Joystick::s_ptr
  Joystick::Open (int aIndex)
  {
    Joystick::s_ptr device = NULL;
    if (m_Opened.find (aIndex) != m_Opened.end ()) {
      device = m_Opened.at (aIndex).lock ();
      if (device != NULL) {
        hdDebug ("CACHED aIndex=%d", aIndex);
        return device;
      }
    }
    SDL_Joystick *jsDevice = SDL_JoystickOpen (aIndex);
    if (!jsDevice) {
      hdError (
          "Unable to open joystick #%d because: %s", aIndex, SDL_GetError ());
      return NULL;
    }
    device = Joystick::s_ptr (new Joystick (aIndex, jsDevice));
    m_Opened[aIndex] = device;
    m_OpenedByPtr[jsDevice] = device;
    return device;
  }

  std::map<int, std::weak_ptr<Joystick> > Joystick::m_Opened;
  std::map<SDL_Joystick *, std::weak_ptr<Joystick> > Joystick::m_OpenedByPtr;
}