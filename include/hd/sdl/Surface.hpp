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
  template <typename target> class EnCom : public std::shared_ptr<target>{
      private:
        bool m_Free;
      public:
        EnCom (target *p, bool aFree = false):std::shared_ptr<target>(p),m_Free(aFree),engine(Engine::Get()){};
        virtual ~EnCom(){if(m_Free){this->Free();}
        };
        const Engine::s_ptr engine;
        virtual void Free (){};
  };
  class Surface : public EnCom<SDL_Surface> {
  public:
    Surface (SDL_Surface *, bool);
    static Surface CreateRGBA (Uint32 flags,
                               int width,
                               int height,
                               int depth,
                               Uint32 Rmask,
                               Uint32 Gmask,
                               Uint32 Bmask,
                               Uint32 Amask);
    ~Surface ();
    virtual void Free () override;
  };
}