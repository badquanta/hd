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
#include "hd/Window.hpp"
#include "hd/sdl/MixerChunk.hpp"
#include "hd/sdl/MixerChannel.hpp"

using hd::Engine;
using hd::Window;
using hd::sdl::MixerChannel;
using hd::sdl::MixerChunk;

Window::s_ptr window;

void
doWindowOutput ()
{
  assert (window); // should only be called while the window isn't null.
  window->MakeCurrent ();
  glClear (GL_COLOR_BUFFER_BIT);
  window->Swap ();
}
void
doWindowClosed ()
{
  if(window!=NULL){
    window->step.Void.Once ([] () { window = NULL; });
  }
}

int
main (int argc, char **argv)
{
  Engine::PrintVersions ();
  Engine::Configure (argc, argv);

  // Engine::s_ptr engine = Engine::Get ();
  window = Window::Create (800, 600, "play some audio");
  window->output.Void.On (&doWindowOutput);
  window->input.Close.Void.On (&doWindowClosed);

  MixerChunk::s_ptr horrorAmbient = MixerChunk::Load("audio/horror ambient.ogg");

  if(!horrorAmbient){
    hdError("Unable to load audio chunk.");
    return 1;
  }

  horrorAmbient->FadeIn (1000);

  Engine::Get ()->Start ();
  return 0;
}
