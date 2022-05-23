/**
 * @file Surface.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-05-08
 *
 * @copyright GNU-GPL 3.0
 *
 */
#pragma once
#include "hd/EngineComponent.hpp"
#include "hd/Wrap.hpp"
namespace hd::sdl {
  /**
   * @brief Encapsulate interface and memory management of an `SDL_Surface*`
   * pointer.
   *
   */
  class Surface : public WRAP_PTR<SDL_Surface> {
  public:
    using WRAP_PTR::WRAP_PTR;

    static Surface::s_ptr Create (SDL_Surface *, bool);
    static Surface CreateRGBA (int width,
                               int height,
                               int depth = 32,
                               Uint32 rMask = 0,
                               Uint32 gMask = 0,
                               Uint32 bMask = 0,
                               Uint32 aMask = 0);
    static Surface
    CreateFormat (int aWidth, int aHeight, int aDepth, Uint32 aFormat);
    static Surface Create (int aWidth, int aHeight);
    static Surface Load (std::filesystem::path);
    /**
     * @brief static version
     *  @see https://wiki.libsdl.org/SDL_ConvertSurface **/
    static Surface Convert (SDL_Surface *aSurface,
                            SDL_PixelFormatEnum aFormat
                            = SDL_PIXELFORMAT_RGBA32);

  public: // Instance Methods
    /**
     * @brief instance version
     *  @see https://wiki.libsdl.org/SDL_ConvertSurface **/
    Surface Convert (SDL_PixelFormatEnum aFormat
                     = SDL_PIXELFORMAT_RGBA32) const;
    /** @see https://wiki.libsdl.org/SDL_BlitSurface **/
    bool BlitTo (SDL_Surface *,
                 SDL_Rect *aDstRect = NULL,
                 const SDL_Rect *aSrcRect = NULL) const;
    /** @see https://wiki.libsdl.org/SDL_BlitSurface **/
    bool BlitFrom (SDL_Surface *,
                   SDL_Rect *aDstRect = NULL,
                   const SDL_Rect *aSrcRect = NULL) const;
    /** @see https://wiki.libsdl.org/SDL_BlitSurface **/
    bool BlitFrom (SDL_Surface *,
                   SDL_Rect &aDstRect,
                   const SDL_Rect &aSrcRect) const;
    /** @see https://wiki.libsdl.org/SDL_BlitScaled **/
    bool BlitScaledTo (SDL_Surface *,
                       SDL_Rect *aDstRect = NULL,
                       const SDL_Rect *aSrcRect = NULL) const;
    /** @see https://wiki.libsdl.org/SDL_BlitScaled **/
    bool BlitScaledFrom (SDL_Surface *,
                         SDL_Rect *aDstRect = NULL,
                         const SDL_Rect *aSrcRect = NULL) const;
    /** @see https://wiki.libsdl.org/SDL_BlitScaled **/
    bool BlitScaledFrom (SDL_Surface *,
                         SDL_Rect &aDstRect,
                         const SDL_Rect &aSrcRect) const;
    /** @see https://wiki.libsdl.org/SDL_FillRect **/
    bool FillRect (const SDL_Rect *, Uint32) const;
    /** @see https://wiki.libsdl.org/SDL_FillRects **/
    bool FillRects (const SDL_Rect *, int, Uint32) const;

    /** @see https://wiki.libsdl.org/SDL_FillRect **/
    bool FillRect (SDL_Rect, Uint32) const;
    /** @see https://wiki.libsdl.org/SDL_MapRGBA **/
    Uint32 MapRGBA (Uint8, Uint8, Uint8, Uint8 = 255) const;
    /** @see https://wiki.libsdl.org/SDL_MapRGBA **/
    Uint32 MapRGBA (SDL_Color) const;
    /** @see https://wiki.libsdl.org/SDL_GetClipRect **/
    SDL_Rect GetClip ();
    /** @see https://wiki.libsdl.org/SDL_SetClipRect **/
    bool SetClip (SDL_Rect);
    /** @see https://wiki.libsdl.org/SDL_SetColorKey **/
    bool SetColorKey (int, Uint32);
    /** Lets one use an SDL_Color to specify the key **/
    bool SetColorKey (SDL_Color);
    /** @see https://wiki.libsdl.org/SDL_GetColorKey **/
    Uint32 GetColorKey ();
    /** Tests to see if ColorKeys are enabled. **/
    bool IsColorKeyed ();
    /** Disable color keys, if they were enabled. **/
    bool DisableColorKey ();
  };
}