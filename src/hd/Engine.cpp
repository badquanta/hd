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

// Vertices coordinates
GLfloat vertexData[] = {
  //     COORDINATES/        COLORS   /   TexCoord  //
  -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // Lower left corner
  -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // Upper left corner
  0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // Upper right corner
  0.5f,  -0.5f, 0.0f, 0.05f, 0.05f, 0.05f, 1.0f, 0.0f  // Lower right corner
};

// Indices for vertices order
GLuint indexData[] = {
  0, 2, 1, // Upper triangle
  0, 3, 2  // Lower triangle
};
// Scene::Scene (SDL_Renderer *r) : renderer (r) {}
/** **/
namespace hd {
  /** Some boring details about how to construct an application. **/
  Engine::Engine () {}
  Engine::Engine (int argc, char **argv) { configure (argc, argv); }
  /** Setup some basic options based on the command line **/
  void
  Engine::configure (int argc, char **argv)
  {
    on.reset ();
    on.type.appendListener (SDL_QUIT,
                            [this] (const SDL_Event &e) { quit = true; });
    on.type.appendListener (SDL_WINDOWEVENT, [this] (const SDL_Event &e) {
      // printf ("WINDOW EVENT\n");
      if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
        printf (
            "WINDOW SIZE CHANGED TO %dx%d\n", e.window.data1, e.window.data2);
        glViewport (0, 0, e.window.data1, e.window.data2);
      }
    });
    // Shared::searchPaths.clear ();
    Shared::searchPaths.push_back (std::filesystem::canonical (
        std::filesystem::path (argv[0]).parent_path () / "../assets"));
  }
  /** **/
  Engine::~Engine () {}
  /** **/
  void
  Engine::setScene (Scene *s)
  {
    currentScene = s;
    // s->load (renderer);
  }
  /** **/
  void
  Engine::frameLoop ()
  {
    quit = false;
    while (!quit) {
      handleEvents ();
      // SDL_Renderer *renderer = SDL_GetRenderer (window);
      glClearColor (0.f, 0.f, 0.f, 1.f);
      glClear (GL_COLOR_BUFFER_BIT);
      // SDL_RenderPresent (renderer);
      // glUseProgram (programId);
      shaderProgram.Bind ();
      glUniform1f (scaleLocation, 0.5f);
      glUniform1i (tex0Uni, 0);
      glBindTexture (GL_TEXTURE_2D, texture);
      vbo.Bind ();
      // glVertexAttribPointer (
      //     aPos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (GLfloat), (void *)0);
      // glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ibo);
      ebo.Bind ();
      glDrawElements (GL_TRIANGLES, 9, GL_UNSIGNED_INT, NULL);
      // glDisableVertexAttribArray (aPos);
      shaderProgram.unbind ();
      SDL_GL_SwapWindow (window);
      // SDL_RenderPresent (renderer);
      SDL_Delay (10 /**SDL_framerateDelay (&fpsMan)**/);
    }
  }
  /** **/
  int
  Engine::handleEvents ()
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
  Engine::startup ()
  {
    if (SDL_Init (SDL_INIT_EVERYTHING) != 0) {
      printSdlError ();
      return false;
    }
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
    if ((window == NULL)) {
      printSdlError ();
      return false;
    }
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
    if (!shaderProgram.create ("shaders/default.vert",
                               "shaders/default.frag")) {
      fprintf (stderr, "Failed to create Shader Program\n");
      SDL_Quit ();
      return false;
    };
    scaleLocation = shaderProgram.getUniformLocation ("scale");
    tex0Uni = shaderProgram.getUniformLocation ("tex0");
    glClearColor (0.f, 0.f, 0.f, 1.f);
    // create vao
    vao.Create ();
    // bind the vao
    vao.Bind ();
    //  create vbo
    vbo.Create (vertexData, sizeof (vertexData));
    // bind the vbo
    vbo.Bind ();
    // create ibo
    ebo.Create (indexData, sizeof (indexData));
    shaderProgram.printAttribues (stdout);
    if ((!vao.LinkAttrib (vbo,
                          shaderProgram,
                          "aPos",
                          3,
                          GL_FLOAT,
                          8 * sizeof (GLfloat),
                          (void *)0))
        || (!vao.LinkAttrib (vbo,
                             shaderProgram,
                             "aColor",
                             3,
                             GL_FLOAT,
                             8 * sizeof (GLfloat),
                             (void *)(3 * sizeof (GLfloat))))
        || (!vao.LinkAttrib (vbo,
                             shaderProgram,
                             "aTex",
                             2,
                             GL_FLOAT,
                             8 * sizeof (GLfloat),
                             (void *)(6 * sizeof (GLfloat))))) {
      fprintf (stderr, "Failed to link aPos to the shader program.\n");
      SDL_Quit ();
      return false;
    };
    Shared::Surface image
        = Shared::makeSurface ("textures/pattern_16/diffus.tga");
    if(image == NULL){
      fprintf (stderr, "Failed to load texture.\n");
      SDL_Quit ();
      return false;
    }
    fprintf (stderr,
             "Texture pixel format name: %s\n",
             SDL_GetPixelFormatName (image->format->format));
    glGenTextures (1, &texture);
    glActiveTexture (GL_TEXTURE0);
    glBindTexture (GL_TEXTURE_2D, texture);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D (GL_TEXTURE_2D,
                  0,
                  GL_RGB,
                  image->w,
                  image->h,
                  0,
                  GL_BGR,
                  GL_UNSIGNED_BYTE,
                  image->pixels);
    image = NULL;
    glGenerateMipmap (GL_TEXTURE_2D);
    return true;
  }
  /** **/
  void
  Engine::shutdown ()
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
  Engine::printSdlError (const char *msg)
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