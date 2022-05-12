/**
 * @file GLContext.cpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-11
 *
 * @copyright GNU-GPL 3.0
 *
 */
#include "hd/sdl/GLContext.hpp"
namespace hd::sdl {
    GLContext GLContext::Create(SDL_Window*aWindow){
      SDL_GLContext created = SDL_GL_CreateContext (aWindow);
      return s_ptr (created, SDL_GL_DeleteContext);
    }
}