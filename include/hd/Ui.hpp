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
#include "hd/Dispatch.hpp"
#include "hd/Rect.hpp"
#include "hd/sdl/Font.hpp"
#include "hd/sdl/Surface.hpp"
#include "hd/sdl/Window.hpp"
namespace hd {
  using point = glm::ivec2;
  template <class Return, class... Signature>
  Return
  ReturnNull (Signature... Args)
  {
    return Return (NULL);
  }
  extern const int UI_FOCUSED_TARGET, UI_FOCUSED, UI_CAN_FOCUS, UI_IS_TAB_STOP,
      UI_GROW_H, UI_GROW_V, UI_MOUSE_IN, UI_MOUSE_OVER, UI_MOUSE_OUT;

  class iUiSurface {
  public:
    typedef std::shared_ptr<iUiSurface> s_ptr;
    unsigned int flags = 0;
    virtual SDL_Rect GetMinimumSize () const = 0;
    virtual SDL_Rect RenderSurface (sdl::Surface, SDL_Rect) const = 0;
  };
  class UiComposition : public iUiSurface {
  public:
    int NextID = 0;

    std::map<int, iUiSurface::s_ptr> elements;
    virtual SDL_Rect GetMinimumSize () const override;
    virtual SDL_Rect RenderSurface (sdl::Surface, SDL_Rect) const override;
    int Append (s_ptr);
  };
  class UiWindowSurface : public iUiSurface {
  public:
    sdl::Window window;
    sdl::WindowDispatch event;
    sdl::WindowDispatch::Handle pipeHandle;
    UiComposition root;
    UiWindowSurface (sdl::Window);
    ~UiWindowSurface ();
    std::function<void ()> DoClose = [this] () {
      Engine::Get ()->step.Void.Once ([this] () { window.ptr = nullptr; });
    };
    virtual SDL_Rect GetMinimumSize () const override;
    virtual SDL_Rect RenderSurface (sdl::Surface, SDL_Rect) const override;
    virtual SDL_Rect RenderSurface () const;
  };
  class UiViewText : public iUiSurface {
  public:
    sdl::Font font;
    SDL_Color color; /** @todo wrap SDL_Color **/
    std::string text;
    UiViewText (sdl::Font f,
                std::string s = "",
                SDL_Color c = { 255, 255, 255, 255 });
    virtual SDL_Rect GetMinimumSize () const override;
    virtual SDL_Rect RenderSurface (sdl::Surface, SDL_Rect) const override;
  };
  class UiViewSurface : public iUiSurface {
  public:
    sdl::Surface surface;
    UiViewSurface (sdl::Surface);
    virtual SDL_Rect GetMinimumSize () const override;
    virtual SDL_Rect RenderSurface (sdl::Surface, SDL_Rect) const override;
  };
  class UiSurfaceColor : public iUiSurface {
  public:
    SDL_Color color;
    UiSurfaceColor (SDL_Color);
    virtual SDL_Rect GetMinimumSize () const override;
    virtual SDL_Rect RenderSurface (sdl::Surface, SDL_Rect) const override;
  };
}