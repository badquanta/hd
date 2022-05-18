#include <hd/Error.hpp>
#include <hd/Ui.hpp>
#include <hd/sdl/Window.hpp>
using namespace hd;

/**
 * @brief A bare bones example of how to use the framework to make a font
 * viewer.
 *
 * @param argc count of command arguments given by the shell
 * @param argv list of command argument strings given by the shell
 * @return int our status code.
 */
int
main (int argc, char **argv)
{
  // HD's Engine is the hub of the system and can be configured via command
  // line.
  Engine::Configure (argc, argv);
  // So we need a shared pointer to our window controller
  std::shared_ptr<UiWindowSurface> winCtl
      // We make a new one with an `sdl::Window` that we also create.
      = std::make_shared<UiWindowSurface> (sdl::Window::Create (800, 600));
  // Let's check that we have a window, and if not we'll raise an exception and
  // exit.
  hdErrorIf (!winCtl->window, "Unable to open sdl window.");
  // We used a window constructor that doesn't set a title, so we should now:
  winCtl->window.SetTitle ("Font Viewer");
  // This is the initial font we will load if no other font has yet been
  // specified.
  sdl::Font font = sdl::Font::Load ("ttf/Ac437_IBM_VGA_8x16.ttf", 16);
  // Let's check if that font exists and if not error out.
  hdErrorIf (!font, "Unable to load font.");
  // We need to tell SDL we are interested in DROPTEXT & DROPFILE events.
  SDL_EventState (SDL_DROPTEXT, SDL_ENABLE);
  SDL_EventState (SDL_DROPFILE, SDL_ENABLE);
  // This is a simple widget that draws a color (i.e. clears) on to its render
  // destination.  This is how we'll give the window a solid background color.
  auto clrCtl
      = std::make_shared<UiSurfaceColor> (SDL_Color{ 255, 255, 255, 255 });
  // This is a simple widget that takes sdl::Font, SDL_Color, and UTF8 string
  // and will render text to the destination surface.
  auto txtCtl = std::make_shared<UiViewText> (
      font, "Font viewer.", SDL_Color{ 0, 0, 0, 255 });
  // We connect a sdl::Event::Handler to the SDL_DROPFILE event
  winCtl->event.Drop.File.On ([&] (const SDL_Event &e) {
    // Try to load the font
    sdl::Font newFont = sdl::Font::Load (e.drop.file, 16);
    // Check that it was loaded
    if (!newFont) {                               // If not
      winCtl->window.SetTitle ("Unable to open"); // Tell the user
    } else {                                      // Otherwise
      winCtl->window.SetTitle (e.drop.file);      // Tell the user
      txtCtl->font = newFont;                     // and start using it.
    }
    // Per the SDL docs on the event
    SDL_free (e.drop.file);
  });
  // We also connect to the SDL_DROPTEXT event
  winCtl->event.Drop.Text.On ([&] (const SDL_Event &e) {
    txtCtl->text = e.drop.file; // So that the user can provide sample text.
    SDL_free (e.drop.file);
  });
  // Append the color & text to the `root` (UiComposition) of the Window Controller
  winCtl->root.Append (clrCtl);
  winCtl->root.Append (txtCtl);
  // @todo (winCtl should have its own output event handler)
  Engine::Get ()->output.Void.On ([winCtl] () {
    if (winCtl->window) {
      winCtl->RenderSurface ();
      winCtl->window.UpdateSurface ();
    }
  });
  // start the engine
  Engine::Get ()->Start ();
}