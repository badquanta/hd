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
namespace hd::sdl {

  class Surface : public EngineComponent<Surface, SDL_Surface> {
  public:
    static s_ptr Create(SDL_Surface*, bool);
    static s_ptr CreateRGBA (Uint32 flags,
                               int width,
                               int height,
                               int depth,
                               Uint32 Rmask,
                               Uint32 Gmask,
                               Uint32 Bmask,
                               Uint32 Amask);
    static s_ptr Load (std::filesystem::path);
  public:// Instance Methods
    virtual void Free () override;
    bool Blit (s_ptr, SDL_Rect *aDstRect= NULL, const SDL_Rect *aSrcRect= NULL);
    bool BlitScaled (s_ptr, SDL_Rect *aDstRect= NULL, const SDL_Rect *aSrcRect= NULL);

  protected: /** Hidden Constructor **/
    Surface (SDL_Surface *, bool);
  };
}