/*
 * holodeck - maybe it will be a game or a game engine
 * Copyright (C) 2022 Jón Davíð Sawyer (badquanta@gmail.com)
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
 */
#pragma once
#include "hd/EngineComponent.hpp"
#include "hd/Shared.hpp"
#include "hd/evt/VoidDispatch.hpp"
#include "hd/evt/WindowDispatch.hpp"
#include <memory>
namespace hd {
  /** future
   * @todo https://wiki.libsdl.org/SDL_SetWindowHitTest
   * @todo https://wiki.libsdl.org/SDL_SetWindowHitTest
   * @todo https://wiki.libsdl.org/SDL_IsScreenSaverEnabled
   **/
  class Window: public EngineComponent{
  public:
    /** Smart pointer reference. **/
    typedef std::shared_ptr<Window> s_ptr;
    /** `Create` uses this position and size if none are defined. **/
    static SDL_Rect NextRect;
    /** `Create` uses this title if none is defined. **/
    static const char *NextTitle;
    /** `Create()` uses these flags if none are defined. **/
    static SDL_WindowFlags NextFlags;
    /** Destroy a window and its associated OpenGL Context.**/
    virtual ~Window ();
  public: // events
    /** Automatically (dis)connected to/from engine output event. **/
    evt::IntDispatch output;
    /** Automatically (dis)connected to/from engine input event. **/
    evt::WindowDispatch input;
  private: // handles used to disconnect on destruct.
    evt::SDL_EventDispatch::Handle onHandle;
    evt::IntDispatch::Handle outputHandle;
  public: // Create window pointer utilities.
    /** Given an existing SDL_Windowand SDL_GLContext pointer construct
     * a Window::s_ptr
     * @todo make this check the caches?
     ***/
    static Window::s_ptr Create (SDL_Window *, SDL_GLContext);
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
    static Window::s_ptr Create (const char *aTitle = Window::NextTitle,
                               SDL_Rect *aRect = &Window::NextRect,

                               Uint32 aFlags = Window::NextFlags);
    static Window::s_ptr Create (int aWidth,
                               int aHeight,
                               const char *aTitle = Window::NextTitle,
                               Uint32 aFlags = Window::NextFlags);
  public: // Window state modifiers:
    void Swap ();
    /** @see https://wiki.libsdl.org/SDL_GL_SetSwapInterval **/
    bool SetSwapInterval (int);
    /** @see https://wiki.libsdl.org/SDL_GL_MakeCurrent **/
    bool MakeCurrent ();
  public: // Window attribute accessors
    /** Return the window Identifier integer. **/
    Uint32 Id ();
    /** @see https://wiki.libsdl.org/SDL_GL_GetDrawableSize **/
    void GetDrawableSize (int *, int *);
    /** @see https://wiki.libsdl.org/SDL_HideWindow **/
    void Hide ();
    /** @see https://wiki.libsdl.org/SDL_ShowWindow **/
    void Show ();
    /** @see https://wiki.libsdl.org/SDL_RaiseWindow **/
    void Raise ();
    /** @see https://wiki.libsdl.org/SDL_MaximizeWindow **/
    void Maximize ();
    /** @see https://wiki.libsdl.org/SDL_RestoreWindow **/
    void Restore ();
    /** @see https://wiki.libsdl.org/SDL_MinimizeWindow **/
    void Minimize ();
    /** @see https://wiki.libsdl.org/SDL_SetWindowBordered **/
    void SetBordered (bool);
    /** @see https://wiki.libsdl.org/SDL_SetWindowData **/
    void *SetData (const char *, void *);
    /** @see https://wiki.libsdl.org/SDL_GetWindowData **/
    void *GetData (const char *);
    /** future @todo https://wiki.libsdl.org/SDL_SetWindowDisplayMode**/
    /** @see https://wiki.libsdl.org/SDL_SetWindowFullscreen **/
    bool SetFullscreen (Uint32 aFlags = SDL_WINDOW_FULLSCREEN_DESKTOP);
    /** future @see https://wiki.libsdl.org/SDL_SetWindowGammaRamp**/
    /** @see https://wiki.libsdl.org/SDL_SetWindowGrab **/
    void SetGrab (bool aGrab);
    /** @see https://wiki.libsdl.org/SDL_SetWindowKeyboardGrab **/
    void SetKeyboardGrab (bool);
    /** @see https://wiki.libsdl.org/SDL_SetWindowTitle **/
    void SetTitle (std::string);
    /** @see https://wiki.libsdl.org/SDL_GetWindowWMInfo **/
    bool GetWmInfo (SDL_SysWMinfo *);
    /** @see https://wiki.libsdl.org/SDL_GetWindowTitle **/
    std::string GetTitle ();
    /** @see https://wiki.libsdl.org/SDL_SetWindowIcon **/
    void SetIcon (SDL_Surface *);
    void SetIcon (Shared::Surface);
    void SetIcon (std::filesystem::path);
    /** maybe future @todo https://wiki.libsdl.org/SDL_SetWindowInputFocus **/
    /** @see https://wiki.libsdl.org/SDL_SetWindowSize **/
    void SetSize (int, int);
    /** @see https://wiki.libsdl.org/SDL_GetWindowSize **/
    void GetSize (int *, int *);
    /** @see https://wiki.libsdl.org/SDL_SetWindowMaximumSize **/
    void SetMaximumSize (int, int);
    /** @see https://wiki.libsdl.org/SDL_GetWindowMaximumSize **/
    void GetMaximumSize (int *, int *);
    /** @see https://wiki.libsdl.org/SDL_SetWindowMinimumSize **/
    void SetMinimumSize (int, int);
    /** @see https://wiki.libsdl.org/SDL_GetWindowMinimumSize **/
    void GetMinimumSize (int *, int *);
    /** @see https://wiki.libsdl.org/SDL_SetWindowModalFor **/
    bool SetModalFor (SDL_Window *);
    bool SetModalFor (Window::s_ptr);
    /** @see https://wiki.libsdl.org/SDL_SetWindowOpacity **/
    bool SetOpacity (float);
    /** @see https://wiki.libsdl.org/SDL_GetWindowOpacity **/
    float GetOpacity ();
    /** @see https://wiki.libsdl.org/SDL_SetWindowPosition **/
    void SetPosition (int, int);
    /** @see https://wiki.libsdl.org/SDL_GetWindowPosition **/
    void GetPosition (int *, int *);
    /** @see https://wiki.libsdl.org/SDL_SetWindowResizable **/
    void SetResizable (bool);
    /** @see https://wiki.libsdl.org/SDL_GetWindowFlags **/
    Uint32 GetFlags ();
    /** @brief Check Window flags for `SDL_WINDOW_FULLSCREEN`
     * @return true if is set **/
    bool IsFullscreen ();
    /** @brief Check Window flags for `SDL_WINDOW_FULLSCREEN_DESKTOP`
     * @return true if is set **/
    bool IsFullscreenDesktop ();
    /** @brief Check Window flags for `SDL_WINDOW_FULLSCREEN`
     * @return true if is set **/
    bool IsOpenGl ();
    bool IsShown ();
    bool IsHidden ();
    bool IsBorderless ();
    bool HasBorder ();
    /** Check if the window is resizable. **/
    bool IsResizable ();
    bool IsMinimized ();
    bool IsMaximized ();
    bool HasInputGrab ();
    bool HasInputFocus ();
    bool HasMouseFocus ();
    bool IsForeign ();
    bool IsHighDPI ();
    bool IsMouseCaptured ();
    bool IsAlwaysOnTop ();
    bool IsSkipTaskbar ();
    bool IsUtility ();
    bool IsTooltip ();
    bool IsPopupMenu ();

    SDL_Window *m_Window;
    const SDL_GLContext m_Context;

    static s_ptr GetById (Uint32);
    static s_ptr GetByPtr (SDL_Window *);
    static s_ptr GetByGlContext (SDL_GLContext);


  private:
    static std::map<SDL_Window *, std::weak_ptr<Window> > m_PtrCache;
    static std::map<Uint32, std::weak_ptr<Window> > m_IdCache;
    static std::map<SDL_GLContext, std::weak_ptr<Window> > m_ContextCache;
    Window (SDL_Window *, SDL_GLContext);
  };
}