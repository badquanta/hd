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
#include "hd/Debug.hpp"
#include "hd/Error.hpp"
namespace hd::sdl {

  Surface::s_ptr
  Surface::Create (SDL_Surface *p, bool b)
  {
    s_ptr pointer (p, [b] (SDL_Surface *p) {
      if (b) {
        hdDebug ("FREEING %p", p);
        SDL_FreeSurface (p);
      } else {
        // hdDebugCall ("Not freeing %p", p);
      }
    });
    return pointer;
  }

  Surface
  Surface::CreateRGBA (int aWidth,
                       int aHeight,
                       int aDepth,
                       Uint32 aRedMask,
                       Uint32 aGreenMask,
                       Uint32 aBlueMask,
                       Uint32 aAlphaMask)
  {
    /** flags is 0 because it is unused. @see
     *https://wiki.libsdl.org/SDL_CreateRGBSurface#function_parameters
     **/
    SDL_Surface *created = SDL_CreateRGBSurface (0,
                                                 aWidth,
                                                 aHeight,
                                                 aDepth,
                                                 aRedMask,
                                                 aGreenMask,
                                                 aBlueMask,
                                                 aAlphaMask);
    hdErrorIf (
        !created, "Unable to create surface because: %s", SDL_GetError ());
    return s_ptr (created, SDL_FreeSurface);
  }

  Surface
  Surface::CreateFormat (int aWidth, int aHeight, int aDepth, Uint32 aFormat)
  {
    SDL_Surface *created
        = SDL_CreateRGBSurfaceWithFormat (0, aWidth, aHeight, aDepth, aFormat);
    hdErrorIf (
        !created, "unable to create surface because %s", SDL_GetError ());
    return s_ptr (created, SDL_FreeSurface);
  }
  Surface
  Surface::Create (int aWidth, int aHeight)
  {
    return CreateFormat (aWidth, aHeight, 32, SDL_PIXELFORMAT_RGBA32);
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

  Surface
  Surface::Convert (SDL_Surface *aSurface, SDL_PixelFormatEnum aFormat)
  {
    if (aSurface == NULL) {
      return Surface (NULL);
    }
    SDL_Surface *converted = SDL_ConvertSurfaceFormat (aSurface, aFormat, 0);
    if (converted == NULL) {
      hdError ("Failed to convert pixel format of surface because: %s",
               SDL_GetError ());
    }
    return Create (converted, true);
  }

  Surface
  Surface::Convert (SDL_PixelFormatEnum aFormat) const
  {
    return Convert (*this, aFormat);
  }

  bool
  Surface::BlitTo (SDL_Surface *aOther,
                   SDL_Rect *aDstRect,
                   const SDL_Rect *aSrcRect) const
  {
    if (SDL_BlitSurface (*this, aSrcRect, aOther, aDstRect) == 0) {
      return true;
    } else {
      hdError ("Unable to BlitSurface because: %s", SDL_GetError ());
      return false;
    }
  }

  bool
  Surface::BlitFrom (SDL_Surface *aOther,
                     SDL_Rect *aDstRect,
                     const SDL_Rect *aSrcRect) const
  {
    if (SDL_BlitSurface (aOther, aSrcRect, *this, aDstRect) == 0) {
      return true;
    } else {
      hdError ("Unable to BlitSurface because: %s", SDL_GetError ());
      return false;
    }
  }

  bool
  Surface::BlitFrom (SDL_Surface *aOther,
                           SDL_Rect& aDstRect,
                           const SDL_Rect& aSrcRect) const
  {
    return BlitFrom (aOther, &aDstRect, &aSrcRect);
  }
  bool
  Surface::BlitScaledTo (SDL_Surface *aOther,
                         SDL_Rect *aDstRect,
                         const SDL_Rect *aSrcRect) const
  {
    return SDL_BlitScaled (*this, aSrcRect, aOther, aDstRect) == 0;
  }
  bool
  Surface::BlitScaledFrom (SDL_Surface *aOther,
                           SDL_Rect *aDstRect,
                           const SDL_Rect *aSrcRect) const
  {
    return SDL_BlitScaled (aOther, aSrcRect, *this, aDstRect) == 0;
  }
  bool
  Surface::BlitScaledFrom (SDL_Surface *aOther,
                           SDL_Rect& aDstRect,
                           const SDL_Rect& aSrcRect) const
  {
    return BlitScaledFrom (aOther, &aDstRect, &aSrcRect);
  }
  bool
  Surface::FillRect (const SDL_Rect *aRect, Uint32 aColor) const
  {
    return SDL_FillRect (*this, aRect, aColor) == 0;
  }
  bool
  Surface::FillRect (SDL_Rect aRect, Uint32 aColor) const
  {
    return FillRect (&aRect, aColor);
  }

  Uint32
  Surface::MapRGBA (Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
  {
    SDL_PixelFormat *format = ptr->format;
    return SDL_MapRGBA (format, r, g, b, a);
  }
}