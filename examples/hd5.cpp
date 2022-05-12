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
#include "hd/sdl/Window.hpp"
#include "hd/sdl/GLContext.hpp"
#include "hd/gl/EBO.hpp"
#include "hd/gl/ShaderProgram.hpp"
#include "hd/gl/VAO.hpp"
#include "hd/gl/VBO.hpp"
hd::sdl::Window::s_ptr window;

int
main (int argc, char **argv)
{
  hd::Engine::Configure (argc, argv);
  hd::sdl::Window window = hd::sdl::Window::Create (800, 600, "HD1");
  hd::sdl::GLContext glContext = hd::sdl::GLContext::Create (window);
  // Vertices coordinates
  GLfloat vertices[] = {
    //               COORDINATES                  /     COLORS           //
    -0.5f,  -0.5f * float (sqrt (3)) * 1 / 3, 0.0f, 0.8f, 0.3f,
    0.02f, // Lower left corner
    0.5f,   -0.5f * float (sqrt (3)) * 1 / 3, 0.0f, 0.8f, 0.3f,
    0.02f, // Lower right corner
    0.0f,   0.5f * float (sqrt (3)) * 2 / 3,  0.0f, 1.0f, 0.6f,
    0.32f, // Upper corner
    -0.25f, 0.5f * float (sqrt (3)) * 1 / 6,  0.0f, 0.9f, 0.45f,
    0.17f, // Inner left
    0.25f,  0.5f * float (sqrt (3)) * 1 / 6,  0.0f, 0.9f, 0.45f,
    0.17f, // Inner right
    0.0f,   -0.5f * float (sqrt (3)) * 1 / 3, 0.0f, 0.8f, 0.3f,
    0.02f // Inner down
  };

  // Indices for vertices order
  GLuint indices[] = {
    0, 3, 5, // Lower left triangle
    3, 2, 4, // Lower right triangle
    5, 4, 1  // Upper triangle
  };
  window.MakeCurrent (glContext);
  hd::gl::ShaderProgram shaderProgram;
  shaderProgram.Create ("shaders/hd5.vert", "shaders/hd5.frag");
  shaderProgram.Bind ();
  hd::gl::VAO vao;
  vao.Create ();
  vao.Bind ();
  hd::gl::VBO vbo (vertices, sizeof (vertices));
  vbo.Bind ();
  hd::gl::EBO ebo (indices, sizeof (indices));
  ebo.Bind ();
  glVertexAttribPointer (
      0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (GLfloat), (void *)0);
  vao.LinkAttrib (vbo,
                  shaderProgram,
                  "aPos",
                  3,
                  GL_FLOAT,
                  6 * sizeof (GLfloat),
                  (void *)0);
  vao.LinkAttrib (vbo,
                  shaderProgram,
                  "aColor",
                  3,
                  GL_FLOAT,
                  6 * sizeof (GLfloat),
                  (void *)(3 * sizeof (float)));
  // glEnableVertexAttribArray (0);
  vao.Unbind ();
  vbo.Unbind ();
  ebo.Unbind ();
  window.engine->output.On ([&] (int aTime) {
    window.MakeCurrent (glContext);
    glClearColor (.0f, .5f, .5f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT);
    shaderProgram.Bind ();
    vao.Bind ();
    glDrawElements (GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
    window.Swap ();
  });
  window.Event().Close.Void.On ([&window] () {
    window.engine->step.Once ([&window] (int) { window = NULL; });
  });

  hd::sdl::Window win2 = hd::sdl::Window::Create (320, 200, "HD2");
  hd::sdl::GLContext win2gl = hd::sdl::GLContext::Create (win2);
  win2.Event ().Close.Void.On (
      [&win2] () { win2.engine->step.Once ([&win2] (int) { win2 = NULL; }); });
  win2.engine->output.On ([&] (int aTime) {
    win2.MakeCurrent (win2gl);
    glClearColor (.5f, .5f, .0f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT);
    win2.Swap ();
  });


  // engine->configure (argc, argv);
  hd::Engine::Get ()->Start ();


  return 0;
}