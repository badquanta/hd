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

  Surface::s_ptr
  Surface::Create (SDL_Surface *p, bool b)
  {
    s_ptr pointer (p, [b] (SDL_Surface *p) {
      if (b) {
        hdDebug ("FREEING %p", p);
        SDL_FreeSurface (p);
      } else {
        //hdDebugCall ("Not freeing %p", p);
      }
    });
    return pointer;
  }

  Surface
  Surface::CreateRGBA (Uint32 aFlags,
                       int aWidth,
                       int aHeight,
                       int aDepth,
                       Uint32 aRedMask,
                       Uint32 aGreenMask,
                       Uint32 aBlueMask,
                       Uint32 aAlphaMask)
  {
    SDL_Surface *created = SDL_CreateRGBSurface (
        aFlags, aWidth, aHeight, aDepth, aRedMask, aGreenMask, aBlueMask, aAlphaMask);
    if (!created) {
      hdError ("Unable to create surface because: %s", SDL_GetError ());
    }
    return s_ptr(created, SDL_FreeSurface);
  }

  Surface
  Surface::Load (std::filesystem::path aPath)
  {
    SDL_Surface *loaded
        = IMG_Load (Engine::FindPath (aPath).generic_string ().c_str ());
    if (loaded == NULL) {
      hdError ("Unable to load '%s' because %s.",
               aPath.generic_string ().c_str (),
               SDL_GetError ());
    }
    return Create (loaded, true);
  }

  Surface Surface::Convert(SDL_Surface* aSurface, SDL_PixelFormatEnum aFormat){
    if (aSurface== NULL) {
      return Surface(NULL);
    }
    SDL_Surface *converted
        = SDL_ConvertSurfaceFormat (aSurface, aFormat, 0);
    if (converted == NULL) {
      hdError("Failed to convert pixel format of surface because: %s",SDL_GetError ());
    }
    return Create (converted, true);
  }

  Surface Surface::Convert(SDL_PixelFormatEnum aFormat){
    return Convert (*this, aFormat);
  }

  bool
  Surface::Blit (SDL_Surface* aOther, SDL_Rect *aDstRect, const SDL_Rect *aSrcRect)
  {
    if (SDL_BlitSurface (
            *this, aSrcRect, aOther, aDstRect)
        == 0) {
      return true;
    } else {
      hdError ("Unable to BlitSurface because: %s", SDL_GetError ());
      return false;
    }
  }
  bool
  Surface::BlitScaled (SDL_Surface* aOther,
                       SDL_Rect *aDstRect,
                       const SDL_Rect *aSrcRect)
  {
    return SDL_BlitSurface (
               *this, aSrcRect, aOther, aDstRect)
           == 0;
  }
  bool
  Surface::FillRect (const SDL_Rect *aRect, Uint32 aColor)
  {
    return SDL_FillRect (*this, aRect, aColor) == 0;
  }
  Uint32
  Surface::MapRGBA (Uint8 r, Uint8 g, Uint8 b, Uint8 a)
  {
    SDL_PixelFormat *format = ptr->format;
    return SDL_MapRGBA (format, r, g, b, a);
  }
}