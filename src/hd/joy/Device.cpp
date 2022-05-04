#include "hd/joy/Device.hpp"

namespace hd::joy {
    Sint16 Device::GetAxis(int aAxis){

    }
    bool Device::IsAttached(){
      return SDL_JoystickGetAttached (m_Joystick) == SDL_TRUE;
    }

  Device::Ptr
  Device::FromInstanceId (SDL_JoystickID aId)
  {
    SDL_Joystick *found = SDL_JoystickFromInstanceID (aId);
    Device::Ptr device = NULL;
    if (m_OpenedByPtr.find (found) != m_OpenedByPtr.end ()) {
      device = m_openedByPtr.at (found).lock();
    }
    if(device == NULL){
      hdError ("This joystick is not managed by hd.");
    }
    return device;
  }
  Device::~Device ()
  {
    m_Opened.erase (m_Index);
    m_OpenedByPtr.erase (m_Joystick);
    SDL_JoystickClose (m_Joystick);
    hdDebugCall(NULL);
  }
  Device::Device (int aIndex, SDL_Joystick *aDevice)
      : m_Index (aIndex), m_Joystick (aDevice)
  {
    hdDebugCall (NULL);
  }
  int
  Device::Count ()
  {
    return SDL_NumJoysticks ();
  }

  Device::Ptr
  Device::Open (int aIndex)
  {
    Device::Ptr device = NULL;
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
    device = Device::Ptr (new Device (jsDevice));
    m_Opened[aIndex] = device;
    m_OpenedByPtr[jsDevice] = device;
    return device;
  }

  std::map<int, std::weak_ptr<Device> > Device::m_Opened;
  std::map<SDL_Joystick *, std::weak_ptr<Device> > Device::m_OpenedByPtr;
}