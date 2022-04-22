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
#include "hd/Program.hpp"

// Scene::Scene (SDL_Renderer *r) : renderer (r) {}
/** **/
namespace hd {
  /** Some boring details about how to construct an application. **/
  Program::Program () {}
  Program::Program (int argc, char **argv) { configure (argc, argv); }
  /** Setup some basic options based on the command line **/
  void
  Program::configure (int argc, char **argv)
  {
    on.reset ();
    on.type.appendListener (SDL_QUIT,
                            [this] (const SDL_Event &e) { quit = true; });
    Shared::searchPaths.clear ();
    Shared::searchPaths.push_back (std::filesystem::canonical (
        std::filesystem::path (argv[0]).parent_path () / "../assets"));
  }
  /** **/
  Program::~Program () {}
  /** **/
  void
  Program::setScene (Scene *s)
  {
    currentScene = s;
    // s->load (renderer);
  }
  /** **/
  void
  Program::frameLoop ()
  {
    quit = false;
    while (!quit) {
      handleEvents ();
      // SDL_Renderer *renderer = SDL_GetRenderer (window);
      glClearColor (0.f, 0.f, 0.f, 1.f);
      glClear (GL_COLOR_BUFFER_BIT);
      // SDL_RenderPresent (renderer);
      //glUseProgram (programId);
      shaderProgram.bind ();
      glEnableVertexAttribArray (vertexPos2dLocation);
      glBindBuffer (GL_ARRAY_BUFFER, vbo);
      glVertexAttribPointer (vertexPos2dLocation,
                             2,
                             GL_FLOAT,
                             GL_FALSE,
                             2 * sizeof (GLfloat),
                             NULL);
      glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ibo);
      glDrawElements (GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);
      glDisableVertexAttribArray (vertexPos2dLocation);
      glUseProgram (0);
      SDL_GL_SwapWindow (window);
      // SDL_RenderPresent (renderer);
      SDL_Delay (10 /**SDL_framerateDelay (&fpsMan)**/);
    }
  }
  /** **/
  int
  Program::handleEvents ()
  {
    int evtCnt = 0;
    SDL_Event e;
    while (SDL_PollEvent (&e) != 0) {
      // on.dispatch (e);
      on (e);
      currentScene->on (e);
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
  Program::startup ()
  {
    if (SDL_Init (SDL_INIT_EVERYTHING) != 0) {
      printSdlError ();
      return false;
    }
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK,
                         SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_initFramerate (&fpsMan);
    SDL_setFramerate (&fpsMan, FPS_UPPER_LIMIT);
    window = SDL_CreateWindow ("hd opengl window",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               800,
                               800,
                               SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
                                   | SDL_WINDOW_RESIZABLE);
    // SDL_CreateWindowAndRenderer (
    //     320, 200, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, &window,
    //     &renderer);
    if ((window == NULL)) {
      printSdlError ();
      return false;
    }
    /*
    renderer = SDL_CreateRenderer (window, -1, 0);
    if (renderer == NULL) {
      printSdlError ();
      SDL_Quit ();
      return false;
    }
    */
    context = SDL_GL_CreateContext (window);
    if (context == NULL) {
      printf ("OpenGL context could not be created: %s\n", SDL_GetError ());
      SDL_Quit ();
      return false;
    }
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit ();
    if (glewError != GLEW_OK) {
      printf ("Failed to initialize GLEW because: %s\n",
              glewGetErrorString (glewError));
      SDL_Quit ();
      return false;
    }
    if (SDL_GL_SetSwapInterval (1) < 0) {
      printf ("Warning: Unable to set VSync because: %s", SDL_GetError ());
    }
    if(!shaderProgram.create ()){
      fprintf (stderr, "Failed to create Shader Program\n");
      SDL_Quit ();
      return false;
    };
    gl::Shader vertexShader (GL_VERTEX_SHADER);
    // programId = glCreateProgram ();
    // GLuint vertexShader = glCreateShader (GL_VERTEX_SHADER);
    //const GLchar *vertexShaderSource
    //    = { "#version 140\n"
    //        "in vec2 LVertexPos2D;"
    //        "void main(){"
    //        "gl_Position = vec4(LVertexPos2D.x, LVertexPos2D.y,0,1);"
    //       "}" };
    if(!vertexShader.loadSource ("shaders/default.vert")){
      fprintf (stderr, "Failed to load source for vertex shader.\n");
    };
    //vertexShader.setSource (vertexShaderSource);
    // glShaderSource (vertexShader, 1, vertexShaderSource, NULL);
    if (!vertexShader.compile ()) {
      fprintf (stderr, "Failed to compile vertex shader.\n");
      vertexShader.printLog (stderr);
      SDL_Quit ();
      return false;
    }
    // glCompileShader (vertexShader);
    // GLint vShaderCompiled = GL_FALSE;
    // glGetShaderiv (vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
    // if (vShaderCompiled != GL_TRUE) {
    //   printf ("Unable to compile vertex shader %d\n", vertexShader);
    //   printShaderLog (vertexShader);
    //   SDL_Quit ();
    //   return false;
    // }
    shaderProgram.attach (vertexShader);
    // glAttachShader (programId, vertexShader);
    gl::Shader fragmentShader (GL_FRAGMENT_SHADER);
    // GLuint fragmentShader = glCreateShader (GL_FRAGMENT_SHADER);
    //const GLchar *fragmentShaderSource = "#version 140\n"
    //                                     "out vec4 LFragment;"
    //                                     "void main(){"
    //                                     "LFragment = vec4(1.0,1.0,1.0,1.0);"
    //                                     "}";
    // glShaderSource (fragmentShader, 1, fragmentShaderSource, NULL);
    //fragmentShader.setSource (fragmentShaderSource);
    fragmentShader.loadSource ("shaders/default.frag");
    // glCompileShader (fragmentShader);
    if(!fragmentShader.compile()){
      fprintf (stderr, "Failed to compile fragment shader.\n");
      fragmentShader.printLog (stderr);
      SDL_Quit ();
      return false;
    }
    //GLint fShaderCompiled = GL_FALSE;
    //glGetShaderiv (fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
    //if (fShaderCompiled != GL_TRUE) {
    //  printf ("Unable to compile fragment shader %d!\n", fragmentShader);
    //  printShaderLog (fragmentShader);
    //  SDL_Quit ();
    //  return false;
    //}
    //glAttachShader (programId, fragmentShader);
    shaderProgram.attach (fragmentShader);
    //glLinkProgram (programId);
    if(!shaderProgram.link()){
      fprintf (stderr, "Failed to link shader program.\n");
      shaderProgram.printLog (stderr);
    }
    //GLint programLinkStatus = GL_TRUE;
    //glGetProgramiv (programId, GL_LINK_STATUS, &programLinkStatus);
    //if (programLinkStatus != GL_TRUE) {
    //  printf ("Error linking GL program %d!\n", programId);
    //  printProgramLog (programId);
    //  return false;
    // }
    //vertexPos2dLocation = glGetAttribLocation (programId, "LVertexPos2D");
    vertexPos2dLocation = shaderProgram.getAttribLocation ("aPos");
    if (vertexPos2dLocation == -1) {
      printf ("LVertexPos2D is not a valid glsl program variable!\n");
      return false;
    }
    glClearColor (0.f, 0.f, 0.f, 1.f);
    GLfloat vertexData[]
        = { -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f };
    GLuint indexData[] = { 0, 1, 2, 3 };
    // create vao
    glGenVertexArrays (1, &vao);
    // create vbo
    glGenBuffers (1, &vbo);

    // bind the vao
    glBindVertexArray (vao);
    // bind the vbo
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (
        GL_ARRAY_BUFFER, sizeof (vertexData), vertexData, GL_STATIC_DRAW);
    // create ibo
    glGenBuffers (1, &ibo);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER,
                  sizeof (indexData),
                  indexData,
                  GL_STATIC_DRAW);

    int imgInitFlags
        = (IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
    if (IMG_Init (imgInitFlags) != imgInitFlags) {
      printf ("Failed to initialize SDL_Image because: %s\n", IMG_GetError ());
      SDL_Quit ();
      return false;
    }
    if (TTF_Init () == -1) {
      IMG_Quit ();
      SDL_Quit ();
      printf ("SDL_TTF failed to initialize because: %s\n", TTF_GetError ());
      return false;
    }
    return true;
  }
  /** **/
  void
  Program::shutdown ()
  {
    if (currentScene != NULL) {
      currentScene->unload ();
      currentScene = NULL;
    }
    if (window != NULL) {
      SDL_DestroyWindow (window);
      window = NULL;
    }
    if (SDL_WasInit (SDL_INIT_EVERYTHING) != 0) {
      SDL_Quit ();
    }
  }
  /** **/
  void
  Program::printSdlError (const char *msg)
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

  void
  Program::printProgramLog (GLuint aId)
  {
    if (glIsProgram (aId)) {
      int infoLogLength = 0;
      int maxLength = 0;
      glGetProgramiv (aId, GL_INFO_LOG_LENGTH, &maxLength);
      char *infoLog = new char[maxLength];
      glGetProgramInfoLog (aId, maxLength, &infoLogLength, infoLog);
      if (infoLogLength > 0) {
        printf ("%s\n", infoLog);
      }
      delete[] infoLog;
    } else {
      printf ("Name %d is not a program.\n", aId);
    }
  }
  void
  Program::printShaderLog (GLuint aId)
  {
    if (glIsShader (aId)) {
      int infoLogLength = 0;
      int maxLength = 0;
      glGetShaderiv (aId, GL_INFO_LOG_LENGTH, &maxLength);
      char *infoLog = new char[maxLength];
      glGetShaderInfoLog (aId, maxLength, &infoLogLength, infoLog);
      if (infoLogLength > 0) {
        printf ("%s\n", infoLog);
      } else {
        printf ("Name %d is not a shader.\n", aId);
      }
    }
  }
} // namespace hd