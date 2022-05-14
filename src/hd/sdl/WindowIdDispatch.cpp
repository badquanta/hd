#include "hd/sdl/WindowIdDispatch.hpp"
#include "hd/Error.hpp"
#include "hd/Debug.hpp"
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
namespace hd::sdl {
  void
  WindowIdDispatch::Trigger (const SDL_Event &e)
  {
    Uint32 windowId;
    switch (e.type) {
    case SDL_WINDOWEVENT: // window state change
      windowId = e.window.windowID;
      break;
    case SDL_KEYDOWN: // key pressed
    case SDL_KEYUP:   // key released
      windowId = e.key.windowID;
      break;
    case SDL_TEXTEDITING: // keyboard text editing (composition)
      windowId = e.edit.windowID;
    case SDL_TEXTINPUT: // keyboard text input
      windowId = e.text.windowID;
      break;
    case SDL_MOUSEMOTION: // mouse moved
      windowId = e.motion.windowID;
      break;
    case SDL_MOUSEBUTTONDOWN: // mouse button pressed
    case SDL_MOUSEBUTTONUP:   // mouse button released
      windowId = e.button.windowID;
      break;
    case SDL_MOUSEWHEEL: // mouse wheel motion
      windowId = e.wheel.windowID;
      break;
    case SDL_DROPFILE:     // the system requests a file open
    case SDL_DROPTEXT:     // text/plain drag-and-drop event
    case SDL_DROPBEGIN:    // a new set of drops is beginning (>= SDL 2.0.5)
    case SDL_DROPCOMPLETE: // current set of drops is now complete (>=
                           // SDL 2.0.5)
      windowId = e.drop.windowID;
      break;
    default:
      hdError ("Unkown Window Event type %d", e.type);
      return;
    }
    EventDispatch::Trigger (e);

    if (find (windowId) != end ()) {
      this->at (windowId).Trigger (e);
    } else {
      hdDebugCall ("no handlers for windowId = %d", windowId);
    }
  }

}