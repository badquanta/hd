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
#include "hdShared.hpp"
#include "hdApp.hpp"
hd::Shared::Surface
hd::Shared::makeSurface (const char *path)
{
  SDL_Surface *surface = IMG_Load (path);
  if (surface == NULL)
    {
      return nullptr;
    }
  return Surface (surface, [] (SDL_Surface *s) { SDL_FreeSurface (s); });
}

hd::Shared::Texture
hd::Shared::makeTexture (const char *path, SDL_Renderer *r)
{
  SDL_Surface *surface = IMG_Load (path);
  if (surface == NULL)
    {
      App::printSdlError ();
      return nullptr;
    }
  SDL_Texture *texture = SDL_CreateTextureFromSurface (r, surface);
  SDL_FreeSurface (surface);
  if (texture == NULL)
    {
      App::printSdlError ();

      return nullptr;
    }
  return Texture (texture, [] (SDL_Texture *t) { SDL_DestroyTexture (t); });
}
