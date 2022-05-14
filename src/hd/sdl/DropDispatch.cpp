#include "hd/sdl/DropDispatch.hpp"

#include "hd/Error.hpp"
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
  DropDispatch::Trigger (const SDL_Event &e)
  {
    switch (e.type) {
    case SDL_DROPBEGIN:
      Begin.Trigger (e);
      break;
    case SDL_DROPCOMPLETE:
      Complete.Trigger (e);
      break;
    case SDL_DROPFILE:
      File.Trigger (e);
      break;
    case SDL_DROPTEXT:
      Text.Trigger (e);
      break;
    default:
      hdError ("Invalid Drop Event type %d", e.type);
    }
    EventDispatch::Trigger (e);
  }

}