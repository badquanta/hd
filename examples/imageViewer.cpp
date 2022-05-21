#include <hd/Error.hpp>
#include <hd/Ui.hpp>
// save us typing hd:: over and over again.
using namespace hd;
int
main (int argc, char **argv)
{
  // Pass the CLI to Engine to get FindFile to locate `assets` path.
  Engine::Configure (argc, argv);
  // Open up a Window Control
  UiCtrlSdlWindowSurface::s_ptr winCtl
      = std::make_shared<UiCtrlSdlWindowSurface> (
          sdl::Window::Create ("Picture Window"));
  hdErrorIf (!winCtl->window,
             "Unable to open SDL Window because %s",
             SDL_GetError ());
  // Open an image.
  UiCtrlSdlSurface::s_ptr picCtl = std::make_shared<UiCtrlSdlSurface> (
      sdl::Surface::Load ("textures/pattern_16/preview.jpg"));
  hdErrorIf (!picCtl->surface,
             "Unable to open SDL Window because %s",
             SDL_GetError ());
  winCtl->root.Append (picCtl);
  winCtl->event.Mouse.Wheel.On ([&] (const SDL_Event &e) {
    const SDL_MouseWheelEvent &w = e.wheel;
    printf ("Mouse wheel x=%d y=%d direction:%d\n", w.x, w.y, w.direction);
  });
  winCtl->event.Key.Keycode[SDLK_F1].Up.Void.On (
      [&] () { picCtl->ToggleFlags (UI_GROW_V); });
  winCtl->event.Key.Keycode[SDLK_F2].Up.Void.On ([&] () {
    printf ("Toggle Grow Horizontal");
    picCtl->ToggleFlags (UI_GROW_H);
  });
  winCtl->event.Key.Keycode[SDLK_f].Void.On (
      [] () { printf ("SFS's keycode.\n"); });

  // win.engine->Start ();
  // winCtl->engine->Start ();
  Engine::Get ()->Start ();

  printf ("Image viewer.\n");
}
