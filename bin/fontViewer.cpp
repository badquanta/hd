/**
 * @file fontViewer.cpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-18
 *
 * @copyright GNU-GPL 3.0 Copyright (C) 2022 Jón Davíð Sawyer
 * (badquanta@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *

 */
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
  std::shared_ptr<UiCtrlSdlWindowSurface> winCtl
      // We make a new one with an `sdl::Window` that we also create.
      = std::make_shared<UiCtrlSdlWindowSurface> (
          sdl::Window::Create (800, 600));
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
  SDL_EventState (SDL_DROPBEGIN, SDL_ENABLE);
  SDL_EventState (SDL_DROPTEXT, SDL_ENABLE);
  SDL_EventState (SDL_DROPFILE, SDL_ENABLE);
  SDL_EventState (SDL_DROPCOMPLETE, SDL_ENABLE);
  // This is a simple widget that draws a color (i.e. clears) on to its render
  // destination.  This is how we'll give the window a solid background color.
  auto clrCtl = std::make_shared<UiCtrlSdlSurfaceColor> (
      SDL_Color{ 255, 255, 255, 255 });
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
  winCtl->event.Drop.Begin.Void.On ([&] () { txtCtl->text = ""; });
  // We also connect to the SDL_DROPTEXT event
  winCtl->event.Drop.Text.On ([&] (const SDL_Event &e) {
    txtCtl->text += e.drop.file; // So that the user can provide sample text.
    txtCtl->text += "\n";
    printf ("DROP TEXT: %s\n", e.drop.file);
    SDL_free (e.drop.file);
  });
  winCtl->event.Drop.Complete.Void.On ([&] () {
    clrCtl->color = { 128, 128, 128, 255 };
  });
  // Append the color & text to the `root` (UiComposition) of the Window
  // Controller
  auto posCtrl = std::make_shared<UiViewPositionedSurfaces> ();
  auto txt2Ctrl = std::make_shared<UiViewText> (
      font, "Say something new", SDL_Color{ 255, 0, 0, 255 });
  auto clr2Ctrl
      = std::make_shared<UiCtrlSdlSurfaceColor> (SDL_Color{ 0, 255, 0, 255 });
  posCtrl->Append (txt2Ctrl, { 100, 200 });
  posCtrl->Append (clr2Ctrl, { { 25, 25 }, { 50, 50 } });
  posCtrl->Append (clr2Ctrl, { { 300, 200 }, { 500, 500 } });

  winCtl->root.Append (clrCtl);
  winCtl->root.Append (txtCtl);
  winCtl->root.Append (posCtrl);

  // @todo (winCtl should have its own output event handler)
  // start the engine
  Engine::Get ()->Start ();
}