#pragma once
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
#include "hdCommon.hpp"
#include <eventpp/eventdispatcher.h>
#include <eventpp/mixins/mixinfilter.h>
namespace hd
{
  template <typename BASE> class CallableDispatcher : public BASE
  {
public:
    void operator() (const SDL_Event &e) { this->dispatch (e); }
  };
  /** Help eventpp extract event types from SDL_Event
   * @see
   * https://github.com/wqking/eventpp/blob/master/doc/tutorial_eventdispatcher.md
   * **/
  struct SDL_EventPolicies
  {
    static int getEvent (const SDL_Event &e);
    using Mixins = eventpp::MixinList<eventpp::MixinFilter, CallableDispatcher>;
  };
  /** SDL_Event.type dispatcher **/
  class SDL_EventTypeDispatcher
      : public eventpp::EventDispatcher<int, void (const SDL_Event &),
                                        SDL_EventPolicies>
  {
public:
    //void operator() (const SDL_Event &e);
  };
  /** Policy for SDL_KEY{DOWN,UP} **/
  struct SDL_EventKeySymPolicies
  {
    static SDL_Keycode getEvent (const SDL_Event &e);
    using Mixins = eventpp::MixinList<eventpp::MixinFilter, CallableDispatcher>;
  };
  /** dispatcher for SDL_KEY{DOWN,UP} KeySyms **/
  class SDL_EventKeyDispatcher
      : public eventpp::EventDispatcher<SDL_Keycode, void (const SDL_Event &e),
                                        SDL_EventKeySymPolicies>
  {
public:
    SDL_EventKeyDispatcher ();
    // void operator() (const SDL_Event &e);
  };
} // namespace hd