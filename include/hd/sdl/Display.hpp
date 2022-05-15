/**
 * @file Display.hpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-14
 *
 * @copyright GNU-GPL 3.0 Copyright (C) 2022 Jón Davíð Sawyer (badquanta@gmail.com)
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
#pragma once
#include "hd/Wrap.hpp"
#include "SDL2/SDL_video.h"
namespace hd::sdl {
    /**
     * @brief Wrap an SDL_DisplayMode pointer?
     * @todo Not sure if this is what I mean to do.
     *
     */
    class DisplayMode : public WRAP_PTR<SDL_DisplayMode> {
    };
    /**
     * @brief Displays are identified by an integer.
     *
     */
    class Display : public WRAP_IDENTITY<int> {
    public:
      using WRAP_IDENTITY::WRAP_IDENTITY;
      using WRAP_IDENTITY::operator=;
      /** @see SDL_GetNumVideoDisplays **/
      static int Count ();
      static Display Get (int);
    public:
      SDL_Rect GetBounds ();
      SDL_DisplayMode CurrentDisplayMode ();
    };
}