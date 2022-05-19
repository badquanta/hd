/**
 * @file Window.hpp
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
#pragma once
#include "hd/sdl/Display.hpp"
#include "hd/sdl/Surface.hpp"
#include <SDL2/SDL_syswm.h>
namespace hd::sdl {

  /** future
   * @todo https://wiki.libsdl.org/SDL_SetWindowHitTest
   * @todo https://wiki.libsdl.org/SDL_SetWindowHitTest
   * @todo https://wiki.libsdl.org/SDL_IsScreenSaverEnabled
   **/
  class Window : public WRAP_PTR<SDL_Window>, public EngineComponent {
  public: // Static constructor methods
    using WRAP_PTR::WRAP_PTR;
    using WRAP_PTR::operator=;
    /** @brief
     * Given an existing SDL_Windowand SDL_GLContext pointer construct
     * a Window::s_ptr
     * @todo make this check the caches?
     ***/
    static Window Create (SDL_Window *, bool);
    /**
     * @brief
     *
     * @param aRect [SDL_Rect*] describing the position and size of the desired
     * window.
     * @param aTitle [const char*] used for the initial window title.
     * @param aFlags [Uint32] describing the window flags to use
     * @note Windows are always created with SDL_WINDOW_OPENGL flag set.
     * @return Window::s_ptr
     */
    static Window Create (const char *aTitle = Window::NextTitle,
                          SDL_Rect *aRect = &Window::NextRect,

                          Uint32 aFlags = Window::NextFlags);
    static Window Create (int aWidth,
                          int aHeight,
                          const char *aTitle = Window::NextTitle,
                          Uint32 aFlags = Window::NextFlags);
    static Window Create (Uint32 aFlags);

  public: // instance methods:
    /** @deprecated gonna remove this
     * @note @see `hd::sdl::events.Windows` **/
    WindowDispatch &Event () const;
    /** Return the SDL Surface associated with this window. **/
    Surface GetSurface () const;
    bool UpdateSurface () const;
    void Swap () const;
    /** @see https://wiki.libsdl.org/SDL_GL_SetSwapInterval **/
    bool SetSwapInterval (int);
    /** @see https://wiki.libsdl.org/SDL_GL_MakeCurrent **/
    bool MakeCurrent (SDL_GLContext) const;

  public: // Window attribute accessors
    Display GetDisplay () const;
    int GetDisplayIndex () const;
    /** Return the window Identifier integer. **/
    Uint32 Id () const;
    /** @see https://wiki.libsdl.org/SDL_GL_GetDrawableSize **/
    void GetDrawableSize (int *, int *) const;
    /** @see https://wiki.libsdl.org/SDL_HideWindow **/
    void Hide () const;
    /** @see https://wiki.libsdl.org/SDL_ShowWindow **/
    void Show () const;
    /** @see https://wiki.libsdl.org/SDL_RaiseWindow **/
    void Raise () const;
    /** @see https://wiki.libsdl.org/SDL_MaximizeWindow **/
    void Maximize () const;
    /** @see https://wiki.libsdl.org/SDL_RestoreWindow **/
    void Restore () const;
    /** @see https://wiki.libsdl.org/SDL_MinimizeWindow **/
    void Minimize () const;
    /** @see https://wiki.libsdl.org/SDL_SetWindowBordered **/
    void SetBordered (bool) const;
    /** @see https://wiki.libsdl.org/SDL_SetWindowData **/
    void *SetData (const char *, void *) const;
    /** @see https://wiki.libsdl.org/SDL_GetWindowData **/
    void *GetData (const char *) const;
    /** future @todo https://wiki.libsdl.org/SDL_SetWindowDisplayMode**/
    /** @see https://wiki.libsdl.org/SDL_SetWindowFullscreen **/
    bool SetFullscreen (Uint32 aFlags = SDL_WINDOW_FULLSCREEN_DESKTOP) const;
    /** future @see https://wiki.libsdl.org/SDL_SetWindowGammaRamp**/
    /** @see https://wiki.libsdl.org/SDL_SetWindowGrab **/
    void SetGrab (bool aGrab) const;
    /** @see https://wiki.libsdl.org/SDL_SetWindowKeyboardGrab **/
    void SetKeyboardGrab (bool) const;
    /** @see https://wiki.libsdl.org/SDL_SetWindowTitle **/
    void SetTitle (std::string) const;
    /** @see https://wiki.libsdl.org/SDL_GetWindowWMInfo **/
    bool GetWmInfo (SDL_SysWMinfo *) const;
    /** @see https://wiki.libsdl.org/SDL_GetWindowTitle **/
    std::string GetTitle () const;
    /** @see https://wiki.libsdl.org/SDL_SetWindowIcon **/
    void SetIcon (SDL_Surface *) const;
    void SetIcon (Surface::s_ptr) const;
    void SetIcon (std::filesystem::path) const;
    /** maybe future @todo https://wiki.libsdl.org/SDL_SetWindowInputFocus **/
    /** @see https://wiki.libsdl.org/SDL_SetWindowSize **/
    void SetSize (int, int) const;
    /** @see https://wiki.libsdl.org/SDL_GetWindowSize **/
    void GetSize (int *, int *) const;
    /** @see https://wiki.libsdl.org/SDL_SetWindowMaximumSize **/
    void SetMaximumSize (int, int) const;
    /** @see https://wiki.libsdl.org/SDL_GetWindowMaximumSize **/
    void GetMaximumSize (int *, int *) const;
    /** @see https://wiki.libsdl.org/SDL_SetWindowMinimumSize **/
    void SetMinimumSize (int, int) const;
    /** @see https://wiki.libsdl.org/SDL_GetWindowMinimumSize **/
    void GetMinimumSize (int *, int *) const;
    /** @see https://wiki.libsdl.org/SDL_SetWindowModalFor **/
    bool SetModalFor (SDL_Window *) const;
    /** @see https://wiki.libsdl.org/SDL_SetWindowOpacity **/
    bool SetOpacity (float) const;
    /** @see https://wiki.libsdl.org/SDL_GetWindowOpacity **/
    float GetOpacity () const;
    /** @see https://wiki.libsdl.org/SDL_SetWindowPosition **/
    void SetPosition (int, int) const;
    /** @see https://wiki.libsdl.org/SDL_GetWindowPosition **/
    void GetPosition (int *, int *) const;
    /** @see https://wiki.libsdl.org/SDL_SetWindowResizable **/
    void SetResizable (bool) const;
    /** @see https://wiki.libsdl.org/SDL_GetWindowFlags **/
    Uint32 GetFlags () const;
    /** @brief Check Window flags for `SDL_WINDOW_FULLSCREEN`
     * @return true if is set **/
    bool IsFullscreen () const;
    /** @brief Check Window flags for `SDL_WINDOW_FULLSCREEN_DESKTOP`
     * @return true if is set **/
    bool IsFullscreenDesktop () const;
    /** @brief Check Window flags for `SDL_WINDOW_FULLSCREEN`
     * @return true if is set **/
    bool IsOpenGl () const;
    bool IsShown () const;
    bool IsHidden () const;
    bool IsBorderless () const;
    bool HasBorder () const;
    /** Check if the window is resizable. **/
    bool IsResizable () const;
    bool IsMinimized () const;
    bool IsMaximized () const;
    bool HasInputGrab () const;
    bool HasInputFocus () const;
    bool HasMouseFocus () const;
    bool IsForeign () const;
    bool IsHighDPI () const;
    bool IsMouseCaptured () const;
    bool IsAlwaysOnTop () const;
    bool IsSkipTaskbar () const;
    bool IsUtility () const;
    bool IsTooltip () const;
    bool IsPopupMenu () const;

  public:
    /** `Create` uses this position and size if none are defined. **/
    static SDL_Rect NextRect;
    /** `Create` uses this title if none is defined. **/
    static const char *NextTitle;
    /** `Create()` uses these flags if none are defined. **/
    static int NextFlags;
    /** Destroy a window and its associated OpenGL Context.**/
  };
}