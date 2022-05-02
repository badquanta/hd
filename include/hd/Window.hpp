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
#include "hd/Shared.hpp"
#include "hd/evt/VoidDispatch.hpp"
#include "hd/evt/WindowDispatch.hpp"
#include "hd/Engine.hpp"
#include <memory>
namespace hd {
  class Window {
  public:
    typedef std::shared_ptr<Window> Ptr;
    static SDL_Rect NextRect;
    static const char *NextTitle;
    static SDL_WindowFlags NextFlags;
    virtual ~Window ();
    static Window::Ptr Create (SDL_Window *, SDL_GLContext);
    evt::VoidDispatch onRender;
    evt::WindowDispatch on;
    evt::SDL_EventDispatch::Handle onHandle;
    /**
     * @brief
     *
     * @param aRect [SDL_Rect*] describing the position and size of the desired
     * window.
     * @param aTitle [const char*] used for the initial window title.
     * @param aFlags [Uint32] describing the window flags to use
     * @note Windows are always created with SDL_WINDOW_OPENGL flag set.
     * @return Window::Ptr
     */
    static Window::Ptr Create (SDL_Rect *aRect = &Window::NextRect,
                                 const char *aTitle = Window::NextTitle,
                                 Uint32 aFlags = Window::NextFlags);
    static Window::Ptr Create (int aWidth,
                                 int aHeight,
                                 const char *aTitle = Window::NextTitle,
                                 Uint32 aFlags = Window::NextFlags);
    Uint32 Id ();
    void Swap ();
    /** @see https://wiki.libsdl.org/SDL_GL_SetSwapInterval **/
    bool SetSwapInterval (int);
    /** @see https://wiki.libsdl.org/SDL_GL_MakeCurrent **/
    bool MakeCurrent ();
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
    /** future @todo https://wiki.libsdl.org/SDL_SetWindowHitTest **/
    /** future @todo https://wiki.libsdl.org/SDL_SetWindowHitTest **/
    /** future @todo https://wiki.libsdl.org/SDL_IsScreenSaverEnabled **/
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
    bool SetModalFor (Window::Ptr);
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

    static Ptr GetById(Uint32);
    static Ptr GetByPtr (SDL_Window *);
    static Ptr GetByGlContext (SDL_GLContext);
    static void RenderAll ();

  private:
    Engine::Mount engine;
    static std::map<SDL_Window *, std::weak_ptr<Window> > m_PtrCache;
    static std::map<Uint32, std::weak_ptr<Window> > m_IdCache;
    static std::map<SDL_GLContext, std::weak_ptr<Window> > m_ContextCache;
    Window (SDL_Window *, SDL_GLContext);
  };
}