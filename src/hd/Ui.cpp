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

namespace hd {
  const int UI_FOCUSED_TARGET = 0x01, //
      UI_FOCUSED = 0x02,              //
      UI_CAN_FOCUS = 0x04,            //
      UI_IS_TAB_STOP = 0x08,          //
      UI_GROW_V = 0x0F,               //
      UI_GROW_H = 0x10;
  SDL_Rect
  UiComposition::GetMinimumSize () const
  {
    Rect r{ 0 };
    for (std::pair<int, iUiSurface::s_ptr> p : elements) {
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
    for (std::pair<int, iUiSurface::s_ptr> p : elements) {

      c = c.Union (p.second->RenderSurface (s, r));
    }
    return c;
  }

  int
  UiComposition::Append (iUiSurface::s_ptr child)
  {
    int ID = ++NextID;
    elements[ID] = child;
    return ID;
  }

  UiWindowSurface::UiWindowSurface (sdl::Window w) : window (w)
  {
    printf ("Connecting UiWindowSurface events.\n");
    pipeHandle = window.Event ().On (event.pipe);
    event.Close.Void.Once (DoClose);
  }
  UiWindowSurface::~UiWindowSurface ()
  {
    printf ("Disconnecting UiWindowSurface events.\n");
    window.Event ().Delete (pipeHandle);
  }
  SDL_Rect
  UiWindowSurface::GetMinimumSize () const
  {
    SDL_Rect r;
    if (window) {
      window.GetPosition (&r.x, &r.y);
      window.GetSize (&r.w, &r.h);
    }
    return r;
  }
  SDL_Rect
  UiWindowSurface::RenderSurface (sdl::Surface s, SDL_Rect r) const
  {
    return root.RenderSurface (s, r);
  }
  SDL_Rect
  UiWindowSurface::RenderSurface () const
  {
    SDL_Rect r = GetMinimumSize ();
    r.x = r.y = 0;
    if (window) {
      return RenderSurface (window.GetSurface (), r);
    }
    return r;
  }
  /**********************/
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
    SDL_Rect dstRect{ 0 }, minRect = GetMinimumSize ();
    if (font) {
      sdl::Surface tmp = font.RenderBlendedText (text, color);
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
  /*************************/
  UiViewSurface::UiViewSurface (sdl::Surface s) : surface (s) {}
  SDL_Rect
  UiViewSurface::GetMinimumSize () const
  {
    SDL_Rect r{ 0 };
    if (surface) {
      r.w = surface.ptr->w;
      r.h = surface.ptr->h;
    }
    return r;
  }

  SDL_Rect
  UiViewSurface::RenderSurface (sdl::Surface dst, SDL_Rect reqRect) const
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
  /**************************************/
  UiSurfaceColor::UiSurfaceColor (SDL_Color c) : color (c) {}

  SDL_Rect
  UiSurfaceColor::GetMinimumSize () const
  {
    return SDL_Rect{ 0 };
  }

  SDL_Rect
  UiSurfaceColor::RenderSurface (sdl::Surface dst, SDL_Rect dstRect) const
  {
    if (dst) {
      dst.FillRect (dstRect, dst.MapRGBA (color));
    }
    return dstRect;
  }
}