/**
 * @file ControllerDeviceDispatch.cpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-14
 *
 * @copyright GNU-GPL 3.0 Copyright (C) 2022 Jón Davíð Sawyer
 * (badquanta@gmail.com)
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
 *
 *
 */
#include "hd/sdl/ControllerDeviceDispatch.hpp"
#include "hd/Error.hpp"

namespace hd::sdl {
  void
  ControllerDeviceDispatch::Trigger (const SDL_Event &e)
  {
    SDL_JoystickID controllerId;
    switch (e.type) {
    case SDL_CONTROLLERAXISMOTION: // controller axis motion
      controllerId = e.caxis.which;
      break;
    case SDL_CONTROLLERBUTTONDOWN: // controller button pressed
    case SDL_CONTROLLERBUTTONUP:   // controller button released
      controllerId = e.cbutton.which;
    case SDL_CONTROLLERDEVICEADDED: // controller connected
      controllerId = e.cdevice.which;
      Added.Trigger (e);
      break;
    case SDL_CONTROLLERDEVICEREMOVED: // controller disconnected
      controllerId = e.cdevice.which;
      Removed.Trigger (e);
      break;
    case SDL_CONTROLLERDEVICEREMAPPED: // controller mapping updated
      controllerId = e.cdevice.which;
      break;
    default:
      hdError ("Invalid Controller SDL Event type %d", e.type);
      return;
    }
    EventDispatch::Trigger (e);
    if (find (controllerId) != end ()) {
      at (controllerId).Trigger (e);
    }
  }

}