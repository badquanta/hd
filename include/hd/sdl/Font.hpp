/**
 * @file Font.hpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-11
 *
 * @copyright GNU-GPL 3.0
 *
 */
#include "hd/Wrap.hpp"
namespace hd::sdl {
    class Font : public WRAP_PTR<SDL_Font> {
        public:
          using WRAP_PTR::WRAP_PTR;
    }
}