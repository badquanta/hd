#include "hd/Events.hpp"
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
namespace hd {

  void
  Event::Trigger (const SDL_Event &e)
  {
    for (std::pair<int, Handler> pair : handlers) {
      pair.second (e);
    }
  }
  unsigned int
  Event::Add (Event::Handler aHandler)
  {
    unsigned int id = nextId;
    nextId++;
    handlers[id] = aHandler;
    return id;
  }
  bool
  Event::Delete (unsigned int aId)
  {
    return handlers.erase (aId) == 1;
  }
  void
  TypeEvent::Trigger (const SDL_Event &e)
  {
    Event::Trigger (e);
    switch (e.type) {
    case SDL_QUIT: // user-requested quit; see Remarks for details
      Quit.Trigger (e);
      break;
    case SDL_APP_TERMINATING:         // OS is terminating the application
    case SDL_APP_LOWMEMORY:           // OS is low on memory; free some
    case SDL_APP_WILLENTERBACKGROUND: // application is entering background
    case SDL_APP_DIDENTERBACKGROUND:  // application entered background
    case SDL_APP_WILLENTERFOREGROUND: // application is entering foreground
    case SDL_APP_DIDENTERFOREGROUND:  // application entered foreground
      App.Trigger (e);
      break;
    case SDL_WINDOWEVENT: // window state change
      Window.Trigger (e);
      break;
    case SDL_SYSWMEVENT: // system specific event
      SysWM.Trigger (e);
      break;
    case SDL_KEYDOWN:       // key pressed
    case SDL_KEYUP:         // key released
    case SDL_KEYMAPCHANGED: // keymap changed due to a system event such as an
                            // input language or keyboard layout change (>=
                            // SDL 2.0.4)
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
    case SDL_JOYAXISMOTION:    // joystick axis motion
    case SDL_JOYBALLMOTION:    // joystick trackball motion
    case SDL_JOYHATMOTION:     // joystick hat position change
    case SDL_JOYBUTTONDOWN:    // joystick button pressed
    case SDL_JOYBUTTONUP:      // joystick button released
    case SDL_JOYDEVICEADDED:   // joystick connected
    case SDL_JOYDEVICEREMOVED: // joystick disconnected
      Joy.Trigger (e);
      break;
    case SDL_CONTROLLERAXISMOTION:     // controller axis motion
    case SDL_CONTROLLERBUTTONDOWN:     // controller button pressed
    case SDL_CONTROLLERBUTTONUP:       // controller button released
    case SDL_CONTROLLERDEVICEADDED:    // controller connected
    case SDL_CONTROLLERDEVICEREMOVED:  // controller disconnected
    case SDL_CONTROLLERDEVICEREMAPPED: // controller mapping updated
      Controller.Trigger (e);
      break;
    case SDL_FINGERDOWN:   // user has touched input device
    case SDL_FINGERUP:     // user stopped touching input device
    case SDL_FINGERMOTION: // user is dragging finger on input device
      Finger.Trigger (e);
      break;
    case SDL_DOLLARGESTURE: //
    case SDL_DOLLARRECORD:  //
    case SDL_MULTIGESTURE:  //
      Dollar.Trigger (e);
      break;
    case SDL_CLIPBOARDUPDATE: // the clipboard changed
      Clipboard.Trigger (e);
      break;
    case SDL_DROPFILE:     // the system requests a file open
    case SDL_DROPTEXT:     // text/plain drag-and-drop event
    case SDL_DROPBEGIN:    // a new set of drops is beginning (>= SDL 2.0.5)
    case SDL_DROPCOMPLETE: // current set of drops is now complete (>=
                           // SDL 2.0.5)
      Drop.Trigger (e);
      break;
    case SDL_AUDIODEVICEADDED:   // a new audio device is available (>=
                                 // SDL 2.0.4)
    case SDL_AUDIODEVICEREMOVED: // an audio device has been removed (>=
                                 // SDL 2.0.4)
      Audio.Trigger (e);
      break;
    case SDL_RENDER_TARGETS_RESET: // the render targets have been reset and
                                   // their contents need to be updated (>=
                                   // SDL 2.0.2)
    case SDL_RENDER_DEVICE_RESET: // the device has been reset and all textures
                                  // need to be recreated (>= SDL 2.0.4)
      Render.Trigger (e);
      break;
    case SDL_USEREVENT:
      // These are for your use, and should be allocated with
      // SDL_RegisterEvents()
      User.Trigger (e);
      break;
    default:
      fprintf (stderr, "Unknown Event %d\n", e.type);
    }
  }
  void
  MouseEvent::Trigger (const SDL_Event &e)
  {
    Event::Trigger (e);
    switch (e.type) {
    case SDL_MOUSEMOTION: // mouse moved
      Motion.Trigger (e);
      break;
    case SDL_MOUSEBUTTONDOWN: // mouse button pressed
    case SDL_MOUSEBUTTONUP:   // mouse button released
      Button[e.button.button].Trigger (e);
      break;
    case SDL_MOUSEWHEEL: // mouse wheel motion
      Wheel.Trigger (e);
      break;
    default:
      fprintf (stderr, "Unknown MouseEvent %d\n", e.type);
    }
  }
  void
  MouseButtonEvent::Trigger (const SDL_Event &e)
  {
    Event::Trigger (e);
    switch (e.type) {
    case SDL_MOUSEBUTTONDOWN: // mouse button pressed
      Down.Trigger (e);
      break;
    case SDL_MOUSEBUTTONUP: // mouse button released
      Up.Trigger (e);
      break;
    default:
      fprintf (stderr, "Unknown MouseButtonEvent %d\n", e.type);
    }
  }
  void
  KeyboardEvent::Trigger (const SDL_Event &e)
  {
    Event::Trigger (e);
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
    case SDL_KEYMAPCHANGED: // keymap changed due to a system event such as an
                            // input language or keyboard layout change (>=
                            // SDL 2.0.4)
      MapChange.Trigger (e);
      break;
    default:
      fprintf (stderr, "Unknown Keyboard event %d\n", e.type);
    }
  }
  void
  KeyEvent::Trigger (const SDL_Event &e)
  {
    Event::Trigger (e);
    switch(e.type){
      case SDL_KEYDOWN:
        Down.Trigger(e);
        break;
      case SDL_KEYUP:
        Up.Trigger (e);
        break;
      default:
        fprintf (stderr, "Unknown Key event %d\n", e.type);
      }
  }

} // namespace hd