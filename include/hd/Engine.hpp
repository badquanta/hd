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
#include "hd/Shared.hpp"
#include "hd/evt/EngineDispatch.hpp"
#include "hd/evt/IntDispatch.hpp"
#include <memory>
/**
 * @brief holodeck?
 */
namespace hd {
  /**
   * @brief Initializes SDL, helps finds files, and dispatches events.
   */
  class Engine {
  public:
    typedef std::shared_ptr<Engine> Mount;
    /**
     * @brief Initializes or simply returns an already initialized engine.
     *
     * @return Ptr `std::shared_ptr<Engine>`, NULL if initialization error.
     */
    static Mount Get ();
    static void PrintVersion (const SDL_version *,
                              const char *of = "SDL",
                              FILE *aFile = stdout);
    static void PrintVersions (FILE *aFile = stdout);
    /** Sets up engine options from Command Line Environment **/
    static void Configure (int, char **);
    /** Returns the program's name, `Configure()` needs to have been called.
     *  @return A character string with the program's name.
     *  @return "" if `Configure(argc, argv)` has not be called
     * **/
    static const char *ProgramName ();
    /** Finish applying **/
    ~Engine ();
    /** Signal to the engine we wish to exit the main loop **/
    void Quit ();
    void Start ();
    /** Print out what went wrong with SDL2.
     * \deprecated someone else should just do this. **/
    static void PrintSdlError (const char *msg = NULL);
    /** Callback list for any/all events. **/
    evt::EngineDispatch on;
    evt::IntDispatch eachFrame;
    evt::VoidDispatch beforeStart;
    /** @todo remove from Engine gl::Camera camera; **/
  private:
    /**
     * @brief Construct a new Engine object
     * @attention This method is private and one needs to call it through
     * `Engine::Get()`
     */
    Engine ();
    /** Singleton instance storage **/
    static std::weak_ptr<Engine> instance;
    /** Pre-constructor initialization, called by Get() before construction.
     * **/
    static Mount Initialize ();
    /** Clean up our workspace.**/
    static void Shutdown ();
    /** Perform a step of our work.**/
    void FrameLoop ();
    /** Ensure we are responsive to changes to our environment. */
    int HandleEvents ();
    /** Should we keep on working? */
    bool quit = false;
  };
} // namespace hd