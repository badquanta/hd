#include "hd/evt/MouseDispatch.hpp"
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
  MouseDispatch::Trigger (const SDL_Event &e)
  {
    SDL_EventDispatch::Trigger (e);
    switch (e.type) {
    case SDL_MOUSEMOTION: // mouse moved
      //hdDebug ("Motion");
      Motion.Trigger (e);
      break;
    case SDL_MOUSEBUTTONDOWN: // mouse button pressed
    case SDL_MOUSEBUTTONUP:   // mouse button released
      if (Button.find (e.button.button) != Button.end ()) {
        Button[e.button.button].Trigger (e);
      }
      break;
    case SDL_MOUSEWHEEL: // mouse wheel motion
      //hdDebug ("Wheel");
      Wheel.Trigger (e);
      break;
    default:
      hdError ("Unknown MouseDispatch %d\n", e.type);
    }

  }

}