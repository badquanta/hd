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
#include "hd/SplashScene.hpp"

void
printVersion (const SDL_version *v, const char *of = "SDL")
{
  printf ("%s version %d.%d.%d\n", of, v->major, v->minor, v->patch);
}
void
printVersions ()
{
  SDL_version sdlver;
  SDL_GetVersion (&sdlver);
  printVersion (&sdlver);
  printVersion (IMG_Linked_Version (), "SDL_image");
  printVersion (TTF_Linked_Version (), "SDL_TTF");
}

int
main (int argc, char **argv)
{
  printVersions ();
  hd::Engine program (argc, argv);
  hd::SplashScene splash;
  if (program.startup ())
    {
      program.on.Key.Keycode[SDLK_UP].Add([] (const SDL_Event &e) { printf ("Got Up key.\n"); });
      program.setScene (&splash);
      program.frameLoop ();
      program.shutdown ();
    };
  return 0;
}