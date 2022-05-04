/**
 * holodeck - maybe it will be a game or a game engine
 * Copyright (C) 2022 Jón Davíð Sawyer (badquanta@gmail.com)
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
 */
#include "hd/Engine.hpp"
#include "hd/Window.hpp"

hd::Window::s_ptr window;

int
main (int argc, char **argv)
{
  hd::Engine::PrintVersions ();
  //hd::Engine::s_ptr engine = hd::Engine::Get ();
  hd::Window::s_ptr window = hd::Window::Create (800, 600, "HD1");
  window->output.On ([&window] (int aTime) {
    window->MakeCurrent ();
    glClear (GL_COLOR_BUFFER_BIT);
    window->Swap ();
  });
  window->input.Close.Void.On ([&window] () { window=NULL;});

  hd::Window::s_ptr win2 = hd::Window::Create (320, 200, "HD2");
  win2->input.Close.Void.On ([&win2] () { win2 = NULL; });
  // engine->configure (argc, argv);
  hd::Engine::Get ()->Start ();
  return 0;
}