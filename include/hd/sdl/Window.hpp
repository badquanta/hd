#pragma once
#include "hd/sdl/Surface.hpp"
namespace hd::sdl {

  /** future
   * @todo https://wiki.libsdl.org/SDL_SetWindowHitTest
   * @todo https://wiki.libsdl.org/SDL_SetWindowHitTest
   * @todo https://wiki.libsdl.org/SDL_IsScreenSaverEnabled
   **/
  class Window : public WRAP_PTR<SDL_Window> , public EngineComponent {
  public: // Static constructor methods
    using WRAP_PTR::WRAP_PTR;
    using WRAP_PTR::operator=;
    /** Given an existing SDL_Windowand SDL_GLContext pointer construct
     * a sdl::Window::s_ptr
     * @todo make this check the caches?
     ***/
    static sdl::Window Create (SDL_Window *, SDL_GLContext, bool);
    /**
     * @brief
     *
     * @param aRect [SDL_Rect*] describing the position and size of the desired
     * window.
     * @param aTitle [const char*] used for the initial window title.
     * @param aFlags [Uint32] describing the window flags to use
     * @note Windows are always created with SDL_WINDOW_OPENGL flag set.
     * @return sdl::Window::s_ptr
     */
    static sdl::Window Create (const char *aTitle = Window::NextTitle,
                                      SDL_Rect *aRect = &Window::NextRect,

                                      Uint32 aFlags = Window::NextFlags);
    static sdl::Window Create (int aWidth,
                                      int aHeight,
                                      const char *aTitle = Window::NextTitle,
                                      Uint32 aFlags = Window::NextFlags);

  public: // instance methods:
    evt::WindowDispatch &Event ();
    /** Return the SDL Surface associated with this window. **/
    Surface GetSurface ();
    bool UpdateSurface ();
    void Swap ();
    /** @see https://wiki.libsdl.org/SDL_GL_SetSwapInterval **/
    bool SetSwapInterval (int);
    /** @see https://wiki.libsdl.org/SDL_GL_MakeCurrent **/
    bool MakeCurrent (SDL_GLContext);

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
    void SetIcon (sdl::Surface::s_ptr);
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

  public:
    /** `Create` uses this position and size if none are defined. **/
    static SDL_Rect NextRect;
    /** `Create` uses this title if none is defined. **/
    static const char *NextTitle;
    /** `Create()` uses these flags if none are defined. **/
    static SDL_WindowFlags NextFlags;
    /** Destroy a window and its associated OpenGL Context.**/
  };
}