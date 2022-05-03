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
#include "hd/Window.hpp"
#include "hd/glCamera.hpp"
#include "hd/glEBO.hpp"
#include "hd/glTexture.hpp"
#include "hd/glVAO.hpp"

// Vertices coordinates
GLfloat vertexData[] = {
  //     COORDINATES     /        COLORS      /   TexCoord  //
  -0.5f, 0.0f, 0.5f,  0.83f, 0.70f, 0.44f, 0.0f, 0.0f, //
  -0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 1.0f, 0.0f, //
  0.5f,  0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f, //
  0.5f,  0.0f, 0.5f,  0.83f, 0.70f, 0.44f, 1.0f, 0.0f, //
  0.0f,  0.8f, 0.0f,  0.92f, 0.86f, 0.76f, 0.5f, 1.0f  //
};

// Indices for vertices order
GLuint indexData[] = {
  0, 1, 2, //
  0, 2, 3, //
  0, 1, 4, //
  1, 2, 4, //
  2, 3, 4, //
  3, 0, 4  //
};
/** GLSL Program ID */
hd::gl::Program shaderProgram;
// GLuint programId = 0;
// GLint aPos = -1;
hd::gl::VAO vao;
// GLuint vao = 0;
hd::gl::VBO vbo;
// GLuint vbo = 0;
hd::gl::EBO ebo;
// GLuint ibo = 0;
GLint scaleLocation = -1;
GLint tex0Uni = -1;
hd::gl::Texture texture;

hd::Window::Ptr window;
hd::gl::Camera camera;

void
drawFrame (int ticks)
{
  window->MakeCurrent ();
  glClearColor (0.f, 0.f, 0.f, 1.f);
  glEnable (GL_DEPTH_TEST);
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // SDL_RenderPresent (renderer);
  // glUseProgram (programId);
  shaderProgram.Bind ();
  // GLint modelLoc = shaderProgram.getUniformLocation ("model");
  // glUniformMatrix4fv (modelLoc, 1, GL_FALSE, glm::value_ptr (model));
  // GLint viewLoc = shaderProgram.getUniformLocation ("view");
  // glUniformMatrix4fv (viewLoc, 1, GL_FALSE, glm::value_ptr (view));
  // GLint projLoc = shaderProgram.getUniformLocation ("proj");
  // glUniformMatrix4fv (projLoc, 1, GL_FALSE, glm::value_ptr (proj));
  int w, h;
  window->GetDrawableSize (&w, &h);
  // hdDebug ("Drawable size: %dx%d", w, h);
  glViewport (0, 0, w, h);
  camera.Matrix (w, h, 45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");
  glUniform1f (scaleLocation, 0.5f);
  glUniform1i (tex0Uni, 0);
  // glBindTexture (GL_TEXTURE_2D, texture);
  texture.Bind ();
  vbo.Bind ();
  // glVertexAttribPointer (
  //     aPos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (GLfloat), (void *)0);
  // glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ibo);
  ebo.Bind ();
  glDrawElements (GL_TRIANGLES,
                  sizeof (indexData) / sizeof (GLuint),
                  GL_UNSIGNED_INT,
                  NULL);
  // glDisableVertexAttribArray (aPos);
  shaderProgram.unbind ();
  window->Swap ();
}

int
main (int argc, char **argv)
{
  hd::Engine::PrintVersions ();
  // hd::Engine::Mount engine = hd::Engine::Get ();
  hd::Engine::Configure (argc, argv);
  window = hd::Window::Create (hd::Engine::GetProgramName ());
  window->input.On (camera.input.pipe);
  window->input.Close.Void.Once ([] () {
    hd::Engine::Get ()->process.Void.Once ([] () { window = NULL; });
  });
  hd::Engine::Get ()->process.Void.Once ([] () {
    window->MakeCurrent ();
    hd::Engine::Ptr engine = hd::Engine::Get ();
    if (!shaderProgram.Create ("shaders/default.vert",
                               "shaders/default.frag")) {
      fprintf (stderr, "Failed to create Shader Program\n");
      engine->Quit ();
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
      fprintf (stderr, "Failed to link attributes to the shader program.\n");
      engine->Quit ();
    };

    texture.Create (
        "textures/pattern_16/diffus.tga", GL_TEXTURE_2D, GL_TEXTURE0);
    texture.Assign (shaderProgram, "tex0", 0);
  });
  window->output.On (&drawFrame);
  hd::Engine::Get ()->Start ();
  return 0;
}