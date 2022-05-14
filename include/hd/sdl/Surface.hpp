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

  class Surface : public EngineComponent, public WRAP_PTR<SDL_Surface> {
  public:
    using WRAP_PTR::WRAP_PTR;

    static Surface::s_ptr Create (SDL_Surface *, bool);
    static Surface CreateRGBA (int width,
                               int height,
                               int depth = 32,
                               Uint32 Rmask = 0,
                               Uint32 Gmask = 0,
                               Uint32 Bmask = 0,
                               Uint32 Amask = 0);
    static Surface CreateFormat (int aWidth, int aHeight, int aDepth, Uint32 aFormat);
    static Surface Create (int aWidth, int aHeight);
    static Surface Load (std::filesystem::path);
    static Surface Convert (SDL_Surface *aSurface,
                                     SDL_PixelFormatEnum aFormat=SDL_PIXELFORMAT_RGBA32);

  public: // Instance Methods
    bool Blit (SDL_Surface*, SDL_Rect *aDstRect= NULL, const SDL_Rect *aSrcRect= NULL)const;
    bool BlitScaled (SDL_Surface*, SDL_Rect *aDstRect= NULL, const SDL_Rect *aSrcRect= NULL)const;
    bool FillRect (const SDL_Rect *, Uint32)const;
    bool FillRect (SDL_Rect, Uint32)const;
    Uint32 MapRGBA (Uint8, Uint8, Uint8, Uint8 = 255)const;
    Surface Convert (SDL_PixelFormatEnum aFormat=SDL_PIXELFORMAT_RGBA32)const;
  };
}