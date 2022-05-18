#include <hd/sdl/Window.hpp>
#include <hd/Ui.hpp>
#include <hd/Error.hpp>
using namespace hd;

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
  std::shared_ptr<UiWindowSurface> winCtl = std::make_shared<UiWindowSurface> (sdl::Window::Create(800,600));
  winCtl->window.SetTitle ("hd font viewer");
  sdl::Font font
      = sdl::Font::Load ("ttf/Ac437_IBM_VGA_8x16.ttf", 16);
  hdErrorIf (!font, "Unable to load font.");
  SDL_EventState (SDL_DROPTEXT, SDL_ENABLE);
  auto clrCtl = std::make_shared<UiSurfaceColor>(SDL_Color{ 255, 255, 255, 255 });
  auto txtCtl = std::make_shared<UiViewText> (font, "Font viewer.", SDL_Color{ 0, 0, 0, 255 });
  winCtl->event.Drop.Text.On ([&] (const SDL_Event &e) {
    printf ("%s\n", e.drop.file);
    std::string data = e.drop.file;
    SDL_free (e.drop.file);
    txtCtl->text = data;
  });
  winCtl->event.Mouse.Button[1].Down.Void.On (
      [] () { printf ("Mouse button down. \n"); });


  winCtl->root.Append (clrCtl);
  winCtl->root.Append (txtCtl);
  Engine::Get ()->output.Void.On ([winCtl] () {
    if(winCtl->window){
      winCtl->RenderSurface ();
      winCtl->window.UpdateSurface ();
    }
  });

  Engine::Get ()->Start ();
}