#include <hd/sdl/Window.hpp>

using namespace hd;

class Rect : public SDL_Rect {
  public:
  operator SDL_Rect * () { return (SDL_Rect *)this; };
  static Rect
  Union (const SDL_Rect *a1, const SDL_Rect *a2)
  {
    Rect created;
    SDL_UnionRect (a1, a2, &created);
    return created;
  };
  Rect
  Union (const SDL_Rect *aOther)
  {
    return Rect::Union (*this, aOther);
  };
};

template<class UI_CONCRETE_WIDGET>
class UiAbstractWidget {
public:
  typedef std::shared_ptr<UI_CONCRETE_WIDGET> s_ptr;
  UiAbstractWidget (){};
  ~UiAbstractWidget (){};
  std::function<Rect ()> GetMinimumRect = [this] () {
    return Rect{ 0, 0, 0, 0 };
  };
  std::function<Rect ()> GetRectRequested
      = [this] () { return this->GetMinimumRect (); };
};

template<class UI_CONCRETE_WIDGET>
class UiAbstractSurface : public UiAbstractWidget<UI_CONCRETE_WIDGET> {
protected:
  UiAbstractSurface (){};
public:
  std::function<sdl::Surface ()> GetSurface;
};
/**
 * @brief
 *
 */
class UiCompositSurface : public UiAbstractSurface<UiCompositSurface> {
public:
  std::unordered_multimap<Rect, UiAbstractSurface &> map_Surfaces();
  UiCompositSurface ()
  {
    GetRectRequested = [this] () {
      Rect last = { 0 };
      Rect next = { 0 };
      for (std::pair<Rect, UiAbstractSurface &> pair : map_Surfaces) {
        SDL_UnionRect (&last, &pair.first, &next);
        last = next;
      }
      return last;
    };
    GetSurface = [this] () {
      Rect requested = this->GetRectRequested ();
      sdl::Surface surface = sdl::Surface::Create (requested.w, requested.h);
      for (std::pair<Rect, UiAbstractSurface &> pair : map_Surfaces) {
        const Rect subReq = pair.second.GetRectRequested ();
        surface.BlitScaledFrom (pair.second.GetSurface (), pair.first, subReq);
      }
      return surface;
    };
  };
  void
  Add (Rect aRect, UiAbstractSurface &aUiSurface)
  {
    map_Surfaces.insert (std::pair<Rect, UiAbstractSurface &> (aRect, aUiSurface));

  };
};
/**
 * @brief
 *
 */
class UiSurface : public UiAbstractSurface {
public:
  sdl::Surface m_Surface;
  UiCompositSurface root;
  UiSurface (int aWidth, int aHeight)
      : m_Surface (sdl::Surface::Create (aWidth, aHeight))
  {
    GetRectRequested = [this] () {
      return Rect{ 0, 0, m_Surface.ptr->w, m_Surface.ptr->h };
    };
    GetSurface = [this] () { return m_Surface; };
  };
};

/**
 * @brief
 *
 */
class UiWindowSurface : public UiAbstractSurface {
public:
  sdl::Window window;
  std::function<void ()> doClose = [this] () {
    Engine::Get ()->step.Void.Once ([this] () { window.ptr = nullptr; });
  };
  UiWindowSurface (int aWidth, int aHeight)
      : window (sdl::Window::Create (aWidth, aHeight))
  {
    window.Event ().Close.Void.Once (doClose);
    GetRectRequested = [this] () {
      int x, y, w, h;
      window.GetSize (&w, &h);
      window.GetPosition (&x, &y);
      return Rect{ x, y, w, h };
    };
    GetSurface = [this] () { return window.GetSurface (); };
  };
};
/**
 * @brief
 *
 * @param argc
 * @param argv
 * @return int
 */
int
main (int argc, char **argv)
{
  Engine::Configure (argc, argv);
  // sdl::Window mainWindow = sdl::Window::Create (SDL_WINDOW_HIDDEN);
  UiWindowSurface main (800, 600);
  Engine::Get ()->Start ();
}