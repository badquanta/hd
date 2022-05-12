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
#include "hd/sdl/GLContext.hpp"
#include "hd/sdl/MixerChannel.hpp"
#include "hd/sdl/MixerChunk.hpp"
#include "hd/sdl/Window.hpp"
#include <cassert>
using hd::Engine;
using hd::sdl::GLContext;
using hd::sdl::MixerChannel;
using hd::sdl::MixerChunk;
using hd::sdl::Window;

hd::sdl::Window window;
GLContext glContext;
hd::Engine::s_ptr engine;

void
doWindowOutput ()
{
  assert (window); // should only be called while the window isn't null.
  window.MakeCurrent (glContext);
  glClear (GL_COLOR_BUFFER_BIT);
  window.Swap ();
}
void
doWindowClosed ()
{
  if (window) {
    engine->step.Void.Once ([] () { window.m_IDENTITY = NULL; });
  }
}

int
main (int argc, char **argv)
{
  Engine::PrintVersions ();
  Engine::Configure (argc, argv);

  // Engine::s_ptr engine = Engine::Get ();
  window = Window::Create (800, 600, "play some audio");

  glContext = GLContext::Create (window.ptr.get ());
  engine->output.Void.On (&doWindowOutput);
  window.Event().Close.Void.On (&doWindowClosed);

  MixerChunk horrorAmbient
      = MixerChunk::Load ("audio/horror ambient.ogg");
  if (!horrorAmbient) {
    hdError ("Unable to load audio chunk.");
    return 1;
  }
  MixerChannel defaultChannel = -1, s_Channel;

  window.Event().Key.Keycode[SDLK_s].Up.Void.On (
      [&] () { s_Channel = horrorAmbient.Play (-1); });
  window.Event().Key.Keycode[SDLK_m].Up.Void.On (
      [&] () { defaultChannel.FadeOut (100); });

  window.engine->Delay (5000, [&horrorAmbient] (int) {
    auto channel = horrorAmbient.FadeIn (1000, 0);
    window.engine->Delay (2000, [channel] (int) {
      hdLog ("Delayed callback.");
      channel.FadeOut (500);
    });
  });
  window.engine->Delay (16000, [&] (int) { horrorAmbient.Play (); });

  Engine::Get ()->Start ();
  return 0;
}
