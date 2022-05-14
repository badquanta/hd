#include <hd/sdl/Window.hpp>
using namespace hd;

class UiAbstractSurface {
protected:
  UiAbstractSurface (){};

public:
  virtual SDL_Rect GetRectRequested () = 0;
  virtual sdl::Surface GetSurface () = 0;
};
class UiSurface : public UiAbstractSurface {
public:
  UiSurface (int aWidth, int aHeight)
      : m_Surface (sdl::Surface::Create (aWidth, aHeight)){

        };
  virtual SDL_Rect
  GetRectRequested () override
  {
    return { 0, 0, m_Surface.ptr->w, m_Surface.ptr->h };
  };
  virtual sdl::Surface
  GetSurface () override
  {
    return m_Surface;
  };

private:
  sdl::Surface m_Surface;
};
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
  };
  virtual SDL_Rect
  GetRectRequested () override
  {
    int x, y, w, h;
    window.GetSize (&w, &h);
    window.GetPosition (&x, &y);
    return { x, y, w, h };
  };
  virtual sdl::Surface
  GetSurface () override
  {
    return window.GetSurface ();
  };
};
int
main (int argc, char **argv)
{
  Engine::Configure (argc, argv);
  sdl::Window mainWindow = sdl::Window::Create (SDL_WINDOW_HIDDEN);

  Engine::Get ()->Start ();
}