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
#include "hd/glProgram.hpp"
#include "hd/glVAO.hpp"
#include "hd/glVBO.hpp"
#include "hd/glEBO.hpp"
#include <stdio.h>

/**
 * @brief holodeck?
 */
namespace hd
{

  /**
   * @brief Apply what we know
   */
  class Engine
  {
  public:
    /** Start applying **/
    Engine ();
    Engine (int, char **);
    /** Finish applying **/
    ~Engine ();
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
    /** OpenGL Context **/
    SDL_GLContext context;
    /** Should we keep on working? */
    bool quit = false;
    /** GLSL Program ID */
    gl::Program shaderProgram;
    // GLuint programId = 0;
    //GLint aPos = -1;
    gl::VAO vao;
    // GLuint vao = 0;
    gl::VBO vbo;
    // GLuint vbo = 0;
    gl::EBO ebo;
    // GLuint ibo = 0;
    GLint scaleLocation = -1;
    GLint tex0Uni = -1;
    GLuint texture;
  };
} // namespace hd