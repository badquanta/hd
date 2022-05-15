/**
 * @file multiWindow.cpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-14
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
 *
 */
#include "hd/sdl/Font.hpp"
#include "hd/sdl/GLContext.hpp"
#include "hd/sdl/Window.hpp"
#include "hd/Error.hpp"
using namespace hd;
/** Keep track of the windows this opens. **/
std::map<int, sdl::Window> openWindows;
/** Keep track of the FIRST window. **/
sdl::Window::s_ptr firstWinPtr;
/** Open additional windows. **/
void
openAnotherWindow ()
{
  SDL_Rect desktop = sdl::Window (firstWinPtr).GetDisplay ().GetBounds ();
  int w = rand () % desktop.w;
  int h = rand () % desktop.h;
  SDL_Rect r = { desktop.x + (rand () % desktop.w - w),
                 desktop.y + (rand () % desktop.h - h),
                 w,
                 h };
  sdl::Window opened = sdl::Window::Create ("Random New Window", &r);
  openWindows[opened.Id ()] = opened;
  auto OutputHandle = Engine::Get ()->output.Void.On ([=] () {
    sdl::Window win = opened;
    sdl::Surface winSurf = win.GetSurface ();
    winSurf.FillRect ({ 0, 0, 320, 200 }, winSurf.MapRGBA (0, 0, 0));
    win.UpdateSurface ();
  });
  opened.Event ().Key.Keycode[SDLK_f].Up.Void.On (
      [=] { sdl::Window (firstWinPtr).Raise (); });
  opened.Event ().Close.Void.Once ([=] () {
    Engine::Get ()->step.Void.Once ([=] () {
      Engine::Get ()->output.Void.Delete (OutputHandle);
      openWindows.erase (sdl::Window (opened).Id ());
    });
  });
}
void
openFirstWindow ()
{
  sdl::Window win = firstWinPtr = sdl::Window::Create (800, 600, "Multi-Window HD Example");
  sdl::Font font = sdl::Font::Load ("ttf/Ac437_IBM_VGA_8x16.ttf", 32);
  if (!font) {
    Engine::PrintSdlError ("Font not initialized because");
  }
  sdl::Surface message = font.RenderBlendedTextWrapped (
      "Welcome to the Multi-Window demo.\n\n  Click Here to Open new "
      "Windows.\n\nIf you close THIS window the program will quit.",
      { 255, 255, 255, 255 },
      800);

  assert (message);
  hdLog ("E");
  win.engine->output.On ([=] (int aTime) {
    if (firstWinPtr) {
      sdl::Surface winSurf = win.GetSurface ();

      SDL_Rect f{ 0 };
      winSurf.FillRect ({ 0, 0, 800, 600 }, winSurf.MapRGBA (0, 0, 0));
      message.BlitTo (winSurf, NULL, NULL);
      win.UpdateSurface ();
    }
  });
  win.Event ().Mouse.Button[1].Up.Void.On (openAnotherWindow);
  win.Event ().Close.Void.Once ([=] () {
    win.engine->step.Void.Once ([=] () { firstWinPtr = NULL; });
    Engine::Get ()->Quit ();
  });
}

int
main (int argc, char **argv)
{
  Engine::PrintVersions ();
  Engine::Configure (argc, argv);
  // Engine::s_ptr engine = Engine::Get ();
  openFirstWindow ();
  Engine::Get ()->Start ();
  return 0;
}