#include "hd/Window.hpp"
namespace hd {
  /** Creates an SDL Window with an OpenGL context
   * @note use `Create` instead of directly calling this constructor **/
  Window::Window (SDL_Window *aWindow, SDL_GLContext aContext)
      : m_Window (aWindow), m_Context (aContext)
  {
    engine = Engine::Get ();
    assert (aWindow);
    assert (aContext);
    onHandle = engine->input.Windows[Id ()].On (input.pipe);
    outputHandle = engine->output.On (output.pipe);
    hdDebugCall ("id=%d winPtr=%p glCtxPr = %p, ", Id (), aWindow, aContext);
  }
  /** Destroys the underlying SDL_Window reference **/
  Window::~Window ()
  {
    hdDebugCall ("id#%d");
    engine->input.Windows[Id ()].Delete (onHandle);
    engine->output.Delete (outputHandle);
    m_IdCache.erase (Id ());
    m_ContextCache.erase (m_Context);
    m_PtrCache.erase (m_Window);
    if (m_Window) {
      hdDebug ("Destroying Window ID%d", Id ());
      SDL_DestroyWindow (m_Window);
    } else {
      hdDebug ("NULL");
    }
    if (m_Context != NULL) {
      hdDebug ("Deleting GL Context");
      SDL_GL_DeleteContext (m_Context);
    }
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
  std::map<SDL_Window *, std::weak_ptr<Window> > Window::m_PtrCache;
  /** Uint32 (SDL_WindowID) -> Smart pointer cache. **/
  std::map<Uint32, std::weak_ptr<Window> > Window::m_IdCache;
  /** SDL_GLContext pointer -> Window Smart pointer cache. **/
  std::map<SDL_GLContext, std::weak_ptr<Window> > Window::m_ContextCache;
  Window::Ptr
  Window::Create (SDL_Window *aWindow, SDL_GLContext aContext)
  {
    Ptr created(new Window (aWindow, aContext));
    m_PtrCache[aWindow] = created;
    m_IdCache[created->Id ()] = created;
    m_ContextCache[aContext] = created;
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
  Window::Ptr
  Window::Create (const char *aTitle, SDL_Rect *aRect,  Uint32 aFlags)
  {
    assert (aRect != NULL);
    assert (aTitle != NULL);
    Engine::Ptr engine = Engine::Get ();
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
      return NULL;
    }
    SDL_GLContext created_context = SDL_GL_CreateContext (created);
    if (created_context == NULL) {
      hdError ("Unable to create SDL GL Context because:\n %s",
               SDL_GetError ());
      SDL_DestroyWindow (created);
    }
    return Create (created, created_context);
  }
  Window::Ptr
  Window::Create (int w, int h, const char *aTitle, Uint32 aFlags)
  {
    SDL_Rect aRect = { NextRect.x, NextRect.h, w, h };
    return Create ( aTitle, &aRect, aFlags);
  }
  Window::Ptr
  Window::GetById (Uint32 aId)
  {
    if (m_IdCache.find (aId) != m_IdCache.end ()) {
      return m_IdCache.at (aId).lock ();
    }
    return NULL;
  }
  Window::Ptr
  Window::GetByPtr (SDL_Window *aWindow)
  {
    if (m_PtrCache.find (aWindow) != m_PtrCache.end ()) {
      return m_PtrCache.at (aWindow).lock ();
    }
    return NULL;
  }
  Window::Ptr
  Window::GetByGlContext (SDL_GLContext aContext)
  {
    if (m_ContextCache.find (aContext) != m_ContextCache.end ()) {
      return m_ContextCache.at (aContext).lock ();
    }
    return NULL;
  }

  Uint32
  Window::Id ()
  {
    Uint32 id = SDL_GetWindowID (m_Window);
    if (id == 0) {
      hdError ("Failed to get Window Id %s", SDL_GetError ());
    }
    return id;
  }

  void
  Window::Swap ()
  {
    SDL_GL_SwapWindow (m_Window);
  }

  bool
  Window::SetSwapInterval (int aInterval)
  {
    return SDL_GL_SetSwapInterval (aInterval) == 0;
  }

  bool
  Window::MakeCurrent ()
  {
    if (SDL_GL_MakeCurrent (m_Window, m_Context) != 0) {
      hdError ("Unable to make Window/GLContext current: %s", SDL_GetError ());
      return false;
    }
    return true;
  }
  void
  Window::GetDrawableSize (int *w, int *h)
  {
    SDL_GL_GetDrawableSize (m_Window, w, h);
  }
  void
  Window::Hide ()
  {
    SDL_HideWindow (m_Window);
  }
  void
  Window::Show ()
  {
    SDL_ShowWindow (m_Window);
  }
  void
  Window::Raise ()
  {
    SDL_RaiseWindow (m_Window);
  }
  void
  Window::Maximize ()
  {
    SDL_MaximizeWindow (m_Window);
  }
  void
  Window::Minimize ()
  {
    SDL_MinimizeWindow (m_Window);
  }
  void
  Window::Restore ()
  {
    SDL_RestoreWindow (m_Window);
  }
  void
  Window::SetBordered (bool aBool)
  {
    SDL_bool setting = (aBool ? SDL_TRUE : SDL_FALSE);
    SDL_SetWindowBordered (m_Window, setting);
  }
  void *
  Window::SetData (const char *aName, void *aData)
  {
    return SDL_SetWindowData (m_Window, aName, aData);
  }
  void *
  Window::GetData (const char *aName)
  {
    return SDL_GetWindowData (m_Window, aName);
  }
  bool
  Window::SetFullscreen (Uint32 aFlags)
  {
    if (SDL_SetWindowFullscreen (m_Window, aFlags) != 0) {
      hdError ("Unable to set window fullscreen because: %s", SDL_GetError ());
      return false;
    }
    return true;
  }
  void
  Window::SetGrab (bool aGrab)
  {
    SDL_bool setting = (aGrab ? SDL_TRUE : SDL_FALSE);
    SDL_SetWindowGrab (m_Window, setting);
  }
  void
  Window::SetKeyboardGrab (bool aGrab)
  {
#if SDL_VERSION_ATLEAST(2, 0, 16)
    SDL_bool setting = (aGrab ? SDL_TRUE : SDL_FALSE);
    SDL_SetWindowKeyboardGrab (m_Window, setting);
#else
    hdError ("SDL Version is less than 2.0.16");
#endif
  }

  void
  Window::SetTitle (std::string aTitle)
  {
    SDL_SetWindowTitle (m_Window, aTitle.c_str ());
  }

  std::string
  Window::GetTitle ()
  {
    const char *title = SDL_GetWindowTitle (m_Window);
    return std::string (title);
  }

  bool
  Window::GetWmInfo (SDL_SysWMinfo *aInfo)
  {
    if (SDL_GetWindowWMInfo (m_Window, aInfo) != SDL_TRUE) {
      hdError ("Unable to get System Window Manager Info: %s",
               SDL_GetError ());
      return false;
    }
    return true;
  }

  void
  Window::SetIcon (SDL_Surface *aSurface)
  {
    SDL_SetWindowIcon (m_Window, aSurface);
  }
  void
  Window::SetIcon (Shared::Surface aSurface)
  {
    SetIcon (aSurface.get ());
  }
  void
  Window::SetIcon (std::filesystem::path aPath)
  {
    SetIcon (Shared::makeSurface (aPath));
  }
  void
  Window::SetSize (int w, int h)
  {
    SDL_SetWindowSize (m_Window, w, h);
  }
  void
  Window::GetSize (int *w, int *h)
  {
    SDL_GetWindowSize (m_Window, w, h);
  }
  void
  Window::SetMaximumSize (int w, int h)
  {
    SDL_SetWindowMaximumSize (m_Window, w, h);
  }
  void
  Window::GetMaximumSize (int *w, int *h)
  {
    SDL_GetWindowMaximumSize (m_Window, w, h);
  }
  void
  Window::SetMinimumSize (int w, int h)
  {
    SDL_SetWindowMinimumSize (m_Window, w, h);
  }
  void
  Window::GetMinimumSize (int *w, int *h)
  {
    SDL_GetWindowMinimumSize (m_Window, w, h);
  }
  bool
  Window::SetModalFor (SDL_Window *aWindow)
  {
    if (SDL_SetWindowModalFor (aWindow, m_Window) != 0) {
      hdError ("Unable to set Window as Modal: %s", SDL_GetError ());
      return false;
    }
    return true;
  }
  bool
  Window::SetModalFor (Window::Ptr aWindow)
  {
    return SetModalFor (aWindow->m_Window);
  }
  bool
  Window::SetOpacity (float aOpacity)
  {
    if (SDL_SetWindowOpacity (m_Window, aOpacity) != 0) {
      hdError ("Unable to set Window Opacity: %s", SDL_GetError ());
      return false;
    }
    return true;
  }
  float
  Window::GetOpacity ()
  {
    float value;
    if (SDL_GetWindowOpacity (m_Window, &value) != 0) {
      hdError ("Unable to get Window Opacity: %s", SDL_GetError ());
    }
    return value;
  }
  void
  Window::SetPosition (int x, int y)
  {
    SDL_SetWindowPosition (m_Window, x, y);
  }
  void
  Window::GetPosition (int *x, int *y)
  {
    SDL_GetWindowPosition (m_Window, x, y);
  }
  void
  Window::SetResizable (bool aBool)
  {
    SDL_bool setting = aBool ? SDL_TRUE : SDL_FALSE;
    SDL_SetWindowResizable (m_Window, setting);
  }
  Uint32
  Window::GetFlags ()
  {
    return SDL_GetWindowFlags (m_Window);
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