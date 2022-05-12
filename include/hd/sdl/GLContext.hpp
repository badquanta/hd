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
#include "hd/Common.hpp"
#include "hd/Wrap.hpp"
namespace hd::sdl {
    class GLContext: public WRAP_PTR<void> {
        public:
          using WRAP_PTR::WRAP_PTR;
          static GLContext Create (SDL_Window *);
    };
}