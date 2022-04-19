#pragma once
/*
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
#include "hd/Events.hpp"
#include "hd/Scene.hpp"
#include "hd/Shared.hpp"
#include <stdio.h>

/**
 * @brief holodeck?
 */
namespace hd
{

  /**
   * @brief Apply what we know
   */
  class Program
  {
  public:
    /** Start applying **/
    Program ();
    Program (int, char **);
    /** Finish applying **/
    ~Program ();
    /** Configure **/
    void configure (int, char **);
    /** Start our work.**/
    bool startup ();
    /** Clean up our workspace.**/
    void shutdown ();
    /** Perform a step of our work.**/
    void frameLoop ();
    /** Change our workspace.**/
    void setScene (Scene *);
    /** Print out what went wrong with SDL2.
     * \deprecated someone else should just do this. **/
    static void printSdlError (const char *msg = NULL);
    /** Callback list for any/all events. **/
    sdl::event::Tree on;
    /** Event Type Dispatcher **/
    sdl::event::TypeDispatcher onType;
    /** Event Dispatcher for KEY{UP,DOWN} events by Key Code. **/
    sdl::event::key::CodeDispatcher onKey;

  private:
    /** Ensure we are responsive to changes to our environment. */
    int handleEvents ();
    /** Where are we? */
    Scene *currentScene;
    /** How fast are we working?*/
    FPSmanager fpsMan;
    /** What can we see?*/
    SDL_Window *window;
    /** How do we see? */
    SDL_Renderer *renderer;
    /** Should we keep on working? */
    bool quit = false;
  };
} // namespace hd