/**
 * @file ControllerDispatch.cpp
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
#include "hd/sdl/ControllerDispatch.hpp"

#include "hd/Error.hpp"

/** __FILENAME__ **/
namespace hd::sdl {
  void
  ControllerDispatch::Trigger (const SDL_Event &e)
  {
    switch (e.type) {
    case SDL_CONTROLLERAXISMOTION:
      if (Axis.find (e.caxis.axis) != Axis.end ()) {
        Axis[e.caxis.axis].Trigger (e);
      }
      break;
    case SDL_CONTROLLERBUTTONDOWN:
    case SDL_CONTROLLERBUTTONUP:
      if (Button.find (e.cbutton.button) != Button.end ()) {
        Button[e.cbutton.button].Trigger (e);
      }
      break;
    case SDL_CONTROLLERDEVICEREMAPPED:
      Remapped.Trigger (e);
      break;
    case SDL_CONTROLLERDEVICEADDED:
    case SDL_CONTROLLERDEVICEREMOVED:
      // Valid events but not handled specially here
      break;
    default:
      hdError ("Invalid Controller Event type: %d", e.type);
    }
  }

}