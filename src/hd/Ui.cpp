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
#include "hd/Debug.hpp"
#include "hd/Error.hpp"

/**
 * ### UI Flags
 *
 * @todo more detailed documentation about the ui flags.
 */
namespace hd {
  const int UI_FOCUSED_TARGET = 0x01, //
      UI_FOCUSED = 0x02,              //
      UI_CAN_FOCUS = 0x04,            //
      UI_IS_TAB_STOP = 0x08,          //
      UI_GROW_V = 0x0F,               //
      UI_GROW_H = 0x10;
}
/** ### UiViewSurfaceBase **/
namespace hd {
  UiViewSurfaceBase::flag_t UiViewSurfaceBase::ToggleFlags(flag_t toggleFlags){
    return flags ^= toggleFlags;
  }
  UiViewSurfaceBase::flag_t UiViewSurfaceBase::SetFlags(flag_t setFlags){
    return flags |= setFlags;
  }
  UiViewSurfaceBase::flag_t UiViewSurfaceBase::ClearFlags(flag_t clearFlags){
    return flags &= ~clearFlags;
  }
  bool UiViewSurfaceBase::CheckAllFlags(flag_t checkFlags) {
    return (flags & checkFlags) == checkFlags;
  }
  bool UiViewSurfaceBase::CheckAnyFlags(flag_t anyFlags){
    return (flags & anyFlags) != 0;
  }
}
/** ### UiPointPair **/
namespace hd {
  UiPointPair::UiPointPair ()
      : std::pair<UiPoint, UiPoint>{ { 0, 0 }, { 0, 0 } }
  {
  }
  UiPointPair::UiPointPair (const SDL_Rect &r)
      : std::pair<UiPoint, UiPoint>{ UiPoint{ r.x, r.y },
                                     UiPoint{ r.x + r.w, r.y + r.h } }
  {
  }
  UiPointPair::operator SDL_Rect ()
  {
    return SDL_Rect{
      first.x, first.y, second.x - first.x, second.y - first.y
    };
  }
  UiPointPair &
  UiPointPair::operator= (SDL_Rect r)
  {
    first.x = r.x;
    first.y = r.y;
    second.x = r.x + r.w;
    second.y = r.y + r.h;
    return (*this);
  }
  UiPointPair
  UiPointPair::operator+ (UiPoint a)
  {
    return { a + first, a + second };
  }
  UiPointPair
  UiPointPair::Union (const UiPointPair &aPair)
  {
    return { {
                 (aPair.first.x < first.x) ? aPair.first.x : first.x,
                 (aPair.first.y < first.y) ? aPair.first.y : first.y,
             },
             {
                 (aPair.second.x > second.x) ? aPair.second.x : second.x,
                 (aPair.second.x > second.x) ? aPair.second.x : second.x,
             } };
  }
}
/** ### UiComposition **/
namespace hd {

  UiPointPair
  UiViewSurfaces::GetMinimumSize () const
  {
    UiPointPair r;
    for (std::pair<int, UiViewSurfaceBase::s_ptr> pair : elements) {
      if (pair.second) {
        r = r.Union (pair.second->GetMinimumSize ());
      }
    }
    return r;
  }

  UiPointPair
  UiViewSurfaces::RenderSurface (sdl::Surface s, UiPointPair r) const
  {
    UiPointPair c;
    for (std::pair<int, UiViewSurfaceBase::s_ptr> p : elements) {
      c = c.Union (p.second->RenderSurface (s, r));
    }
    return c;
  }

  int
  UiViewSurfaces::Append (UiViewSurfaceBase::s_ptr child)
  {
    int ID = ++NextID;
    elements[ID] = child;
    return ID;
  }

  bool
  UiViewSurfaces::Delete (int aID)
  {
    return (elements.erase (aID) > 0);
  }
}
/** ### UiViewPositionedSurfaces **/
namespace hd {
  int
  UiViewPositionedSurfaces::Append (UiViewSurfaceBase::s_ptr aPtr)
  {
    return Append (aPtr, aPtr->GetMinimumSize ());
  }
  int
  UiViewPositionedSurfaces::Append (UiViewSurfaceBase::s_ptr aPtr,
                                    UiPoint aPoint)
  {
    return Append (aPtr, aPtr->GetMinimumSize () + aPoint);
  }
  int
  UiViewPositionedSurfaces::Append (UiViewSurfaceBase::s_ptr aPtr,
                                    UiPointPair aRct)
  {
    int ID = UiViewSurfaces::Append (aPtr);
    positions[ID] = aRct;
    return ID;
  }
  bool
  UiViewPositionedSurfaces::Delete (int aID)
  {
    return (positions.erase (aID) | Delete (aID));
  }
  UiPointPair
  UiViewPositionedSurfaces::GetMinimumSize () const
  {
    UiPointPair u;
    for (std::pair pair : elements) {
      int ID = pair.first;
      // UiPointPair u2 = pair.second->GetMinimumSize ();
      UiPointPair pos;
      if (positions.find (ID) != positions.end ()) {
        pos = positions.at (ID);
      } else {
        pos = pair.second->GetMinimumSize ();
      }
      u = u.Union (pos);
    }
    return u;
  }
  UiPointPair
  UiViewPositionedSurfaces::RenderSurface (sdl::Surface dst,
                                           UiPointPair reqRect) const
  {
    UiPointPair c;
    for (std::pair pair : elements) {
      auto ID = pair.first;
      auto ctrlSurf = pair.second;
      UiPointPair position;
      if (positions.find (ID) != positions.end ()) {
        position = positions.at (ID);
      } else {
        position = ctrlSurf->GetMinimumSize ();
      }

      c = c.Union (ctrlSurf->RenderSurface (dst, position));
    }
    return c;
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
  UiPointPair
  UiCtrlSdlWindowSurface::GetMinimumSize () const
  {
    SDL_Rect r;
    if (window) {
      window.GetPosition (&r.x, &r.y);
      window.GetSize (&r.w, &r.h);
    }
    return r;
  }
  UiPointPair
  UiCtrlSdlWindowSurface::RenderSurface (sdl::Surface s, UiPointPair r) const
  {
    return root.RenderSurface (s, r);
  }
  UiPointPair
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

  UiPointPair
  UiViewText::GetMinimumSize () const
  {
    SDL_Rect r{ 0 };
    if (font) {
      font.SizeUTF8 (text, &r.w, &r.h);
    }
    return r;
  }
  UiPointPair
  UiViewText::RenderSurface (sdl::Surface dst, UiPointPair reqRect) const
  {
    if (!dst) {
      hdError ("destination surface missing");
    }
    SDL_Rect dstRect = reqRect, minRect = GetMinimumSize ();
    if (font) {
      sdl::Surface tmp
          = font.RenderBlendedTextWrapped (text, color, dstRect.w);
      assert (tmp);
      minRect = { 0, 0, tmp.ptr->w, tmp.ptr->h };

      if ((flags & (UI_GROW_H))) {
        dstRect.w = minRect.w;
      }
      if (!(flags & (UI_GROW_V))) {
        dstRect.h = minRect.h;
      }

      if (flags & (UI_GROW_H | UI_GROW_V)) {
        tmp.BlitScaledTo (dst, &dstRect, &minRect);
      } else {
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
  UiPointPair
  UiCtrlSdlSurface::GetMinimumSize () const
  {
    SDL_Rect r{ 0 };
    if (surface) {
      r.w = surface.ptr->w;
      r.h = surface.ptr->h;
    }
    return r;
  }

  UiPointPair
  UiCtrlSdlSurface::RenderSurface (sdl::Surface dst, UiPointPair reqRect) const
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
        hdDebug ("Dynamic Blit %b",flags);
        surface.BlitScaledTo (dst, &dstRect, &minRect);
      } else {
        surface.BlitTo (dst, &dstRect, &minRect);

      }
    }
    return dstRect;
  }
}
/** ### UiCtrlSdlSurfaceColor **/
namespace hd {
  UiCtrlSdlSurfaceColor::UiCtrlSdlSurfaceColor (SDL_Color c) : color (c) {}

  UiPointPair
  UiCtrlSdlSurfaceColor::GetMinimumSize () const
  {
    return SDL_Rect{ 0 };
  }

  UiPointPair
  UiCtrlSdlSurfaceColor::RenderSurface (sdl::Surface dst,
                                        UiPointPair dstRect) const
  {
    if (dst) {
      dst.FillRect (dstRect, dst.MapRGBA (color));
    }
    return dstRect;
  }
}