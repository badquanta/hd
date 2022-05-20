/**
 * @file Ui.hpp
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
#pragma once
#include "glm/vec2.hpp"
#include "glm/matrix.hpp"
#include "glm/gtx/transform.hpp"
#include "hd/Dispatch.hpp"
#include "hd/Rect.hpp"
#include "hd/sdl/Font.hpp"
#include "hd/sdl/Surface.hpp"
#include "hd/sdl/Window.hpp"
namespace hd {
  using point = glm::ivec2;
  typedef glm::ivec2 UiPoint;
  //typedef std::pair<UiPoint, UiPoint> UiPointPair;
  class UiPointPair: public std::pair<UiPoint, UiPoint> {
    public:
      using std::pair<UiPoint,UiPoint>::pair;
      UiPointPair ();
      UiPointPair (const SDL_Rect &);
      operator SDL_Rect ();
      UiPointPair& operator= (SDL_Rect);
      UiPointPair Union (const UiPointPair &);
      UiPointPair operator+ (UiPoint);
  };
  template <class Return, class... Signature>
  Return
  ReturnNull (Signature... Args)
  {
    return Return (NULL);
  }
  extern const int UI_FOCUSED_TARGET, UI_FOCUSED, UI_CAN_FOCUS, UI_IS_TAB_STOP,
      UI_GROW_H, UI_GROW_V, UI_MOUSE_IN, UI_MOUSE_OVER, UI_MOUSE_OUT;

  class UiViewSurfaceBase {
  public:
    typedef std::shared_ptr<UiViewSurfaceBase> s_ptr;
    unsigned int flags = 0;
    virtual UiPointPair GetMinimumSize () const = 0;
    virtual UiPointPair RenderSurface (sdl::Surface, UiPointPair) const = 0;
  };
  class UiViewSurfaces : public UiViewSurfaceBase {
  public:
    int NextID = 0;
    std::map<int, UiViewSurfaceBase::s_ptr> elements;
    virtual UiPointPair GetMinimumSize () const override;
    virtual UiPointPair RenderSurface (sdl::Surface, UiPointPair) const override;
    virtual int Append (s_ptr);
    virtual bool Delete (int);
  };
  class UiSdlPositionedSurfaceComposition : public UiViewSurfaces {
    public:
      typedef std::shared_ptr<UiSdlPositionedSurfaceComposition> s_ptr;
      std::map<int, UiPointPair> positions;
      virtual int Append (UiViewSurfaceBase::s_ptr, UiPointPair);
      virtual int Append (UiViewSurfaceBase::s_ptr) override;
      virtual int Append (UiViewSurfaceBase::s_ptr, UiPoint);
      virtual bool Delete (int) override;
      virtual UiPointPair GetMinimumSize () const override;
      virtual UiPointPair RenderSurface (sdl::Surface,
                                         UiPointPair) const override;
  };
  class UiCtrlSdlWindowSurface : public UiViewSurfaceBase {
  public:
    typedef std::shared_ptr<UiCtrlSdlWindowSurface> s_ptr;
    sdl::Window window;
    sdl::WindowDispatch event;
    sdl::WindowDispatch::Handle eventPipeHandle;
    IntDispatch output;
    IntDispatch::Handle outputPipeHandle;
    UiViewSurfaces root;
    UiCtrlSdlWindowSurface (sdl::Window);
    ~UiCtrlSdlWindowSurface ();
    std::function<void ()> DoClose = [this] () {
      Engine::Get ()->step.Void.Once ([this] () { window.ptr = nullptr; });
    };
    std::function<void ()> DoRender = [this] () {
      if (window) {
        this->RenderSurface ();
        window.UpdateSurface ();
      }
    };
    virtual UiPointPair GetMinimumSize () const override;
    virtual UiPointPair RenderSurface (sdl::Surface, UiPointPair) const override;
    virtual UiPointPair RenderSurface () const;
  };
  class UiViewText : public UiViewSurfaceBase {
  public:
    sdl::Font font;
    SDL_Color color; /** @todo wrap SDL_Color **/
    std::string text;
    UiViewText (sdl::Font f,
                std::string s = "",
                SDL_Color c = { 255, 255, 255, 255 });
    virtual UiPointPair GetMinimumSize () const override;
    virtual UiPointPair RenderSurface (sdl::Surface, UiPointPair) const override;
  };
  class UiCtrlSdlSurface : public UiViewSurfaceBase {
  public:
    typedef std::shared_ptr<UiCtrlSdlSurface> s_ptr;
    sdl::Surface surface;
    UiCtrlSdlSurface (sdl::Surface);
    virtual UiPointPair GetMinimumSize () const override;
    virtual UiPointPair RenderSurface (sdl::Surface, UiPointPair) const override;
  };
  class UiCtrlSdlSurfaceColor : public UiViewSurfaceBase {
  public:
    SDL_Color color;
    UiCtrlSdlSurfaceColor (SDL_Color);
    virtual UiPointPair GetMinimumSize () const override;
    virtual UiPointPair RenderSurface (sdl::Surface, UiPointPair) const override;
  };
}