#include "hd/sdl/Surface.hpp"
#include "hd/sdl/Window.hpp"
using hd::sdl::Surface;
using hd::sdl::Window;

int
main (int argc, char **argv)
{
  Surface::s_ptr pic = Surface::Load ("textures/pattern_16/preview.jpg");
  Window::s_ptr win = Window::Create ("Picture Window");
  win->input.Close.Void.On ([&win] () {
    win->Hide ();
    win->engine->Delay (0, [&win] (int) { win = NULL; });
  });
  const SDL_Rect src
      = { 0, 0, pic->GetComponent ()->w, pic->GetComponent ()->h };
  SDL_Rect dst = { 50, 75, 100, 100 };
  win->output.Void.On ([=] () {
    Surface::s_ptr winSurf = win->GetSurface ();

    pic->BlitScaled (winSurf, &dst, &src);
    win->UpdateSurface ();
  });
  win->input.Mouse.Wheel.On ([&] (const SDL_Event &e) {

  });
  win->engine->Start ();
  printf ("Image viewer.\n");
}