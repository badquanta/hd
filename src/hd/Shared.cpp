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
  std::list<std::filesystem::path> Shared::searchPaths({
#if HD_DEBUG_BUILD == 1
  HD_SOURCE_ASSETS,
#endif
  "./"
  });
  Shared::Surface
  Shared::makeSurface (const char *path)
  {
    std::filesystem::path realPath = findRealPath (path);
    SDL_Surface *surface = IMG_Load (realPath.generic_string().c_str());
    if (surface == NULL) {
      return nullptr;
    }
    return Surface (surface, [] (SDL_Surface *s) { SDL_FreeSurface (s); });
  }
  /** Load an image from disk into a Texture **/
  Shared::Texture
  Shared::makeTexture (const char *aPath, SDL_Renderer *r)
  {
    std::filesystem::path realPath = aPath;

    for (auto const &base : searchPaths) {
      std::filesystem::path thisPath = base / realPath;
      printf ("Checking for '%s'...\n", thisPath.generic_string ().c_str ());
      if (std::filesystem::exists (thisPath)) {
        realPath = thisPath;
        break;
      }
    }
    SDL_Surface *surface = IMG_Load (realPath.generic_string ().c_str ());
    if (surface == NULL) {
      Engine::printSdlError ();
      return nullptr;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface (r, surface);
    SDL_FreeSurface (surface);
    if (texture == NULL) {
      Engine::printSdlError ();

      return nullptr;
    }
    return Texture (texture, [] (SDL_Texture *t) { SDL_DestroyTexture (t); });
  }

  Shared::Texture
  Shared::makeTexture (SDL_Surface *aSurface, SDL_Renderer *aRenderer)
  {
    if((aSurface == NULL) || (aRenderer == NULL)){
      return NULL;
    }
    SDL_Texture* tmp = SDL_CreateTextureFromSurface(aRenderer,aSurface);
    if(tmp==NULL) {
      Engine::printSdlError ();
      return NULL;
    }
    return Texture(tmp,[](SDL_Texture*t){
      SDL_DestroyTexture (t);});
  }
  /** Locate the canonical path for the given path.
   * If the path is already absolute; it will simply return the same path
   *given. If it is not, it will attempt to find the actual file by appending
   *searchPaths and checking if it exists; then returning the 'canonical' path.
   *
   * @return aPath IF `aPath` is already absolute OR it cannot find this path
   *within `searchPaths`
   **/
  std::filesystem::path
  Shared::findRealPath (std::filesystem::path aPath)
  {
    if (aPath.is_absolute ()) {
      return aPath;
    }
    std::filesystem::path realPath = aPath;
    for (auto const &base : searchPaths) {
      std::filesystem::path thisPath = base / realPath;
      printf ("Checking for '%s'...\n", thisPath.generic_string ().c_str ());
      if (std::filesystem::exists (thisPath)) {
        return std::filesystem::canonical (thisPath);
      }
    }
    return aPath;
  }

  Shared::Font
  Shared::loadFont (const char *aPath, int aSize)
  {
    TTF_Font *font = TTF_OpenFont (
        findRealPath (aPath).generic_string ().c_str (), aSize);
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
    SDL_Surface *textSurface
        = TTF_RenderText_Blended_Wrapped (aFont.get (), aChars.data (), aColor, 640);
    Texture texture = NULL;
    if (textSurface != NULL) {
      texture = makeTexture (textSurface, aRenderer);
      SDL_FreeSurface (textSurface);
    }
    return texture;
  }
}