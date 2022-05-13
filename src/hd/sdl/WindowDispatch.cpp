#include "hd/sdl/WindowDispatch.hpp"
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
  WindowDispatch::Trigger (const SDL_Event &e)
  {
    EventDispatch::Trigger (e);
    switch (e.type) {
      // window state change
    case SDL_KEYDOWN: // key pressed
    case SDL_KEYUP:   // key released
      Key.Trigger (e);
      break;
    case SDL_TEXTEDITING: // keyboard text editing (composition)
    case SDL_TEXTINPUT:   // keyboard text input
      Text.Trigger (e);
      break;
    case SDL_MOUSEMOTION:     // mouse moved
    case SDL_MOUSEBUTTONDOWN: // mouse button pressed
    case SDL_MOUSEBUTTONUP:   // mouse button released
    case SDL_MOUSEWHEEL:      // mouse wheel motion
      Mouse.Trigger (e);
      break;
    case SDL_DROPFILE:     // the system requests a file open
    case SDL_DROPTEXT:     // text/plain drag-and-drop event
    case SDL_DROPBEGIN:    // a new set of drops is beginning (>= SDL 2.0.5)
    case SDL_DROPCOMPLETE: // current set of drops is now complete (>=
                           // SDL 2.0.5)
      Drop.Trigger (e);
      break;
    case SDL_WINDOWEVENT:
      switch (e.window.event) {
      case SDL_WINDOWEVENT_SHOWN:
        Shown.Trigger (e);
        break;
      case SDL_WINDOWEVENT_HIDDEN:
        Hidden.Trigger (e);
        break;
      case SDL_WINDOWEVENT_EXPOSED:
        Exposed.Trigger (e);
        break;
      case SDL_WINDOWEVENT_MOVED:
        Moved.Trigger (e);
        break;
      case SDL_WINDOWEVENT_RESIZED:
        Resized.Trigger (e);
        break;
      case SDL_WINDOWEVENT_SIZE_CHANGED:
        SizeChanged.Trigger (e);
        break;
      case SDL_WINDOWEVENT_MINIMIZED:
        Minimized.Trigger (e);
        break;
      case SDL_WINDOWEVENT_MAXIMIZED:
        Maximized.Trigger (e);
        break;
      case SDL_WINDOWEVENT_RESTORED:
        Restored.Trigger (e);
        break;
      case SDL_WINDOWEVENT_ENTER:
        Enter.Trigger (e);
        break;
      case SDL_WINDOWEVENT_LEAVE:
        Leave.Trigger (e);
        break;
      case SDL_WINDOWEVENT_FOCUS_GAINED:
        FocusGained.Trigger (e);
        break;
      case SDL_WINDOWEVENT_FOCUS_LOST:
        FocusLost.Trigger (e);
        break;
      case SDL_WINDOWEVENT_CLOSE:
        Close.Trigger (e);
        break;
#if SDL_VERSION_ATLEAST(2, 0, 5)
      case SDL_WINDOWEVENT_TAKE_FOCUS:
        TakeFocus.Trigger (e);
        break;
      case SDL_WINDOWEVENT_HIT_TEST:
        HitTest.Trigger (e);
        break;
#endif
      default:
        SDL_Log ("Window %d got unknown event %x",
                 e.window.windowID,
                 e.window.event);
        break;
      }
      break;
    default:
      hdError ("Invalid Window Event type %x", e.type);
    }
  }

}