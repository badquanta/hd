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
  Surface::Surface (SDL_Surface *p, bool b) : EngineComponent (p, b) {}
  Surface::s_ptr
  Surface::Create (SDL_Surface *p, bool b)
  {
    if (p == NULL) {
      return NULL;
    }
    return Surface::s_ptr (new Surface (p, b));
  }

  Surface::s_ptr
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
    if (!created) {
      hdError ("Unable to create surface because: %s", SDL_GetError ());
    }
    return Create (created, true);
  }

  Surface::s_ptr
  Surface::Load (std::filesystem::path aPath)
  {
    SDL_Surface *loaded
        = IMG_Load (Engine::FindPath (aPath).generic_string ().c_str ());
    if (loaded == NULL) {
      hdError ("Unable to load '%s' because %s.",
               aPath.generic_string ().c_str (),
               SDL_GetError ());
      return nullptr;
    }
    return Create (loaded, true);
  }

  void
  Surface::Free ()
  {
    hdDebugCall (NULL);
    if (m_Free) {
      SDL_FreeSurface (m_Component);
    }
  }

  bool
  Surface::Blit (Surface::s_ptr aOther,
                 SDL_Rect *aDstRect,
                 const SDL_Rect *aSrcRect)
  {
    if (SDL_BlitSurface (m_Component, aSrcRect, aOther->m_Component, aDstRect)
        == 0) {
      return true;
    } else {
      hdError ("Unable to BlitSurface because: %s", SDL_GetError ());
      return false;
    }
  }
  bool
  Surface::BlitScaled (Surface::s_ptr aOther,
                       SDL_Rect *aDstRect,
                       const SDL_Rect *aSrcRect)
  {
    return SDL_BlitSurface (
               m_Component, aSrcRect, aOther->m_Component, aDstRect)
           == 0;
  }
}