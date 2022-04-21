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

#include "hd/SplashScene.hpp"
#include <filesystem>
namespace hd {
  SplashScene::SplashScene () {}

  bool
  SplashScene::load (SDL_Renderer *r)
  {
    keyState = SDL_GetKeyboardState (NULL);
    std::filesystem::path p;
    splash = Shared::makeTexture ("splash.bmp", r);
    up = Shared::makeTexture ("kgi/Black/1x/arrowUp.png", r);
    down = Shared::makeTexture ("kgi/Black/1x/arrowDown.png", r);
    left = Shared::makeTexture ("kgi/Black/1x/arrowLeft.png", r);
    right = Shared::makeTexture ("kgi/Black/1x/arrowRight.png", r);
    font = Shared::loadFont ("ttf/Ac437_IBM_VGA_8x16.ttf", 16);
    textMsg = Shared::renderText (
        r, font, "Hello World!\nThis is a message.", { 0, 0, 0 });
    if (splash != NULL) {
      return true;
    } else {
      return false;
    }
  }
  void
  SplashScene::unload ()
  {
    if (splash != NULL) {
      splash = nullptr;
      splash = NULL;
    }
  }

  void
  SplashScene::render (SDL_Renderer *r)
  {
    SDL_Rect dstBox = { 0, 0 };
    int scrW, scrH;
    SDL_GetRendererOutputSize (r, &scrW, &scrH);
    dstBox.w = scrW;
    dstBox.h = scrH;
    SDL_RenderCopy (r, splash.get (), NULL, &dstBox);
    int texW, texH;
    if (keyState[SDL_SCANCODE_UP]) {
      SDL_QueryTexture (up.get (), NULL, NULL, &texW, &texH);
      dstBox = { (scrW / 2) - (texW / 2), 0, texW, texH };
      SDL_RenderCopy (r, up.get (), NULL, &dstBox);
    }
    if (keyState[SDL_SCANCODE_DOWN]) {
      SDL_QueryTexture (down.get (), NULL, NULL, &texW, &texH);
      dstBox = { (scrW / 2) - (texW / 2), scrH - texH, texW, texH };
      SDL_RenderCopy (r, down.get (), NULL, &dstBox);
    }
    if (keyState[SDL_SCANCODE_LEFT]) {
      SDL_QueryTexture (left.get (), NULL, NULL, &texW, &texH);
      dstBox = { 0, (scrH / 2) - (texH / 2), texW, texH };
      SDL_RenderCopy (r, left.get (), NULL, &dstBox);
    }
    if (keyState[SDL_SCANCODE_RIGHT]) {
      SDL_QueryTexture (right.get (), NULL, NULL, &texW, &texH);
      dstBox = { scrW - texW, (scrH / 2) - (texH / 2), texW, texH };
      SDL_RenderCopy (r, right.get (), NULL, &dstBox);
    }
    SDL_QueryTexture (textMsg.get(), NULL, NULL, &texW, &texH);
    dstBox = { 0, 0, texW, texH };
    SDL_RenderCopy (r, textMsg.get (), NULL, &dstBox);
  }
} // namespace hd