/**
 * @file events.hpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-13
 *
 * @copyright GNU-GPL 3.0 Copyright (C) 2022 Jón Davíð Sawyer
 * (badquanta@gmail.com)
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
 *
 *
 */
#pragma once
#include <SDL2/SDL_events.h>
#include <hd/Dispatch.hpp>
#include <set>
/** # Event Dispatching
 *
 * SDL Provides SDL_Event structures. This is mapped here to an object tree.
 *
 * @code
 *    hd::sdl::events.quit.On([](const SDL_Event&){
 *      // Your event code here
 *    })
 *
 * @todo document hd::sdl::`events` better.
 */
namespace hd::sdl {
  /**
   * @brief Implementation of AbstractDispatch that distributes `const
   * SDL_Event&^ references.
   *
   */
  class EventDispatch : public AbstractToVoidDispatch<const SDL_Event &> {
  public:
  };
  /**
   * @brief
   *
   * @tparam FIELD_TYPE
   * @tparam FIELD_CLASS
   * @tparam DISPATCH_PARENT
   */
  template <class FIELD_TYPE, class FIELD_CLASS = EventDispatch>
  class AbstractFieldDispatch : public std::map<FIELD_TYPE, FIELD_CLASS>,
                                public EventDispatch {
  public:
    typedef std::function<FIELD_TYPE (const SDL_Event &e)> FieldExtractor;
    FieldExtractor fieldExtractor;
    AbstractFieldDispatch (FieldExtractor fe) : fieldExtractor (fe) {}
    virtual void
    Trigger (const SDL_Event &e) override
    {
      EventDispatch::Trigger (e);
      FIELD_TYPE field = fieldExtractor (e);
      if (this->find (field) != this->end ()) {
        this->at (field).Trigger (e);
      }
    };
    using EventDispatch::On;
    Handle
    On (FIELD_TYPE key, Handler handler)
    {
      return (*this)[key].On (handler);
    };
    std::vector<Handle>
    On (std::set<FIELD_TYPE> keys, Handler handler)
    {
      std::vector<Handle> handles;
      for (FIELD_TYPE fieldValue : keys) {
        handles.push_back (On (fieldValue, handler));
      }
      return handles;
    }
  };
  /**
   * @brief
   *
   */
  class EventTypeDispatch
      : public AbstractFieldDispatch<SDL_EventType, EventDispatch> {
  public:
    EventTypeDispatch ();
  };
  /** @brief Dispatch Key up & down events to specialized handlers.
   *
   */
  class KeyDispatch : public EventTypeDispatch {
  public:
    EventDispatch &up, &down, downOnce;
    KeyDispatch ();
  };
}
/** ## Key / keyboard event dispatching
 * - up / down
 * - Scancode
 * - Keycode
 **/
namespace hd::sdl {
  /** Implementation of scancode dispatching. **/
  class ScancodeDispatch
      : public AbstractFieldDispatch<SDL_Scancode, KeyDispatch> {
  public:
    ScancodeDispatch ();
  };
  /** Implementation of keycode dispatching. **/
  class KeycodeDispatch
      : public AbstractFieldDispatch<SDL_Keycode, KeyDispatch> {
  public:
    KeycodeDispatch ();
  };
  /** @brief Dispatch events to specialized `SDL_Keycode` or `SDL_Scancode`
   * handers.
   *
   */
  class KeyboardDispatch : public KeyDispatch {
  public:
    ScancodeDispatch scancode;
    KeycodeDispatch keycode;
    virtual void Trigger (const SDL_Event &e) override;
  };
  /** @brief Distribute between Text `Editing` and `Input` events.
   *
   */
}
/** ## Text, & Drop Event I/O
 * - Editing
 * - Input
 **/
namespace hd::sdl {
  /** */
  class TextDispatch : public EventTypeDispatch {
  public:
    EventDispatch &editing, &input;
    TextDispatch ();
  };
  /** @brief Dispatch drop events to specialized handlers.
   *
   */
  class DropDispatch : public EventTypeDispatch {
  public:
    EventDispatch &file, &text, &begin, &complete;
    DropDispatch ();
    // virtual void Trigger (const SDL_Event &) override;
  };
}
/** ## Mouse Events
 * - Which button
 * - Which Mouse
 * - button up/down
 * - motion
 * - wheel
 * **/
namespace hd::sdl {
  /** @brief Distribute between up & down event handlers.
   *
   */
  class MouseButtonDispatch : public EventTypeDispatch {
  public:
    EventDispatch &up, &down;
    MouseButtonDispatch ();
  };
  /** @brief Group & Distinguish mouse buttons **/
  class MouseWhichButtonDispatch
      : public AbstractFieldDispatch<Uint8, MouseButtonDispatch> {
  public:
    MouseWhichButtonDispatch ();
  };
  /** @brief Distribute events between motion, wheel, & button.
   *
   */
  class MouseDispatch : public MouseButtonDispatch {
  public:
    EventDispatch &motion, &wheel;
    MouseWhichButtonDispatch button;
    MouseDispatch ();
  };
  /** @brief Group & Distinguish all mice **/
  class WhichMouseDispatch
      : public AbstractFieldDispatch<Uint32, MouseDispatch> {
  public:
    WhichMouseDispatch ();
  };
}
/** ## WindowID Events **/
namespace hd::sdl {

  /** @brief Dispatch between different types of window events. **/
  class WindowEventTypeDispatch
      : public AbstractFieldDispatch<Uint32, EventDispatch> {
  public:
    EventDispatch &shown, &hidden, &exposed, &moved, &resized, &sizeChanged,
        &minimized, &maximized, &restored, &enter, &leave, &focusGained,
        &focusLost, &close, &takeFocus, &hitTest, &none;
    WindowEventTypeDispatch ();
  };
  /** @brief Distribute between the many types of an event that an individual
   * window can receive.
   *
   */
  class WindowDispatch : public EventTypeDispatch {
  public:
    KeyboardDispatch key;
    MouseDispatch mouse;
    TextDispatch text;
    DropDispatch drop;
    WindowEventTypeDispatch window;
    WindowDispatch ();
  };
  /** @brief Destribute events by their Window ID
   *
   */
  class WhichWindowDispatch
      : public AbstractFieldDispatch<Uint32, WindowDispatch> {
  public:
    // virtual void Trigger (const SDL_Event &) override;
    WhichWindowDispatch ();
  };
}
/** ## Joy Events
 *
 *  - JoysticksDispatch
 *   - ...JoyDispatch...
 *   - stick : WhichJoyDispatch : JoyDispatch
 *     - axis : WhichAxisDispatch
 *
 * @todo document joy events**/
namespace hd::sdl {
  /** Dispatch events by the `jaxis.axis` field. **/
  class JoyWhichAxisDispatch : public AbstractFieldDispatch<Uint8> {
  public:
    JoyWhichAxisDispatch ();
  };
  /** Dispatch events by the `jbutton.button` field. **/
  class JoyWhichButtonDispatch : public AbstractFieldDispatch<Uint8> {
  public:
    JoyWhichButtonDispatch ();
  };
  /** Dispatch events by the `jball.ball` field. **/
  class JoyWhichBallDispatch : public AbstractFieldDispatch<Uint8> {
  public:
    JoyWhichBallDispatch ();
  };
  /** Dispatch events by the `jhat.hat` field. **/
  class JoyWhichHatDispatch : public AbstractFieldDispatch<Uint8> {
  public:
    JoyWhichHatDispatch ();
  };
  /** @brief the different events a joystick can have. */
  class JoyDispatch : public EventTypeDispatch {
  public:
    JoyWhichAxisDispatch axis;
    JoyWhichBallDispatch ball;
    JoyWhichHatDispatch hat;
    JoyWhichButtonDispatch button;
    EventDispatch &added, &removed;
    JoyDispatch ();
  };
  /** @brief Dispatch joystick events by their ID to the specific ID handler.
   */
  class WhichJoyDispatch
      : public AbstractFieldDispatch<SDL_JoystickID, JoyDispatch> {
  public:
    // EventDispatch &Added, &Removed;
    WhichJoyDispatch ();
    // virtual void Trigger (const SDL_Event &) override;
  };
  /** Represents all joysticks, providing access to a specific joystick via
   * `stick`. **/
  class AllJoyDispatch : public JoyDispatch {
  public:
    WhichJoyDispatch stick;
    AllJoyDispatch ();
  };
}
/** ## Controller Events
 * @todo rename to GameController and document */
namespace hd::sdl {
  class GameControllerWhichAxisDispatch : public AbstractFieldDispatch<Uint8> {
  public:
    GameControllerWhichAxisDispatch ();
  };
  class GameControllerButtonDispatch : public EventTypeDispatch {
  public:
    EventDispatch &up, &down;
    GameControllerButtonDispatch ();
  };
  class GameControllerWhichButtonDispatch
      : public AbstractFieldDispatch<Uint8, GameControllerButtonDispatch> {
  public:
    GameControllerWhichButtonDispatch ();
  };
  class GameControllerWhichTouchpadDispatch
      : public AbstractFieldDispatch<Sint32> {
  public:
    GameControllerWhichTouchpadDispatch ();
  };
  class GameControllerWhichSensorDispatch
      : public AbstractFieldDispatch<Sint32> {
  public:
    GameControllerWhichSensorDispatch ();
  };
  /** @brief Dispatch various events for a controller to specialized handlers.
   */
  class GameControllerDispatch : public EventTypeDispatch {
  public:
    GameControllerWhichAxisDispatch axis;
    GameControllerWhichButtonDispatch button;
    GameControllerWhichTouchpadDispatch touchpad;
    GameControllerWhichSensorDispatch sensor;
    EventDispatch &added, &removed, &remapped;
    GameControllerDispatch ();
  };
  /** @brief Forward events to controller specific dispatchers.
   *
   */
  class WhichGameControllerDispatch
      : public AbstractFieldDispatch<SDL_JoystickID, GameControllerDispatch> {
  public:
    WhichGameControllerDispatch ();
  };
  class AllGameControllerDispatch : public GameControllerDispatch {
  public:
    WhichGameControllerDispatch controller;
    AllGameControllerDispatch ();
  };
}
/** ## Engine Dispatch
 * @todo rename to RootDispatch and document? */
namespace hd::sdl {
  /** @brief Simply intended to be the root of an event tree.
   * @todo consider renaming this class.
   * @todo implement AudioDeviceDispatch?
   */
  class EngineDispatch : public EventTypeDispatch {
  public:
    EventDispatch &quit, app, &localChanged, &sysWm, finger, dollar, &clipboard, audio, render,
        &user, &keymap, &sensor;
    WhichWindowDispatch windows;
    AllJoyDispatch joysticks;
    WhichGameControllerDispatch controllers;

    EngineDispatch ();
  };
  /** @brief Basically a singleton/static point at which to begin routing
   * events.
   *
   */
  extern EngineDispatch events;
}