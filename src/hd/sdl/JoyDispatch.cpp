#include "hd/sdl/JoyDispatch.hpp"
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
  JoyDispatch::Trigger (const SDL_Event &e)
  {
    hdDebugCall(NULL);
    EventDispatch::Trigger (e);
    switch (e.type) {
    case SDL_JOYAXISMOTION:
      if (Axis.find (e.jaxis.axis) != Axis.end ()) {
        Axis[e.jaxis.axis].Trigger (e);
      } else {
        hdDebug ("No handlers for Joystick Axis #%d", e.jaxis.axis);
      }
      break;
    case SDL_JOYBALLMOTION:
      if (Ball.find (e.jball.ball) != Ball.end ()) {
        Ball[e.jball.ball].Trigger (e);
      } else {
        hdDebug ("No handlers for Joystick Ball #%d", e.jball.ball);
      }
      break;
    case SDL_JOYHATMOTION:
      if (Hat.find (e.jhat.hat) != Hat.end ()) {
        Hat[e.jhat.hat].Trigger (e);
      }
      break;
    case SDL_JOYBUTTONDOWN:
    case SDL_JOYBUTTONUP:
      if (Button.find (e.jbutton.button) != Button.end ()) {
        Button[e.jbutton.button].Trigger (e);
      } else {
        hdDebug ("No handlers for Joystick Button #%d", e.jbutton.button);
      }
      break;
    case SDL_JOYDEVICEADDED:
    case SDL_JOYDEVICEREMOVED:
      // Valid events but they are simply not handled specially here:
      break;
    default:
      hdError ("Invalid Joystick Event %d", e.type);
    }
  }

}