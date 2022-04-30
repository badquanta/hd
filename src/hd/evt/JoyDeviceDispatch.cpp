#include "hd/evt/JoyDeviceDispatch.hpp"
/*
 * holodeck - maybe it will be a game or a game engine
 * Copyright (C) 2022 Jón Davíð Sawyer (badquanta@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/** **/
namespace hd::evt {
    void
  JoyDeviceDispatch::Trigger (const SDL_Event &e)
  {
    SDL_JoystickID joyId;
    switch (e.type) {
    case SDL_JOYAXISMOTION: // joystick axis motion
      joyId = e.jaxis.which;
      break;
    case SDL_JOYBALLMOTION: // joystick trackball motion
      joyId = e.jball.which;
      break;
    case SDL_JOYHATMOTION: // joystick hat position change
      joyId = e.jhat.which;
      break;
    case SDL_JOYBUTTONDOWN: // joystick button pressed
    case SDL_JOYBUTTONUP:   // joystick button released
      joyId = e.jbutton.which;
      break;
    case SDL_JOYDEVICEADDED: // joystick connected
      joyId = e.jdevice.which;
      Added.Trigger (e);
      break;
    case SDL_JOYDEVICEREMOVED: // joystick disconnected
      joyId = e.jdevice.which;
      Removed.Trigger (e);
      break;
    default:
      hdError ("Invalid Joystick Event type %d", e.type);
      return;
    }
    SDL_EventDispatch::Trigger (e);
    if (find (joyId) != end ()) {
      at (joyId).Trigger (e);
    } else {
      hdDebug ("No handlers for Joystick ID %d", joyId);
    }
  }

}