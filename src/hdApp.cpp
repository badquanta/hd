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
#include "hdApp.hpp"

// Scene::Scene (SDL_Renderer *r) : renderer (r) {}
namespace hd
{
  /** Some boring details about how to construct an application. **/
  App::App () {
    on.append (onType);
    onType.appendListener (SDL_QUIT,
                           [this] (const SDL_Event &e) { quit = true; });
    onType.appendListener (SDL_KEYUP, onKey);
    onType.appendListener (SDL_KEYDOWN, onKey);
  }
  /** **/
  App::~App () {}
  /** **/
  void
  App::setScene (Scene *s)
  {
    currentScene = s;
    s->load (renderer);
  }
  /** **/
  void
  App::frameLoop ()
  {
    quit = false;
    while (!quit)
      {
        handleEvents ();
        SDL_Renderer *renderer = SDL_GetRenderer (window);
        if (renderer != NULL)
          {
            currentScene->render (renderer);
            SDL_RenderPresent (renderer);
            SDL_Delay (SDL_framerateDelay (&fpsMan));
          }
        else
          {
            printSdlError ();
          }
      }
  }
  /** **/
  int
  App::handleEvents ()
  {
    int evtCnt = 0;
    SDL_Event e;
    while (SDL_PollEvent (&e) != 0)
      {
        //on.dispatch (e);
        on (e);
        // switch (e.type)
        //   {
        //   case SDL_QUIT:
        //     quit = true;
        //     break;
        //  default:
        //   if (currentScene != NULL)
        //    {
        //     currentScene->handleEvent (e);
        //  }
        // }
      }
    return evtCnt;
  }
  /** **/
  bool
  App::startup ()
  {
    if (SDL_Init (SDL_INIT_EVERYTHING) != 0)
      {
        printSdlError ();
        return false;
      }
    SDL_initFramerate (&fpsMan);
    SDL_setFramerate (&fpsMan, FPS_UPPER_LIMIT);
    SDL_CreateWindowAndRenderer (
        320, 200, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, &window, &renderer);
    if ((window == NULL) || (renderer == NULL))
      {
        printSdlError ();
        return false;
      }
    int imgInitFlags
        = (IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
    if (IMG_Init (imgInitFlags) != imgInitFlags)
      {
        printf ("Failed to initialize SDL_Image because: %s\n",
                IMG_GetError ());
        return false;
      }
    return true;
  }
  /** **/
  void
  App::shutdown ()
  {
    if (currentScene != NULL)
      {
        currentScene->unload ();
        currentScene = NULL;
      }
    if (window != NULL)
      {
        SDL_DestroyWindow (window);
        window = NULL;
      }
    if (SDL_WasInit (SDL_INIT_EVERYTHING) != 0)
      {
        SDL_Quit ();
      }
  }
  /** **/
  void
  App::printSdlError (const char *msg)
  {
    const char *DEFAULT = "SDL Error";
    const char *MSG;
    if (msg != NULL)
      {
        MSG = msg;
      }
    else
      {
        MSG = DEFAULT;
      }

    fprintf (stderr, "%s: %s\n", MSG, SDL_GetError ());
  }
} // namespace hd