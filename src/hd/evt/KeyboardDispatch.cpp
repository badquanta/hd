#include "hd/evt/KeyboardDispatch.hpp"
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
  KeyboardDispatch::Trigger (const SDL_Event &e)
  {
    // hdDebugCall ("%p, type%d", &e,e.type);
    SDL_EventDispatch::Trigger (e);
    switch (e.type) {
    case SDL_KEYDOWN: // key pressed
    case SDL_KEYUP:   // key released
      if (Scancode.find (e.key.keysym.scancode) != Scancode.end ()) {
        Scancode[e.key.keysym.scancode].Trigger (e);
      }
      if (Keycode.find (e.key.keysym.sym) != Keycode.end ()) {
        Keycode[e.key.keysym.sym].Trigger (e);
      }
      break;
    default:
      fprintf (stderr, "Unknown Keyboard event %d\n", e.type);
    }
  }

}