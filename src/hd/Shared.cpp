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
#include "hd/Program.hpp"
namespace hd
{
  std::list<std::filesystem::path> Shared::searchPaths;
  Shared::Surface
  Shared::makeSurface (const char *path)
  {
    SDL_Surface *surface = IMG_Load (path);
    if (surface == NULL)
      {
        return nullptr;
      }
    return Surface (surface, [] (SDL_Surface *s) { SDL_FreeSurface (s); });
  }

  Shared::Texture
  Shared::makeTexture (const char *path, SDL_Renderer *r)
  {
    std::filesystem::path realPath = path;

    for (auto const &base : searchPaths)
      {
        std::filesystem::path thisPath = base / realPath;
        printf ("Checking for '%s'...\n", thisPath.generic_string ().c_str ());
        if (std::filesystem::exists (thisPath))
          {
            realPath = thisPath;
            break;
          }
      }
    SDL_Surface *surface = IMG_Load (realPath.generic_string ().c_str ());
    if (surface == NULL)
      {
        Program::printSdlError ();
        return nullptr;
      }
    SDL_Texture *texture = SDL_CreateTextureFromSurface (r, surface);
    SDL_FreeSurface (surface);
    if (texture == NULL)
      {
        Program::printSdlError ();

        return nullptr;
      }
    return Texture (texture, [] (SDL_Texture *t) { SDL_DestroyTexture (t); });
  }
}