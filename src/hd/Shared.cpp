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
#include "hd/Shared.hpp"
#include "hd/Engine.hpp"
namespace hd {

  Shared::Surface
  Shared::makeSurface (SDL_Surface *aSurface)
  {
    if (aSurface == NULL)
      return NULL;
    return Surface (aSurface, [] (SDL_Surface *s) { SDL_FreeSurface (s); });
  }
  Shared::Surface
  Shared::ConvertSurface(Shared::Surface aSurface, SDL_PixelFormatEnum aFormat){
    if (aSurface == NULL) {
      return NULL;
    }
    SDL_Surface *converted
        = SDL_ConvertSurfaceFormat (aSurface.get (), aFormat, 0);
    if (converted == NULL) {
      fprintf (stderr,
               "Failed to convert pixel format of surface because: %s\n",
               SDL_GetError ());
      return NULL;
    }
    return makeSurface (converted);
  }
  Shared::Surface
  Shared::makeSurface (std::filesystem::path path)
  {
    std::filesystem::path realPath = Engine::FindPath (path);
    SDL_Surface *surface = IMG_Load (realPath.generic_string ().c_str ());
    if (surface == NULL) {
      return nullptr;
    }
    return makeSurface (surface);
  }
  /** Load an image from disk into a Texture **/
  Shared::Texture
  Shared::makeTexture (const char *aPath, SDL_Renderer *r)
  {
    std::filesystem::path realPath = Engine::FindPath (aPath);
    SDL_Surface *surface = IMG_Load (realPath.generic_string ().c_str ());
    if (surface == NULL) {
      Engine::PrintSdlError ();
      return nullptr;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface (r, surface);
    SDL_FreeSurface (surface);
    if (texture == NULL) {
      Engine::PrintSdlError ();

      return nullptr;
    }
    return Texture (texture, [] (SDL_Texture *t) { SDL_DestroyTexture (t); });
  }

  Shared::Texture
  Shared::makeTexture (SDL_Surface *aSurface, SDL_Renderer *aRenderer)
  {
    if ((aSurface == NULL) || (aRenderer == NULL)) {
      return NULL;
    }
    SDL_Texture *tmp = SDL_CreateTextureFromSurface (aRenderer, aSurface);
    if (tmp == NULL) {
      Engine::PrintSdlError ();
      return NULL;
    }
    return Texture (tmp, [] (SDL_Texture *t) { SDL_DestroyTexture (t); });
  }


  Shared::Font
  Shared::loadFont (const char *aPath, int aSize)
  {
    TTF_Font *font = TTF_OpenFont (
        Engine::FindPath (aPath).generic_string ().c_str (), aSize);
    if (font != NULL) {
      return Font (font, [] (TTF_Font *f) { TTF_CloseFont (f); });
    } else {
      return NULL;
    }
  }

  Shared::Texture
  Shared::renderText (SDL_Renderer *aRenderer,
                      Font aFont,
                      std::string_view aChars,
                      SDL_Color aColor)
  {
    SDL_Surface *textSurface = TTF_RenderText_Blended_Wrapped (
        aFont.get (), aChars.data (), aColor, 640);
    Texture texture = NULL;
    if (textSurface != NULL) {
      texture = makeTexture (textSurface, aRenderer);
      SDL_FreeSurface (textSurface);
    }
    return texture;
  }
}