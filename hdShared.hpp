#pragma once
/**
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
#include "hdCommon.hpp"
#include <memory>
namespace hd
{
  /**
   *
   */
  class Shared
  {
public:
    typedef std::shared_ptr<SDL_Surface> Surface;
    typedef std::shared_ptr<SDL_Texture> Texture;
    static Surface makeSurface (const char *path);
    static Texture makeTexture (const char *path, SDL_Renderer *);
    static Texture makeTexture (Surface, SDL_Renderer *);
  };
} // namespace hd