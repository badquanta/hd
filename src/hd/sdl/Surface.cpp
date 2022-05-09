/**
 * @file Surface.cpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-08
 *
 * @copyright GNU-GPL 3.0
 *
 */
#include "hd/sdl/Surface.hpp"

namespace hd::sdl {
  Surface
  Surface::CreateRGBA (Uint32 flags,
                       int width,
                       int height,
                       int depth,
                       Uint32 Rmask,
                       Uint32 Gmask,
                       Uint32 Bmask,
                       Uint32 Amask)
  {
    SDL_Surface *created = SDL_CreateRGBSurface (
        flags, width, height, depth, Rmask, Gmask, Bmask, Amask);
    if(!created){
      hdError ("Unable to create surface because: %s", SDL_GetError ());
    }
    return Surface(created, true);
  }
  Surface::Surface (SDL_Surface *p, bool b) : EnCom<SDL_Surface> (p, b){

  }

  void
  Surface::Free ()
  {
    SDL_FreeSurface (get ());
  }
}