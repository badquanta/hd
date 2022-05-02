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
#include "hd/glVAO.hpp"
#include "hd/glProgram.hpp"
hd::Window::Ptr window;

int
main (int argc, char **argv)
{
  hd::Engine::Configure (argc, argv);
  hd::Window::Ptr window = hd::Window::Create (800, 600, "HD1");
  // Vertices coordinates
  GLfloat vertices[] = {
    -0.5f, -0.5f * float (sqrt (3)) / 3,    0.0f, // Lower left corner
    0.5f,  -0.5f * float (sqrt (3)) / 3,    0.0f, // Lower right corner
    0.0f,  0.5f * float (sqrt (3)) * 2 / 3, 0.0f  // Upper corner
  };
  window->MakeCurrent ();
  hd::gl::Program shaderProgram;
  shaderProgram.Create ("shaders/hd2.vert","shaders/hd2.frag");
  shaderProgram.Bind ();
  hd::gl::VAO vao;
  vao.Create ();

  window->onRender.On ([&window] () {
    window->MakeCurrent ();
    glClear (GL_COLOR_BUFFER_BIT);
    window->Swap ();
  });
  window->on.Close.Void.On ([&window] () {
    hd::Engine::Get()->eachFrame.Once( [&window](int){
      window = NULL;
    });
  });

  hd::Window::Ptr win2 = hd::Window::Create (320, 200, "HD2");
  win2->on.Close.Void.On (
      [&win2] () { hd::Engine::Get ()->eachFrame.Once ([&win2] (int) {
        win2 = NULL;
      });
  });
  // engine->configure (argc, argv);
  hd::Engine::Get ()->Start ();
  return 0;
}