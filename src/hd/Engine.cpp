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
#include "hd/Engine.hpp"
#include "hd/Debug.hpp"
#include "hd/Error.hpp"
#include "hd/sdl/GLContext.hpp"
#include "hd/sdl/Window.hpp"
#include "hd/sdl/events.hpp"
// Scene::Scene (SDL_Renderer *r) : renderer (r) {}
namespace hd {
  /**
   * @brief Keep track of time precisely.
   *
   */
  struct PrecisionTimer {
    Uint64 /** precision count **/ pc,
        /** precision counts per second **/ pcFps;
    /**
     * @brief Construct a new Precision Timer object
     *
     * @param p
     * @param f
     */
    PrecisionTimer (Uint64 p = SDL_GetPerformanceCounter (),
                    Uint64 f = SDL_GetPerformanceFrequency ())
        : pc (p), pcFps (f)
    {
    }
    /**
     * @brief convert into seconds.
     *
     * @return float
     */
    float
    ToSeconds ()
    {
      return (double)pc / (double)pcFps;
    }
    /**
     * @brief convert into milliseconds
     *
     * @return int
     */
    int
    ToMs ()
    {
      return ToSeconds () * 1000;
    }
    /**
     * @brief simply add these two together.
     *
     * @param o
     * @return PrecisionTimer
     */
    PrecisionTimer
    operator+ (PrecisionTimer &o)
    {
      return { pc + o.pc, (pcFps + o.pcFps) / 2 };
    }
    /**
     * @brief one time minus another
     *
     * @param o
     * @return PrecisionTimer
     */
    PrecisionTimer
    operator- (PrecisionTimer &o)
    {
      return { pc - o.pc, (pcFps + o.pcFps) / 2 };
    }
    /** which time came first? **/
    bool
    operator<(PrecisionTimer &o)
    {
      return pc < o.pc;
    }
    /** which time came last? **/
    bool
    operator> (PrecisionTimer &o)
    {
      return pc > o.pc;
    }
    /**
     * @brief test if two times are the same.
     *
     * @param o
     * @return true
     * @return false
     */
    bool
    operator== (PrecisionTimer &o)
    {
      return pc == o.pc;
    }
    /**
     * @brief advance this time
     *
     * @param o
     * @return PrecisionTimer
     */
    PrecisionTimer
    operator+= (PrecisionTimer &o)
    {
      pc += o.pc;
      pcFps = (pcFps + o.pcFps) / 2;
      return *this;
    }
  };
  /**
   * @brief Return a duration between two points in time.
   *
   * @param b
   * @param e
   * @return PrecisionTimer
   */

  PrecisionTimer
  PcDuration (PrecisionTimer &b, PrecisionTimer &e)
  {

    // PrecisionTimer &begin = (b.pc < e.pc) ? b : e, &end = (b.pc < e.pc) ? e
    // : b; return { begin.pc - end.pc, (begin.pcFps + end.pcFps) / 2 };
    return (b < e) ? e - b : b - e;
  }

  /**
   * @details hd mainloop, while `m_Quit` false do:
   * - input dispatching
   * - process dispatching
   * - output dispatching
   * - delay / wait
   * - repeat.
   * @todo make duration of input dispatching limitable?
   * @todo make duration of delay / wait limitable?
   ***/
  void
  Engine::FrameLoop ()
  {
    hdDebugCall (NULL);
    Uint64 frameCounter = 0;
    PrecisionTimer pcEpoch{}, totalFrame (0), totalHandle (0), totalStep (0),
        totalOutput (0), totalSleep (0);
    while (!m_Quit) {
      PrecisionTimer pcFrameStart{};
      int frameTicks = SDL_GetTicks ();
      HandleEvents ();
      PrecisionTimer pcHandle{};
      PrecisionTimer durHandle = PcDuration (pcFrameStart, pcHandle);
      totalHandle += durHandle;

      step.Trigger (frameTicks);
      PrecisionTimer pcStep{};
      PrecisionTimer durStep = PcDuration (pcHandle, pcStep);
      totalStep += durStep;
      // Uint64 frameStepTicks = SDL_GetPerformanceCounter ();
      output.Trigger (frameTicks);
      PrecisionTimer pcOutput{};
      PrecisionTimer durOutput = PcDuration (pcStep, pcOutput);
      totalOutput += durOutput;

      std::vector<int> eraseList = {};
      for (auto pair : atTicksNext) {
        if (pair.first <= frameTicks) {
          hdDebug ("Adding delayed callback to next step. %d <= %d",
                   pair.first,
                   frameTicks);
          for (auto cb : pair.second) {
            step.Once (cb);
          }
          eraseList.push_back (pair.first);
        }
      }
      for (int tickId : eraseList) {
        atTicksNext.erase (tickId);
      }
      // Uint64 thisOutputTicks = frameOutputTicks - frameStepTicks;
      // totalOutputTicks += thisOutputTicks;

      PrecisionTimer pcFrameEnd{};
      PrecisionTimer durFrame = PcDuration (pcFrameStart, pcFrameEnd);
      totalFrame += durFrame;
      frameCounter++;
      PrecisionTimer totalEpoch = PcDuration (pcEpoch, pcFrameEnd);
      if ((frameCounter % 60) == 0) {
        // hdLog ("frame#%lu ABSOLUTE start: %lx handle: %lx step: %lx output:
        // %lx",
        //        frameCounter,
        //       pcFrameStart.pc,
        //      pcHandle.pc,
        //     pcStep.pc,
        //    pcOutput.pc);
        hdLog (
            "frame#%lu DURATION frame: %lu handle: %lu step: %lu output: %lu",
            frameCounter,
            durFrame.pc,
            durHandle.pc,
            durStep.pc,
            durOutput.pc);
        hdLog ("frame#%lu: TOTALS epoch:%.3fs frame:%.3fs handle:%.3fs "
               "step:%.3fs output:%.3fs",
               frameCounter,
               totalEpoch.ToSeconds (),
               totalFrame.ToSeconds (),
               totalHandle.ToSeconds (),
               totalStep.ToSeconds (),
               totalOutput.ToSeconds ());
        hdLog ("frame#%lu PERCENTS\tfps: %.1f sleep: %.1f%% handle: %.1f%% "
               "step: %.1f%% "
               "output: %.1f%%",
               frameCounter,
               ((float)frameCounter / totalEpoch.ToSeconds ()),
               100.0f
                   - (((float)totalFrame.pc / (float)totalEpoch.pc) * 100.0f),
               ((float)totalHandle.pc / (float)totalFrame.pc) * 100.0f,
               ((float)totalStep.pc / (float)totalFrame.pc) * 100.0f,
               ((float)totalOutput.pc / (float)totalFrame.pc) * 100.0f);
      }

      SDL_Delay (10 /**SDL_framerateDelay (&fpsMan)**/);
    }
  }
  int
  Engine::Delay (int aMs, IntDispatch::Handler aHandler)
  {
    int whenTicks = SDL_GetTicks () + aMs;
    hdDebugCall ("%d @ %d", aMs, whenTicks);
    atTicksNext[whenTicks].push_back (aHandler);
    return whenTicks;
  }
  /**
   * @details process pending events until there are no more events.
   * @todo make how many events are process configurable (count/duration of
   *processing.)
   **/
  int
  Engine::HandleEvents ()
  {
    // hdDebugCall (NULL);
    int evtCnt = 0;
    SDL_Event e;
    while (SDL_PollEvent (&e) != 0) {
      sdl::events.Trigger (e);
    }
    return evtCnt;
  }
  /**
   * @brief weak_ptr reference to singleton instance.
   * @attention auto destructs upon last dereference.
   * @note windows keep a reference to engine. If a window reference is held
   * then at least one engine reference is kept alive.
   */
  std::weak_ptr<Engine> Engine::instance;
  /**
   * @brief attempts to initialize SDL.
   * @note creates a splash window in order to facilitate setting up opengl.
   * @todo research if its possible to avoid this splash window step? the goal
   * is to let the user define all windows and not have to define and destroy
   * one for this reason.
   *
   * @return Engine::s_ptr a smart_ptr reference to the constructed singleton
   * engine instance.
   * @return NULL on failure.
   */
  Engine::s_ptr
  Engine::Initialize ()
  {
    hdDebugCall (NULL);
    if (SDL_Init (SDL_INIT_EVERYTHING) != 0) {
      PrintSdlError ();
      return NULL;
    }
    int imgInitFlags
        = (IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
    if (IMG_Init (imgInitFlags) != imgInitFlags) {
      hdError ("Failed to initialize SDL_Image because: %s", IMG_GetError ());
      SDL_Quit ();
      return NULL;
    }
    if (TTF_Init () == -1) {
      IMG_Quit ();
      SDL_Quit ();
      hdError ("SDL_TTF failed to initialize because: %s\n", TTF_GetError ());
      return NULL;
    }
    int mixInitFlags
        = MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG;
    if (Mix_Init (mixInitFlags) == 0) {
      hdError ("Failed to initialize SDL_mixer because: %s", Mix_GetError ());
      TTF_Quit ();
      IMG_Quit ();
      SDL_Quit ();
    }
    if (Mix_OpenAudio (MIX_DEFAULT_FREQUENCY,
                       MIX_DEFAULT_FORMAT,
                       MIX_DEFAULT_CHANNELS,
                       4096)
        != 0) {
      hdError ("Failed to open audio because: %s", Mix_GetError ());
    };
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK,
                         SDL_GL_CONTEXT_PROFILE_CORE);
    s_ptr mounted = s_ptr (new Engine ());
    instance = mounted;
    // Apparently in order to initializes GLEW we *MUST* have a window created
    sdl::Window splashWindow = sdl::Window::Create (SDL_WINDOW_OPENGL);
    if (!splashWindow) {
      hdError ("Failed to create splash window.");
      mounted = NULL;
      return NULL;
    }
    // Ensure we have an OpenGL Context...
    sdl::GLContext context = sdl::GLContext::Create (splashWindow);
    splashWindow.MakeCurrent (context);
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit ();
    if (glewError != GLEW_OK) {
      hdError ("Failed to initialize GLEW because: %s\n",
               glewGetErrorString (glewError));
      mounted = NULL;
      return NULL;
    }
    /**if (SDL_GL_SetSwapInterval (1) < 0) {
      hdError ("Warning: Unable to set VSync because: %s", SDL_GetError ());
    }**/
    return mounted;
  }

  /**
   * @brief Access the Engine singleton;
   * @attention any reference attempts to initialize it.
   * @attention shuts down if there are no references.
   * @return Engine::s_ptr If the engine fails to initialize this will be
   *`NULL`.
   * @return Engine::s_ptr if the engine has been initialized this will be a
   *smart_ptr reference to it.
   **/
  Engine::s_ptr
  Engine::Get ()
  {
    s_ptr mounted = instance.lock ();
    if (mounted == NULL) {
      mounted = Initialize ();
    }
    return mounted;
  }
  /** @brief helper to print out SDL_Version structures. **/
  void
  Engine::PrintVersion (const SDL_version *v, const char *of, FILE *aFile)
  {
    fprintf (aFile, "%s version %d.%d.%d\n", of, v->major, v->minor, v->patch);
  }
  /**
   * @brief helper to print out versions of libraries used by this program
   * @todo STANDING this should be kept in sync with what libraries are most
   *being relied upon.
   * @param aFile FILE* pointer to where to write this; defaults to `stdout`
   **/
  void
  Engine::PrintVersions (FILE *aFile)
  {
    fprintf (aFile, "HD version %s\n", PROJECT_VERSION);
    SDL_version sdlver;
    SDL_GetVersion (&sdlver);
    PrintVersion (&sdlver, "SDL", aFile);
    PrintVersion (IMG_Linked_Version (), "SDL_image", aFile);
    PrintVersion (TTF_Linked_Version (), "SDL_TTF", aFile);
    PrintVersion (Mix_Linked_Version (), "SDL_mixer", aFile);
  }
  /** Engine construction connects Quit function to SDL Quit Event **/
  Engine::Engine ()
  {
    hdDebugCall (NULL);
    sdl::events.quit.Void.On (Quit);
  }
  /**
   * @brief perminate store of the last int value passed to `Configure`
   *
   */
  int Engine::m_Argc = 0;
  /**
   * @brief perminate store of teh last char** value passed to `Configure`
   *
   */
  char **Engine::m_Argv = NULL;
  // Engine::Engine (int argc, char **argv) { configure (argc, argv); }
  /** Setup some basic options based on the command line **/
  void
  Engine::Configure (int argc, char **argv)
  {
    m_Argc = argc;
    m_Argv = argv;
    hdDebugCall ("argc=%d, argv=%p", argc, argv);
    // Shared::searchPaths.clear ();
    searchPaths.push_back (std::filesystem::canonical (
        std::filesystem::path (argv[0]).parent_path () / "../assets"));
  }
  char *
  Engine::GetProgramName ()
  {
    if (m_Argc > 0) {
      return m_Argv[0];
    } else {
      return NULL;
    }
  }
  /** **/
  Engine::~Engine ()
  {
    hdDebugCall (NULL);
    Shutdown ();
  }
  /** **/

  // void
  // Engine::Quit ()
  //{
  //   hdDebugCall (NULL);
  //   m_Quit = true;
  // }
  /** **/

  void
  Engine::Start ()
  {
    hdDebugCall (NULL);
    m_Quit = false;
    FrameLoop ();
  }

  /** **/
  void
  Engine::Shutdown ()
  {
    hdDebugCall (NULL);
    if (instance.lock ()) {
      instance.lock ()->Quit ();
    }
    Mix_Quit ();
    TTF_Quit ();
    IMG_Quit ();
    SDL_Quit ();
  }

  /** **/
  void
  Engine::PrintSdlError (const char *msg)
  {
    const char *DEFAULT = "SDL Error";
    const char *MSG;
    if (msg != NULL) {
      MSG = msg;
    } else {
      MSG = DEFAULT;
    }

    fprintf (stderr, "%s: %s\n", MSG, SDL_GetError ());
  }
  /**
   * @brief Searches a few extra places where the file could be.
   * @details
   *  Locate the canonical path for the given path.
   *If the path is already absolute; it will simply return the same path
   *given. If it is not, it will attempt to find the actual file by appending
   *searchPaths and checking if it exists; then returning the 'canonical' path.
   * @param aPath
   * @return std::filesystem::path
   * @return aPath IF `aPath` is already absolute OR it cannot find this path
   *within `searchPaths`
   **/
  std::filesystem::path
  Engine::FindPath (std::filesystem::path aPath,
                    std::list<std::filesystem::path> &aSearchPaths)
  {
    if (aPath.is_absolute ()) {
      return aPath;
    }
    std::filesystem::path realPath = aPath;
    for (auto const &base : aSearchPaths) {
      std::filesystem::path thisPath = base / realPath;
      // printf ("Checking for '%s'...\n", thisPath.generic_string ().c_str
      // ());
      if (std::filesystem::exists (thisPath)) {
        return std::filesystem::canonical (thisPath);
      }
    }
    return aPath;
  }
  /**
   * @brief a list of paths to search for a file.
   **/
  std::list<std::filesystem::path> Engine::searchPaths ({
#if HD_DEBUG_BUILD == 1
    HD_SOURCE_ASSETS,
#endif
        "./"
  });
} // namespace hd