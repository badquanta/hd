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
#include "hd/Window.hpp"

// Scene::Scene (SDL_Renderer *r) : renderer (r) {}
namespace hd {
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
    while (!m_Quit) {
      int frameStartTicks = SDL_GetTicks ();
      HandleEvents ();
      // SDL_Renderer *renderer = SDL_GetRenderer (window);
      process.Trigger (frameStartTicks);
      output.Trigger (frameStartTicks);
      // SDL_RenderPresent (renderer);
      SDL_Delay (10 /**SDL_framerateDelay (&fpsMan)**/);
    }
  }
  /**
   * @details process pending events until there are no more events.
   * @todo make how many events are process configurable (count/duration of processing.)
   **/
  int
  Engine::HandleEvents ()
  {
    // hdDebugCall (NULL);
    int evtCnt = 0;
    SDL_Event e;
    while (SDL_PollEvent (&e) != 0) {
      input.Trigger (e);
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
   * @return Engine::Ptr a smart_ptr reference to the constructed singleton
   * engine instance.
   * @return NULL on failure.
   */
  Engine::Ptr
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
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK,
                         SDL_GL_CONTEXT_PROFILE_CORE);
    Ptr mounted = Ptr (new Engine ());
    instance = mounted;
    // Apparently in order to initializes GLEW we *MUST* have a window created
    Window::Ptr splashWindow = Window::Create (320, 200, "Splash!");
    if (!splashWindow) {
      hdError ("Failed to create splash window.");
      mounted = NULL;
      return NULL;
    }
    // Ensure we have an OpenGL Context...
    splashWindow->MakeCurrent ();
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit ();
    if (glewError != GLEW_OK) {
      hdError ("Failed to initialize GLEW because: %s\n",
               glewGetErrorString (glewError));
      mounted = NULL;
      return NULL;
    }
    if (SDL_GL_SetSwapInterval (1) < 0) {
      hdError ("Warning: Unable to set VSync because: %s", SDL_GetError ());
    }
    return mounted;
  }

  /**
   * @brief Access the Engine singleton;
   * @attention any reference attempts to initialize it.
   * @attention shuts down if there are no references.
   * @return Engine::Ptr If the engine fails to initialize this will be `NULL`.
   * @return Engine::Ptr if the engine has been initialized this will be a
   *smart_ptr reference to it.
   **/
  Engine::Ptr
  Engine::Get ()
  {
    Ptr mounted = instance.lock ();
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
  }
  /** Some boring details about how to construct an application. **/
  Engine::Engine () /* @todo :  remove camera (glm::vec3 (0.0f, 0.0f, 1.0f)) */
  {
    hdDebugCall (NULL);
    input.Quit.On ([this] (const SDL_Event &e) { m_Quit = true; });
  }
  int Engine::m_Argc = 0;

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
    Shared::searchPaths.push_back (std::filesystem::canonical (
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

  void
  Engine::Quit ()
  {
    hdDebugCall (NULL);
    m_Quit = true;
  }
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

} // namespace hd