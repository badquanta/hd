#include "hd/Events.hpp"
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
/** **/
namespace hd
{
  namespace sdl
  {
    namespace event
    {
      void
      Tree::reset ()
      {
        List clearList;
        swap (clearList);
        TypeDispatcher clearDispatcher;
        type.swap (clearDispatcher);
        append (type.pipe);
        type.appendListener (SDL_KEYDOWN, keyCode.pipe);
        type.appendListener (SDL_KEYUP, keyCode.pipe);
      }
      Tree::Tree () { reset (); }

      int
      TypeDispatchPolicy::getEvent (const SDL_Event &e)
      {
        return e.type;
      }

      /*
        void TypeDispatcher::operator() (const SDL_Event &e){
          dispatch (e);
        }
        **/
      namespace key
      {
        int
        CodePolicy::getEvent (const SDL_Event &e)
        {
          return e.key.keysym.sym;
        }
        CodeDispatcher::CodeDispatcher ()
        {
          this->appendFilter ([] (const SDL_Event &e) -> bool {
            return ((e.type == SDL_KEYDOWN)) || ((e.type == SDL_KEYUP));
          });
        }
      } // namespace key
    }   // namespace event
  }     // namespace sdl

} // namespace hd