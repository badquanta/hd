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

#include "hdSplashScene.hpp"
#include <filesystem>
namespace hd
{
  bool
  SplashScene::load (SDL_Renderer *r)
  {
    std::filesystem::path p;
    splash = Shared::makeTexture ("splash.bmp", r);
    if (splash != NULL)
      {
        return true;
      }
    else
      {
        return false;
      }
  }
  void
  SplashScene::unload ()
  {
    if (splash != NULL)
      {
        splash = nullptr;
        splash = NULL;
      }
  }

  void
  SplashScene::render (SDL_Renderer *r)
  {
    SDL_Rect dstBox = { 0, 0 };
    SDL_GetRendererOutputSize (r, &dstBox.w, &dstBox.h);
    printf ("dstBox@(%d,%d)of(%d,%d)\n", dstBox.x, dstBox.y, dstBox.w,
            dstBox.h);
    SDL_RenderCopy (r, splash.get (), NULL, &dstBox);
  }
} // namespace hd