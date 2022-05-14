/**
 * @file Display.cpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief Implement the wrapping of SDL_DisplayMode, Video Displays, and such.
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
#include "hd/sdl/Display.hpp"
#include "hd/Error.hpp"

namespace hd::sdl {
  int
  Display::Count ()
  {
    return SDL_GetNumVideoDisplays ();
  }
  /**
   * @brief
   * @exception aId if aId is not between 0 & Count()-1
   * @param aId
   * @return Display
   */
  Display
  Display::Get (int aId)
  {
    hdErrorIf (!(0 >= aId) || (aId >= Count ()),
               "Invalid display id %d out of %d possible:",
               aId,
               Count ());
    return aId;
  }
  /**
   * @brief get the origin & size of the display.
   * @see https://wiki.libsdl.org/SDL_GetDisplayBounds
   * @return SDL_Rect filled origin & size of display.
   */
  SDL_Rect
  Display::GetBounds ()
  {
    SDL_Rect r;
    assert (SDL_GetDisplayBounds (*this, &r) == 0);
    return r;
  }
}