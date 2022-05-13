#include "hd/sdl/Window.hpp"
namespace hd::sdl {

  Surface
  Window::GetSurface ()
  {
    SDL_Surface *aSurface = SDL_GetWindowSurface (*this);
    return Surface::Create (aSurface, false);
  }
  bool
  Window::UpdateSurface ()
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
  SDL_WindowFlags Window::NextFlags
      = (SDL_WindowFlags)(SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  /** SDL_Window* -> Smart Pointer cache. **/
  //std::map<SDL_Window *, std::weak_ptr<SDL_Window> > Window::m_PtrCache;
  /** Uint32 (SDL_WindowID) -> Smart pointer cache. **/
  //std::map<Uint32, std::weak_ptr<SDL_Window> > Window::m_IdCache;
  /** SDL_GLContext pointer -> Window Smart pointer cache. **/
  //std::map<SDL_GLContext, std::weak_ptr<SDL_Window> > Window::m_ContextCache;
  sdl::Window
  Window::Create (SDL_Window *aWindow, SDL_GLContext aContext, bool aFree)
  {
    s_ptr created(aWindow, aFree ? SDL_DestroyWindow : [](SDL_Window*p){});
    Window window = created;
    //@todo remove m_PtrCache[aWindow] = created;
    //m_IdCache[window.Id ()] = created;
    //m_ContextCache[aContext] = created;
    return created;
  }
  /**
   * @brief Construct a new Window:: Create object
   *
   * @param aRect position and size
   * @param aTitle to be set on new window
   * @param aFlags ADDITIONAL flags.
   * @note aFlags is always set with `SDL_WINDOW_OPENGL`
   * @return smart pointer to and instance of this class managing a window.
   * @return NULL on failure.
   */
  sdl::Window
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
    SDL_Window *created = SDL_CreateWindow (aTitle,
                                            aRect->x,
                                            aRect->y,
                                            aRect->w,
                                            aRect->h,
                                            SDL_WINDOW_OPENGL | aFlags);
    if (created == NULL) {
      hdError ("Unable to create SDL Window because:\n %s", SDL_GetError ());
    }
    SDL_GLContext created_context =  (created);
    if (created_context == NULL) {
      hdError ("Unable to create SDL GL Context because:\n %s",
               SDL_GetError ());
      SDL_DestroyWindow (created);
    }
    return s_ptr (created, SDL_DestroyWindow);
  }
  sdl::Window
  Window::Create (int w, int h, const char *aTitle, Uint32 aFlags)
  {
    SDL_Rect aRect = { NextRect.x, NextRect.h, w, h };
    return Create (aTitle, &aRect, aFlags);
  }
  sdl::WindowDispatch&Window::Event(){
    return engine->input.Windows[Id ()];
  }
  Uint32
  Window::Id ()
  {
    Uint32 id = SDL_GetWindowID (*this);
    if (id == 0) {
      hdError ("Failed to get Window Id %s", SDL_GetError ());
    }
    return id;
  }

  void
  Window::Swap ()
  {
    SDL_GL_SwapWindow (*this);
  }

  bool
  Window::SetSwapInterval (int aInterval)
  {
    return SDL_GL_SetSwapInterval (aInterval) == 0;
  }

  bool
  Window::MakeCurrent (SDL_GLContext aContext)
  {
    if (SDL_GL_MakeCurrent (*this, aContext) != 0) {
      hdError ("Unable to make Window/GLContext current: %s", SDL_GetError ());
      return false;
    }
    return true;
  }
  void
  Window::GetDrawableSize (int *w, int *h)
  {
    SDL_GL_GetDrawableSize (*this, w, h);
  }
  void
  Window::Hide ()
  {
    SDL_HideWindow (*this);
  }
  void
  Window::Show ()
  {
    SDL_ShowWindow (*this);
  }
  void
  Window::Raise ()
  {
    SDL_RaiseWindow (*this);
  }
  void
  Window::Maximize ()
  {
    SDL_MaximizeWindow (*this);
  }
  void
  Window::Minimize ()
  {
    SDL_MinimizeWindow (*this);
  }
  void
  Window::Restore ()
  {
    SDL_RestoreWindow (*this);
  }
  void
  Window::SetBordered (bool aBool)
  {
    SDL_bool setting = (aBool ? SDL_TRUE : SDL_FALSE);
    SDL_SetWindowBordered (*this, setting);
  }
  void *
  Window::SetData (const char *aName, void *aData)
  {
    return SDL_SetWindowData (*this, aName, aData);
  }
  void *
  Window::GetData (const char *aName)
  {
    return SDL_GetWindowData (*this, aName);
  }
  bool
  Window::SetFullscreen (Uint32 aFlags)
  {
    if (SDL_SetWindowFullscreen (*this, aFlags) != 0) {
      hdError ("Unable to set window fullscreen because: %s", SDL_GetError ());
      return false;
    }
    return true;
  }
  void
  Window::SetGrab (bool aGrab)
  {
    SDL_bool setting = (aGrab ? SDL_TRUE : SDL_FALSE);
    SDL_SetWindowGrab (*this, setting);
  }
  void
  Window::SetKeyboardGrab (bool aGrab)
  {
#if SDL_VERSION_ATLEAST(2, 0, 16)
    SDL_bool setting = (aGrab ? SDL_TRUE : SDL_FALSE);
    SDL_SetWindowKeyboardGrab (*this, setting);
#else
    hdError ("SDL Version is less than 2.0.16");
#endif
  }

  void
  Window::SetTitle (std::string aTitle)
  {
    SDL_SetWindowTitle (*this, aTitle.c_str ());
  }

  std::string
  Window::GetTitle ()
  {
    const char *title = SDL_GetWindowTitle (*this);
    return std::string (title);
  }

  bool
  Window::GetWmInfo (SDL_SysWMinfo *aInfo)
  {
    if (SDL_GetWindowWMInfo (*this, aInfo) != SDL_TRUE) {
      hdError ("Unable to get System Window Manager Info: %s",
               SDL_GetError ());
      return false;
    }
    return true;
  }

  void
  Window::SetIcon (SDL_Surface *aSurface)
  {
    SDL_SetWindowIcon (*this, aSurface);
  }
  void
  Window::SetIcon (sdl::Surface::s_ptr aSurface)
  {
    SetIcon (aSurface);
  }
  void
  Window::SetIcon (std::filesystem::path aPath)
  {
    SetIcon (sdl::Surface::Load (aPath).ptr);
  }
  void
  Window::SetSize (int w, int h)
  {
    SDL_SetWindowSize (*this, w, h);
  }
  void
  Window::GetSize (int *w, int *h)
  {
    SDL_GetWindowSize (*this, w, h);
  }
  void
  Window::SetMaximumSize (int w, int h)
  {
    SDL_SetWindowMaximumSize (*this, w, h);
  }
  void
  Window::GetMaximumSize (int *w, int *h)
  {
    SDL_GetWindowMaximumSize (*this, w, h);
  }
  void
  Window::SetMinimumSize (int w, int h)
  {
    SDL_SetWindowMinimumSize (*this, w, h);
  }
  void
  Window::GetMinimumSize (int *w, int *h)
  {
    SDL_GetWindowMinimumSize (*this, w, h);
  }
  bool
  Window::SetModalFor (SDL_Window *aWindow)
  {
    if (SDL_SetWindowModalFor (aWindow, *this) != 0) {
      hdError ("Unable to set Window as Modal: %s", SDL_GetError ());
      return false;
    }
    return true;
  }
  bool
  Window::SetOpacity (float aOpacity)
  {
    if (SDL_SetWindowOpacity (*this, aOpacity) != 0) {
      hdError ("Unable to set Window Opacity: %s", SDL_GetError ());
      return false;
    }
    return true;
  }
  float
  Window::GetOpacity ()
  {
    float value;
    if (SDL_GetWindowOpacity (*this, &value) != 0) {
      hdError ("Unable to get Window Opacity: %s", SDL_GetError ());
    }
    return value;
  }
  void
  Window::SetPosition (int x, int y)
  {
    SDL_SetWindowPosition (*this, x, y);
  }
  void
  Window::GetPosition (int *x, int *y)
  {
    SDL_GetWindowPosition (*this, x, y);
  }
  void
  Window::SetResizable (bool aBool)
  {
    SDL_bool setting = aBool ? SDL_TRUE : SDL_FALSE;
    SDL_SetWindowResizable (*this, setting);
  }
  Uint32
  Window::GetFlags ()
  {
    return SDL_GetWindowFlags (*this);
  }

#define CheckFor(aFlag) (GetFlags () & aFlag) != 0

  bool
  Window::IsFullscreen ()
  {
    return CheckFor (SDL_WINDOW_FULLSCREEN);
  }
  bool
  Window::IsFullscreenDesktop ()
  {
    return CheckFor (SDL_WINDOW_FULLSCREEN_DESKTOP);
  }
  bool
  Window::IsOpenGl ()
  {
    return CheckFor (SDL_WINDOW_OPENGL);
  }
  bool
  Window::IsShown ()
  {
    return CheckFor (SDL_WINDOW_SHOWN);
  }
  bool
  Window::IsHidden ()
  {
    return CheckFor (SDL_WINDOW_HIDDEN);
  }
  bool
  Window::IsBorderless ()
  {
    return CheckFor (SDL_WINDOW_BORDERLESS);
  }
  bool
  Window::HasBorder ()
  {
    return !IsBorderless ();
  }
  bool
  Window::IsResizable ()
  {
    return CheckFor (SDL_WINDOW_RESIZABLE);
  }
  bool
  Window::IsMinimized ()
  {
    return CheckFor (SDL_WINDOW_MINIMIZED);
  }
  bool
  Window::IsMaximized ()
  {
    return CheckFor (SDL_WINDOW_MAXIMIZED);
  }
  bool
  Window::HasInputGrab ()
  {
    return CheckFor (SDL_WINDOW_INPUT_GRABBED);
  }
  bool
  Window::HasInputFocus ()
  {
    return CheckFor (SDL_WINDOW_INPUT_FOCUS);
  }
  bool
  Window::HasMouseFocus ()
  {
    return CheckFor (SDL_WINDOW_MOUSE_FOCUS);
  }
  bool
  Window::IsForeign ()
  {
    return CheckFor (SDL_WINDOW_FOREIGN);
  }
  bool
  Window::IsHighDPI ()
  {
    return CheckFor (SDL_WINDOW_ALLOW_HIGHDPI);
  }
  bool
  Window::IsMouseCaptured ()
  {
    return CheckFor (SDL_WINDOW_MOUSE_CAPTURE);
  }
  bool
  Window::IsAlwaysOnTop ()
  {
    return CheckFor (SDL_WINDOW_ALWAYS_ON_TOP);
  }
  bool
  Window::IsSkipTaskbar ()
  {
    return CheckFor (SDL_WINDOW_SKIP_TASKBAR);
  }
  bool
  Window::IsUtility ()
  {
    return CheckFor (SDL_WINDOW_UTILITY);
  }
  bool
  Window::IsTooltip ()
  {
    return CheckFor (SDL_WINDOW_TOOLTIP);
  }
  bool
  Window::IsPopupMenu ()
  {
    return CheckFor (SDL_WINDOW_POPUP_MENU);
  }
}