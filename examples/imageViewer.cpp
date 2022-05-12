#include "hd/sdl/Surface.hpp"
#include "hd/sdl/Window.hpp"
using hd::sdl::Surface;
using hd::sdl::Window;

int
main (int argc, char **argv)
{
  hd::sdl::Surface pic = Surface::Load ("textures/pattern_16/preview.jpg");
  hd::sdl::Window win = Window::Create ("Picture Window");
  win.Event().Close.Void.On ([&] () {
    win.Hide ();
    win.engine->Delay (0, [&win] (int) { win.m_IDENTITY = NULL; });
  });
  const SDL_Rect src
      = { 0, 0, pic.ptr->w, pic.ptr->h };
  SDL_Rect dst = { 50, 75, 100, 100 };
  win.engine->output.Void.On ([&] () {
    Surface winSurf = win.GetSurface ();
    SDL_Rect background{0};
    win.GetSize (&background.w,&background.h);
    winSurf.FillRect (&background, winSurf.MapRGBA(128, 64, 32));
    pic.BlitScaled (winSurf, &dst, &src);
    win.UpdateSurface ();
  });
  win.Event().Mouse.Wheel.On ([&] (const SDL_Event &e) {
    const SDL_MouseWheelEvent &w = e.wheel;
    printf ("Mouse wheel x=%d y=%d direction:%d\n",w.x,w.y, w.direction);
  });

  win.Event ().Key.Keycode[SDLK_f].Void.On (
      [] () { printf ("SFS's keycode.\n"); });

  win.engine->Start ();
  printf ("Image viewer.\n");
}
