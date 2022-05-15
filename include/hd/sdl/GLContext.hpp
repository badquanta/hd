/**
 * @file GLContext.hpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-11
 *
 * @copyright GNU-GPL 3.0
 *
 */
#pragma once

#include "hd/Wrap.hpp"
#include "SDL2/SDL_video.h"
namespace hd::sdl {
    /**
     * @brief Encapsulate the concept of an `SDL_GLContext`
     *
     */
    class GLContext: public WRAP_PTR<void> {
        public:
          using WRAP_PTR::WRAP_PTR;
          static GLContext Create (SDL_Window *);
    };
}