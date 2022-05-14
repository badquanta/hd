/**
 * @file multiWindow.cpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-14
 *
 * @copyright GNU-GPL 3.0 Copyright (C) 2022 Jón Davíð Sawyer (badquanta@gmail.com)
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
 *
 */
#include "hd/sdl/Font.hpp"
#include "hd/sdl/GLContext.hpp"
#include "hd/sdl/Window.hpp"

using namespace hd;

// sdl::GLContext glCtx;
std::map<int, sdl::Window> openWindows;
void
openAnotherWindow ()
{
  sdl::Window opened = sdl::Window::Create ();
  openWindows[opened.Id ()] = opened;
  auto OutputHandle = Engine::Get ()->output.Void.On ([=] () {
    sdl::Window win = opened;
    sdl::Surface winSurf = win.GetSurface ();
    winSurf.FillRect ({ 0, 0, 320, 200 },winSurf.MapRGBA(0,0,0));
    win.UpdateSurface ();
  });
  opened.Event ().Close.Void.Once ([=] () {
    Engine::Get ()->step.Void.Once ([=] () {
      Engine::Get ()->output.Void.Delete (OutputHandle);
      openWindows.erase (sdl::Window (opened).Id ());
    });
  });
}

int
main (int argc, char **argv)
{
  Engine::PrintVersions ();
  Engine::Configure (argc, argv);
  // Engine::s_ptr engine = Engine::Get ();
  sdl::Window firstWin = sdl::Window::Create (800, 600, "HD1");

  sdl::Font font = sdl::Font::Load ("ttf/Ac437_IBM_VGA_8x16.ttf", 18);
  if (!font) {
    Engine::PrintSdlError ("Font not initialized because");
  }
  sdl::Surface message = font.RenderBlendedTextWrapped (
      "Welcome to the Multi-Window demo.\n\n  Click Here to Open new Windows.\n\nIf you close THIS window the program will quit.",
      { 255, 255, 255, 255 },
      800);
  assert (message);
  hdLog ("E");
  firstWin.engine->output.On ([&] (int aTime) {
    if (firstWin) {
      sdl::Surface winSurf = firstWin.GetSurface ();
      SDL_Rect f{ 0 };
      winSurf.FillRect ({ 0, 0, 800, 600 }, winSurf.MapRGBA (0, 0, 0));
      message.Blit (winSurf, NULL, NULL);
      firstWin.UpdateSurface ();
    }
  });
  firstWin.Event ().Mouse.Button[1].Up.Void.On (openAnotherWindow);
  firstWin.Event ().Close.Void.Once ([&] () {
    firstWin.engine->step.Void.Once ([&] () { firstWin = NULL; });
    Engine::Get ()->Quit ();
  });

  Engine::Get ()->Start ();
  return 0;
}