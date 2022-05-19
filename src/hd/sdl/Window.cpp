/**
 * @file Window.cpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-14
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
#include "hd/sdl/Window.hpp"
#include "hd/Debug.hpp"
#include "hd/Error.hpp"
#include "hd/sdl/events.hpp"
namespace hd::sdl {

  Surface
  Window::GetSurface () const
  {
    SDL_Surface *aSurface = SDL_GetWindowSurface (*this);
    return Surface::Create (aSurface, false);
  }
  bool
  Window::UpdateSurface () const
  {
    assert (*this);
    return SDL_UpdateWindowSurface (*this) == 0;
  }
  /** Defines the shape of the next window if no shape is specified. **/
  SDL_Rect Window::NextRect
      = { SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 320, 240 };
  const char *Window::NextTitle = "hd version";
  /**
   * @brief Controls default __additional__ flags for the next window created.
   * @note SDL_WINDOW_OPENGL is always enabled.
   */
  int Window::NextFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
  /**
   * @brief Create an `sdl::Window` from an existing `SDL_Window*` pointer
   * @param aWindow the existing `SDL_Window` pointer
   * @param aFree if true then `SDL_DestroyWindow` will be called when the
   * shared pointer is finally dereferenced.
   * @todo define operation around `NULL` pointer assignment. It is allowed for
   * now but could cause segfaults.
   * @return Window
   */
  Window
  Window::Create (SDL_Window *aWindow, bool aFree)
  {
    s_ptr created (
        aWindow, aFree ? SDL_DestroyWindow : [] (SDL_Window *p) {});
    Window window = created;
    return created;
  }
  /**
   * Calls `SDL_CreateWindow(char*,int,int,int,int,Uint32)` and makes it into
   * `std::shared_ptr` that will call `SDL_DestroyWindow`
   * @param aTitle to be set on new window
   * @param aRect position and size
   * @param aFlags ADDITIONAL flags.
   * @note aFlags is always set with `SDL_WINDOW_OPENGL`
   * @return smart pointer to and instance of this class managing a window.
   * @return NULL on failure.
   */
  Window
  Window::Create (const char *aTitle, SDL_Rect *aRect, Uint32 aFlags)
  {
    assert (aRect != NULL);
    assert (aTitle != NULL);
    Engine::s_ptr engine = Engine::Get ();
    hdDebugCall ("{%d, %d, %d, %d}, \"%s\", 0x%x",
                 aRect->x,
                 aRect->y,
                 aRect->w,
                 aRect->h,
                 aTitle,
                 aFlags);
    SDL_Window *created = SDL_CreateWindow (
        aTitle, aRect->x, aRect->y, aRect->w, aRect->h, aFlags);
    if (created == NULL) {
      hdError ("Unable to create SDL Window because:\n %s", SDL_GetError ());
    }
    SDL_GLContext created_context = (created);
    if (created_context == NULL) {
      hdError ("Unable to create SDL GL Context because:\n %s",
               SDL_GetError ());
      SDL_DestroyWindow (created);
    }
    return s_ptr (created, SDL_DestroyWindow);
  }
  /**
   *
   **/
  Window
  Window::Create (int w, int h, const char *aTitle, Uint32 aFlags)
  {
    SDL_Rect aRect = { NextRect.x, NextRect.h, w, h };
    return Create (aTitle, &aRect, aFlags);
  }
  /** **/
  Window
  Window::Create (Uint32 aFlags)
  {
    return Create (NextTitle, &NextRect, aFlags);
  }
  Display
  Window::GetDisplay () const
  {
    return Display::Get (GetDisplayIndex ());
  }
  int
  Window::GetDisplayIndex () const
  {
    return SDL_GetWindowDisplayIndex (*this);
  }
  /** **/
  WindowDispatch &
  Window::Event () const
  {
    return events.Windows[Id ()];
  }

  /**
   * @brief
   *
   * @return Uint32
   */
  Uint32
  Window::Id () const
  {
    Uint32 id = SDL_GetWindowID (*this);
    if (id == 0) {
      hdError ("Failed to get Window Id %s", SDL_GetError ());
    }
    return id;
  }

  void
  Window::Swap () const
  {
    SDL_GL_SwapWindow (*this);
  }

  bool
  Window::SetSwapInterval (int aInterval)
  {
    return SDL_GL_SetSwapInterval (aInterval) == 0;
  }

  bool
  Window::MakeCurrent (SDL_GLContext aContext) const
  {
    if (SDL_GL_MakeCurrent (*this, aContext) != 0) {
      hdError ("Unable to make Window/GLContext current: %s", SDL_GetError ());
      return false;
    }
    return true;
  }
  void
  Window::GetDrawableSize (int *w, int *h) const
  {
    SDL_GL_GetDrawableSize (*this, w, h);
  }
  void
  Window::Hide () const
  {
    SDL_HideWindow (*this);
  }
  void
  Window::Show () const
  {
    SDL_ShowWindow (*this);
  }
  void
  Window::Raise () const
  {
    SDL_RaiseWindow (*this);
  }
  void
  Window::Maximize () const
  {
    SDL_MaximizeWindow (*this);
  }
  void
  Window::Minimize () const
  {
    SDL_MinimizeWindow (*this);
  }
  void
  Window::Restore () const
  {
    SDL_RestoreWindow (*this);
  }
  void
  Window::SetBordered (bool aBool) const
  {
    SDL_bool setting = (aBool ? SDL_TRUE : SDL_FALSE);
    SDL_SetWindowBordered (*this, setting);
  }
  void *
  Window::SetData (const char *aName, void *aData) const
  {
    return SDL_SetWindowData (*this, aName, aData);
  }
  void *
  Window::GetData (const char *aName) const
  {
    return SDL_GetWindowData (*this, aName);
  }
  bool
  Window::SetFullscreen (Uint32 aFlags) const
  {
    if (SDL_SetWindowFullscreen (*this, aFlags) != 0) {
      hdError ("Unable to set window fullscreen because: %s", SDL_GetError ());
      return false;
    }
    return true;
  }
  void
  Window::SetGrab (bool aGrab) const
  {
    SDL_bool setting = (aGrab ? SDL_TRUE : SDL_FALSE);
    SDL_SetWindowGrab (*this, setting);
  }
  void
  Window::SetKeyboardGrab (bool aGrab) const
  {
#if SDL_VERSION_ATLEAST(2, 0, 16)
    SDL_bool setting = (aGrab ? SDL_TRUE : SDL_FALSE);
    SDL_SetWindowKeyboardGrab (*this, setting);
#else
    hdError ("SDL Version is less than 2.0.16");
#endif
  }

  void
  Window::SetTitle (std::string aTitle) const
  {
    SDL_SetWindowTitle (*this, aTitle.c_str ());
  }

  std::string
  Window::GetTitle () const
  {
    const char *title = SDL_GetWindowTitle (*this);
    return std::string (title);
  }

  bool
  Window::GetWmInfo (SDL_SysWMinfo *aInfo) const
  {
    if (SDL_GetWindowWMInfo (*this, aInfo) != SDL_TRUE) {
      hdError ("Unable to get System Window Manager Info: %s",
               SDL_GetError ());
      return false;
    }
    return true;
  }

  void
  Window::SetIcon (SDL_Surface *aSurface) const
  {
    SDL_SetWindowIcon (*this, aSurface);
  }
  void
  Window::SetIcon (Surface::s_ptr aSurface) const
  {
    SetIcon (aSurface);
  }
  void
  Window::SetIcon (std::filesystem::path aPath) const
  {
    SetIcon (Surface::Load (aPath).ptr);
  }
  void
  Window::SetSize (int w, int h) const
  {
    SDL_SetWindowSize (*this, w, h);
  }
  void
  Window::GetSize (int *w, int *h) const
  {
    SDL_GetWindowSize (*this, w, h);
  }
  void
  Window::SetMaximumSize (int w, int h) const
  {
    SDL_SetWindowMaximumSize (*this, w, h);
  }
  void
  Window::GetMaximumSize (int *w, int *h) const
  {
    SDL_GetWindowMaximumSize (*this, w, h);
  }
  void
  Window::SetMinimumSize (int w, int h) const
  {
    SDL_SetWindowMinimumSize (*this, w, h);
  }
  void
  Window::GetMinimumSize (int *w, int *h) const
  {
    SDL_GetWindowMinimumSize (*this, w, h);
  }
  bool
  Window::SetModalFor (SDL_Window *aWindow) const
  {
    if (SDL_SetWindowModalFor (aWindow, *this) != 0) {
      hdError ("Unable to set Window as Modal: %s", SDL_GetError ());
      return false;
    }
    return true;
  }
  bool
  Window::SetOpacity (float aOpacity) const
  {
    if (SDL_SetWindowOpacity (*this, aOpacity) != 0) {
      hdError ("Unable to set Window Opacity: %s", SDL_GetError ());
      return false;
    }
    return true;
  }
  float
  Window::GetOpacity () const
  {
    float value;
    if (SDL_GetWindowOpacity (*this, &value) != 0) {
      hdError ("Unable to get Window Opacity: %s", SDL_GetError ());
    }
    return value;
  }
  void
  Window::SetPosition (int x, int y) const
  {
    SDL_SetWindowPosition (*this, x, y);
  }
  void
  Window::GetPosition (int *x, int *y) const
  {
    SDL_GetWindowPosition (*this, x, y);
  }
  void
  Window::SetResizable (bool aBool) const
  {
    SDL_bool setting = aBool ? SDL_TRUE : SDL_FALSE;
    SDL_SetWindowResizable (*this, setting);
  }
  Uint32
  Window::GetFlags () const
  {
    return SDL_GetWindowFlags (*this);
  }
/**
 * @brief Simply calls `Window::GetFlags()` and checks if `aFlag` is set.
 *
 */
#define CheckFor(aFlag) (GetFlags () & aFlag) != 0

  bool
  Window::IsFullscreen () const
  {
    return CheckFor (SDL_WINDOW_FULLSCREEN);
  }
  bool
  Window::IsFullscreenDesktop () const
  {
    return CheckFor (SDL_WINDOW_FULLSCREEN_DESKTOP);
  }
  bool
  Window::IsOpenGl () const
  {
    return CheckFor (SDL_WINDOW_OPENGL);
  }
  bool
  Window::IsShown () const
  {
    return CheckFor (SDL_WINDOW_SHOWN);
  }
  bool
  Window::IsHidden () const
  {
    return CheckFor (SDL_WINDOW_HIDDEN);
  }
  bool
  Window::IsBorderless () const
  {
    return CheckFor (SDL_WINDOW_BORDERLESS);
  }
  bool
  Window::HasBorder () const
  {
    return !IsBorderless ();
  }
  bool
  Window::IsResizable () const
  {
    return CheckFor (SDL_WINDOW_RESIZABLE);
  }
  bool
  Window::IsMinimized () const
  {
    return CheckFor (SDL_WINDOW_MINIMIZED);
  }
  bool
  Window::IsMaximized () const
  {
    return CheckFor (SDL_WINDOW_MAXIMIZED);
  }
  bool
  Window::HasInputGrab () const
  {
    return CheckFor (SDL_WINDOW_INPUT_GRABBED);
  }
  bool
  Window::HasInputFocus () const
  {
    return CheckFor (SDL_WINDOW_INPUT_FOCUS);
  }
  bool
  Window::HasMouseFocus () const
  {
    return CheckFor (SDL_WINDOW_MOUSE_FOCUS);
  }
  bool
  Window::IsForeign () const
  {
    return CheckFor (SDL_WINDOW_FOREIGN);
  }
  bool
  Window::IsHighDPI () const
  {
    return CheckFor (SDL_WINDOW_ALLOW_HIGHDPI);
  }
  bool
  Window::IsMouseCaptured () const
  {
    return CheckFor (SDL_WINDOW_MOUSE_CAPTURE);
  }
  bool
  Window::IsAlwaysOnTop () const
  {
    return CheckFor (SDL_WINDOW_ALWAYS_ON_TOP);
  }
  bool
  Window::IsSkipTaskbar () const
  {
    return CheckFor (SDL_WINDOW_SKIP_TASKBAR);
  }
  bool
  Window::IsUtility () const
  {
    return CheckFor (SDL_WINDOW_UTILITY);
  }
  bool
  Window::IsTooltip () const
  {
    return CheckFor (SDL_WINDOW_TOOLTIP);
  }
  bool
  Window::IsPopupMenu () const
  {
    return CheckFor (SDL_WINDOW_POPUP_MENU);
  }
}