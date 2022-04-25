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
#include "hd/Common.hpp"
#include <functional>
#include <map>
/** **/
namespace hd {
  /** **/
  class Event {
  public:
    typedef std::function<void (const SDL_Event &)> Handler;
    virtual void Trigger (const SDL_Event &);
    unsigned int Add (Handler);
    bool Delete (unsigned int);
    // Pipes are used to attach one event to the trigger of another.
    const Handler pipe = [this] (const SDL_Event &e) {
      this->Trigger (e);
    };

  private:
    unsigned int nextId = 0;
    std::map<int, Handler> handlers;
  };
  /** **/
  class KeyEvent : public Event {
    public:
    Event Up, Down;
    virtual void Trigger (const SDL_Event &) override;
  };
  /** **/
  class KeyboardEvent : public Event {
    public:
    std::map<SDL_Scancode, KeyEvent> Scancode;
    std::map<SDL_Keycode, KeyEvent> Keycode;
    Event MapChange;
    virtual void Trigger (const SDL_Event &) override;
  };
  /** **/
  class MouseButtonEvent : public Event {
  public:
    Event Up, Down;
    virtual void Trigger (const SDL_Event &) override;
  };
  /** **/
  class MouseEvent  : public Event {
  public:
    Event Motion, Wheel;
    std::map<Uint8,MouseButtonEvent> Button;
    virtual void Trigger (const SDL_Event &) override;
  };
  /** **/
  class TypeEvent  : public Event {
  public:
    Event Quit, App, Window, SysWM, Text, Joy, Controller,
        Finger, Dollar, Clipboard, Drop, Audio, Render, User;
    KeyboardEvent Key;
    MouseEvent Mouse;

    virtual void Trigger (const SDL_Event &) override;
  };
} // namespace hd