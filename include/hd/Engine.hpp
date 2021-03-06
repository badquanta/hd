/**
 * @file Engine.hpp
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
#pragma once
#include "hd/Log.hpp"
#include "hd/sdl/events.hpp"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_version.h>
#include <filesystem>
#include <list>
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
    /** Refer to the engine via smart pointers. **/
    typedef std::shared_ptr<Engine> s_ptr;
    /**
     * @brief Initializes or simply returns an already initialized engine.
     *
     * @return s_ptr `std::shared_ptr<Engine>`, NULL if initialization error.
     */
    static s_ptr Get ();
    /** Print an SDL_version structure to aFile
     * @param v `SDL_Version*`
     * @param aFile `*FILE` defaults to `stdout`
     * @param of `const char*` defaults to `"SDL"`.
     **/
    static void PrintVersion (const SDL_version *,
                              const char *of = "SDL",
                              FILE *aFile = stdout);
    /** Gather and print out all of the versions of libraries used by hd? **/
    static void PrintVersions (FILE *aFile = stdout);
    /** Sets up engine options from Command Line Environment **/
    static void Configure (int, char **);
    /** Returns the program's name, `Configure()` needs to have been called.
     *  @return A character string with the program's name.
     *  @return "" if `Configure(argc, argv)` has not be called
     * **/
    static char *GetProgramName ();
    /** These paths will be utilized automatically if no paths are
     * specified when `FindPath` is called.**/
    static std::list<std::filesystem::path> searchPaths;
    /** Test if `aPath` exists. If it does it simply returns the path given;
     * however if it does not this function attempts to append the file to
     *`aSearchPaths`.
     **/
    static std::filesystem::path
    /**
     * @brief Construct a new Find Path object
     *
     * @param aSearchPaths
     */
    FindPath (std::filesystem::path,
              std::list<std::filesystem::path> &aSearchPaths = searchPaths);

    /** Finish applying **/
    ~Engine ();
    /** Signal to the engine we wish to exit the main loop **/
    // void Quit ();
    const std::function<void ()> Quit = [this] () { m_Quit = true; };
    /** Run the engine. **/
    void Start ();
    /** Print out what went wrong with SDL2.
     * \deprecated someone else should just do this. **/
    static void PrintSdlError (const char *msg = NULL);
    /** Callback list for logical processing. **/
    IntDispatch step;
    /** Callback list for rendering. **/
    IntDispatch output;
    /** Register something to happen later. **/
    int Delay (int aMs, IntDispatch::Handler);

  private:
    /** Keeps a copy of the argc & argv for interegation **/
    static int m_Argc;
    /** @copydoc m_Argc **/
    static char **m_Argv;
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
    static s_ptr Initialize ();
    /** Clean up our workspace.**/
    static void Shutdown ();
    /** Perform a step of our work.**/
    void FrameLoop ();
    /** **/
    std::map<int, std::list<IntDispatch::Handler> > atTicksNext;

    /** Ensure we are responsive to changes to our environment. */
    int HandleEvents ();
    /** Should we keep on working? */
    bool m_Quit = false;
  };
} // namespace hd