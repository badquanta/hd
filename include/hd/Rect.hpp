/**
 * @file Rect.hpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-16
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
#pragma once
#include "SDL2/SDL_rect.h"
#include "hd/Wrap.hpp"
namespace hd {
  /**
   * @brief Extend the SDL_Rect
   *
   */
  class Rect : public WRAP_IDENTITY<SDL_Rect> {
  public:
    /** utilize parent constructors **/
    using WRAP_IDENTITY::WRAP_IDENTITY;
    Rect (int = 0, int = 0, int = 0, int = 0);
    operator SDL_Rect * ();
    static Rect Union (const SDL_Rect *a1, const SDL_Rect *a2);
    Rect Union (const SDL_Rect);
  };
}