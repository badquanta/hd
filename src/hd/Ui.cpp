/**
 * @file Ui.cpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-16
 *
 * @copyright GNU-GPL 3.0 Copyright (C) 2022 Jón Davíð Sawyer
 * (badquanta@gmail.com)
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
 *
 *
 */
#include "hd/Ui.hpp"

/**
 * ### UI Flags
 *
 * @todo more detailed documention about the ui flags.
 */
namespace hd {
  const int UI_FOCUSED_TARGET = 0x01, //
      UI_FOCUSED = 0x02,              //
      UI_CAN_FOCUS = 0x04,            //
      UI_IS_TAB_STOP = 0x08,          //
      UI_GROW_V = 0x0F,               //
      UI_GROW_H = 0x10;
}
/** ### UiComposition **/
namespace hd {
  SDL_Rect
  UiComposition::GetMinimumSize () const
  {
    Rect r{ 0 };
    for (std::pair<int, UiSdlSurfaceView::s_ptr> p : elements) {
      if (p.second) {
        Rect r2;
        r2 = p.second->GetMinimumSize ();
        r = r.Union (r2);
      }
    }
    return r;
  }

  SDL_Rect
  UiComposition::RenderSurface (sdl::Surface s, SDL_Rect r) const
  {
    Rect c{ 0 };
    for (std::pair<int, UiSdlSurfaceView::s_ptr> p : elements) {

      c = c.Union (p.second->RenderSurface (s, r));
    }
    return c;
  }

  int
  UiComposition::Append (UiSdlSurfaceView::s_ptr child)
  {
    int ID = ++NextID;
    elements[ID] = child;
    return ID;
  }
}
/** ### UiCtrlSdlWindowSurface **/
namespace hd {
  UiCtrlSdlWindowSurface::UiCtrlSdlWindowSurface (sdl::Window w) : window (w)
  {
    printf ("Connecting UiCtrlSdlWindowSurface events.\n");
    eventPipeHandle = window.Event ().On (event.pipe);
    outputPipeHandle = Engine::Get ()->output.On (output.pipe);
    event.Close.Void.Once (DoClose);
    output.Void.On (DoRender);
  }
  UiCtrlSdlWindowSurface::~UiCtrlSdlWindowSurface ()
  {
    printf ("Disconnecting UiCtrlSdlWindowSurface events.\n");
    window.Event ().Delete (eventPipeHandle);
    Engine::Get ()->output.Delete (outputPipeHandle);
  }
  SDL_Rect
  UiCtrlSdlWindowSurface::GetMinimumSize () const
  {
    SDL_Rect r;
    if (window) {
      window.GetPosition (&r.x, &r.y);
      window.GetSize (&r.w, &r.h);
    }
    return r;
  }
  SDL_Rect
  UiCtrlSdlWindowSurface::RenderSurface (sdl::Surface s, SDL_Rect r) const
  {
    return root.RenderSurface (s, r);
  }
  SDL_Rect
  UiCtrlSdlWindowSurface::RenderSurface () const
  {
    SDL_Rect r = GetMinimumSize ();
    r.x = r.y = 0;
    if (window) {
      return RenderSurface (window.GetSurface (), r);
    }
    return r;
  }
}
/** ### UiViewText **/
namespace hd {
  UiViewText::UiViewText (sdl::Font f, std::string s, SDL_Color c)
      : font (f), text (s), color (c)
  {
  }

  SDL_Rect
  UiViewText::GetMinimumSize () const
  {
    SDL_Rect r{ 0 };
    if (font) {
      font.SizeUTF8 (text, &r.w, &r.h);
    }
    return r;
  }
  SDL_Rect
  UiViewText::RenderSurface (sdl::Surface dst, SDL_Rect reqRect) const
  {
    SDL_Rect dstRect = reqRect, minRect = GetMinimumSize ();
    if (font) {
      if (!(flags & (UI_GROW_V))) {
        // dstRect.w = minRect.w;
      }
      if (!(flags & (UI_GROW_H))) {
        dstRect.h = minRect.h;
      }
      sdl::Surface tmp
          = font.RenderBlendedTextWrapped (text, color, dstRect.w);
      tmp.BlitTo (dst, &reqRect);
      if (flags & (UI_GROW_H | UI_GROW_V)) {
        SDL_Rect dstRect = reqRect;
        if (!(flags & UI_GROW_H)) {
          dstRect.w = minRect.w;
        }
        if (!(flags & UI_GROW_V)) {
          dstRect.h = minRect.h;
        }
        tmp.BlitScaledTo (dst, &dstRect, &minRect);
      } else {
        dstRect = { reqRect.x, reqRect.y, minRect.w, minRect.h };
        tmp.BlitTo (dst, &dstRect, &minRect);
      }
    }
    return dstRect;
  }
}
/** ### UiCtrlSdlSurface **/
namespace hd {
  /*************************/
  UiCtrlSdlSurface::UiCtrlSdlSurface (sdl::Surface s) : surface (s) {}
  SDL_Rect
  UiCtrlSdlSurface::GetMinimumSize () const
  {
    SDL_Rect r{ 0 };
    if (surface) {
      r.w = surface.ptr->w;
      r.h = surface.ptr->h;
    }
    return r;
  }

  SDL_Rect
  UiCtrlSdlSurface::RenderSurface (sdl::Surface dst, SDL_Rect reqRect) const
  {
    SDL_Rect minRect = GetMinimumSize ();
    SDL_Rect dstRect = reqRect;
    if (surface) {
      if (!(flags & UI_GROW_H)) {
        dstRect.w = minRect.w;
      }
      if (!(flags & UI_GROW_V)) {
        dstRect.h = minRect.h;
      }
      if (flags & (UI_GROW_H | UI_GROW_V)) {
        surface.BlitTo (dst, &dstRect, &minRect);
      } else {
        surface.BlitScaledTo (dst, &dstRect, &minRect);
      }
    }
    return dstRect;
  }
}
/** ### UiCtrlSdlSurfaceColor **/
namespace hd {
  UiCtrlSdlSurfaceColor::UiCtrlSdlSurfaceColor (SDL_Color c) : color (c) {}

  SDL_Rect
  UiCtrlSdlSurfaceColor::GetMinimumSize () const
  {
    return SDL_Rect{ 0 };
  }

  SDL_Rect
  UiCtrlSdlSurfaceColor::RenderSurface (sdl::Surface dst,
                                        SDL_Rect dstRect) const
  {
    if (dst) {
      dst.FillRect (dstRect, dst.MapRGBA (color));
    }
    return dstRect;
  }
}