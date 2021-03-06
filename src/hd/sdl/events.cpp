/**
 * @file events.cpp
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
#include "hd/sdl/events.hpp"
/** ###### Event grouping
 * hd groups events into common dispatchers so that one can listen to all that
 * group at once.**/
namespace hd::sdl {
  template <typename TARGET_FIELD_t, typename INDIRECT_FIELD_t>
  struct indirect_extractor {
    std::set<INDIRECT_FIELD_t> indirect_fields;
    std::function<TARGET_FIELD_t (const SDL_Event &)> target_extractor;
  };
  template <typename TARGET_FIELD_t, typename INDIRECT_FIELD_t>
  std::list<indirect_extractor<TARGET_FIELD_t, INDIRECT_FIELD_t> >
      indirect_extractor_list;

  template <typename TARGET_FIELD_t, typename INDIRECT_FIELD_t>
  std::function<TARGET_FIELD_t (const SDL_Event &)>
  IndirectExtractor (
      std::list<indirect_extractor<TARGET_FIELD_t, INDIRECT_FIELD_t> > list,
      std::function<INDIRECT_FIELD_t (const SDL_Event &)> indirectExtractor)
  {
    return [list, indirectExtractor] (const SDL_Event &e) {
      INDIRECT_FIELD_t t = indirectExtractor (e);
      for (auto extractor : list) {
        if (extractor.indirect_fields.find (t)
            != extractor.indirect_fields.end ()) {
          return extractor.target_extractor (e);
        }
      }
      throw "Unable to extract";
    };
  }

  std::set<SDL_EventType>

#define HD_KEY_EVENTS SDL_KEYDOWN, SDL_KEYUP

      KeyEvents = { HD_KEY_EVENTS },

#define HD_MOUSE_BUTTON_EVENTS SDL_MOUSEBUTTONDOWN, SDL_MOUSEBUTTONUP

      MouseButtonEvents = { HD_MOUSE_BUTTON_EVENTS },

#define HD_MOUSE_EVENTS SDL_MOUSEMOTION, SDL_MOUSEWHEEL, HD_MOUSE_BUTTON_EVENTS

      MouseEvents = { HD_MOUSE_EVENTS },

#define HD_JOY_BUTTON_EVENTS SDL_JOYBUTTONDOWN, SDL_JOYBUTTONUP

      JoyButtonEvents = { HD_JOY_BUTTON_EVENTS },

#define HD_JOY_DEVICE_EVENTS SDL_JOYDEVICEADDED, SDL_JOYDEVICEREMOVED

      JoyDeviceEvents = { HD_JOY_DEVICE_EVENTS },

#define HD_JOY_ID_EVENTS                                                      \
  SDL_JOYAXISMOTION, SDL_JOYHATMOTION, SDL_JOYBALLMOTION, HD_JOY_BUTTON_EVENTS

      JoyIdEvents = { HD_JOY_ID_EVENTS },

#define HD_JOY_EVENTS HD_JOY_DEVICE_EVENTS, HD_JOY_ID_EVENTS

      JoyEvents = { HD_JOY_EVENTS },

#define HD_GAMECONTROLLER_BUTTON_EVENTS                                       \
  SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLERBUTTONUP

      GameControllerButtonEvents = { HD_GAMECONTROLLER_BUTTON_EVENTS },

#define HD_GAMECONTROLLER_DEVICE_EVENTS                                       \
  SDL_CONTROLLERDEVICEADDED, SDL_CONTROLLERDEVICEREMAPPED,                    \
      SDL_CONTROLLERDEVICEREMOVED

      GameControllerDeviceEvents = { HD_GAMECONTROLLER_DEVICE_EVENTS },

#define HD_GAMECONTROLLER_TOUCHPAD_EVENTS                                     \
  SDL_CONTROLLERTOUCHPADDOWN, SDL_CONTROLLERTOUCHPADMOTION,                   \
      SDL_CONTROLLERTOUCHPADUP

      GameControllerTouchpadEvents = { HD_GAMECONTROLLER_TOUCHPAD_EVENTS },

#define HD_GAMECONTROLLER_EVENTS                                              \
  SDL_CONTROLLERAXISMOTION, SDL_CONTROLLERSENSORUPDATE,                       \
      HD_GAMECONTROLLER_BUTTON_EVENTS, HD_GAMECONTROLLER_DEVICE_EVENTS,       \
      HD_GAMECONTROLLER_TOUCHPAD_EVENTS

      GameControllerEvents = { HD_GAMECONTROLLER_EVENTS },

#define HD_AUDIO_DEVICE_EVENTS SDL_AUDIODEVICEADDED, SDL_AUDIODEVICEREMOVED

      AudioDeviceEvents = { HD_AUDIO_DEVICE_EVENTS },

#define HD_FINGER_EVENTS SDL_FINGERDOWN, SDL_FINGERUP, SDL_FINGERMOTION

      FingerEvents = { HD_FINGER_EVENTS },

#define HD_DOLLAR_EVENTS SDL_DOLLARGESTURE, SDL_DOLLARRECORD

      DollarEvents = { HD_DOLLAR_EVENTS },

#define HD_DROP_EVENTS                                                        \
  SDL_DROPBEGIN, SDL_DROPFILE, SDL_DROPTEXT, SDL_DROPCOMPLETE

      DropEvents = { HD_DROP_EVENTS }

  ;

}
/** ######## Event Type Dispatch
 * Often what is required is the categorization of events into dispatchers of
 *those types.
 ***/
namespace hd::sdl {
  EventTypeDispatch::EventTypeDispatch ()
      : AbstractFieldDispatch (
          [] (const SDL_Event &e) { return (SDL_EventType)e.type; })
  {
  }
}
/** ########## Key / Keyboard events
 *  - KeyDispatch: up, down
 *  - ScancodeDispatch: each scancode -> up/down
 *  - KeycodeDispatch: each keycode -> up/down
 *  - KeyboardDispatch: up/down, each scancode/keycode.
 * **/
namespace hd::sdl {
  /** Set up & down references to the map entries
   *  @todo key dispatch once & filters **/
  KeyDispatch::KeyDispatch ()
      : down ((*this)[SDL_KEYDOWN]), up ((*this)[SDL_KEYUP])
  {
  }
  /** Dispatch based on `e.key.keysym.scancode **/
  ScancodeDispatch::ScancodeDispatch ()
      : AbstractFieldDispatch (
          [] (const SDL_Event &e) { return e.key.keysym.scancode; })
  {
  }
  /** Dispatch based on `e.key.keysym.sym` **/
  KeycodeDispatch::KeycodeDispatch ()
      : AbstractFieldDispatch (
          [] (const SDL_Event &e) { return e.key.keysym.sym; })
  {
  }
  /** Trigger inherited, keycode, and scancode. **/
  void
  KeyboardDispatch::Trigger (const SDL_Event &e)
  {
    KeyDispatch::Trigger (e);
    keycode.Trigger (e);
    scancode.Trigger (e);
  }
}
/** ########## Text, & Drop Implementation
 * - TextDispatch
 * - DropDispatch
 **/
namespace hd::sdl {
  /** */
  TextDispatch::TextDispatch ()
      : editing ((*this)[SDL_TEXTEDITING]), input ((*this)[SDL_TEXTINPUT])
  {
  }
  /** */
  DropDispatch::DropDispatch ()
      : file ((*this)[SDL_DROPFILE]), text ((*this)[SDL_DROPTEXT]),
        begin ((*this)[SDL_DROPBEGIN]), complete ((*this)[SDL_DROPCOMPLETE])
  {
  }
}
/** ########## Mouse Dispatch Implementation
 * - MouseButtonDispatch groups up & down events.
 * - MouseWhichButtonDispatch
 * - MouseDispatch groups up/down, motion, wheel events.
 * - WhichMouseDispatch
 ***/
namespace hd::sdl {
  MouseButtonDispatch::MouseButtonDispatch ()
      : up ((*this)[SDL_MOUSEBUTTONUP]), down ((*this)[SDL_MOUSEBUTTONDOWN])
  {
    /** @todo add filter to once **/
    // once.filterIf([](const SDL_Event&e){return e.key.repeat > 0;});
  }
  MouseWhichButtonDispatch::MouseWhichButtonDispatch ()
      : AbstractFieldDispatch (
          [] (const SDL_Event &e) { return e.button.button; })
  {
  }
  /** Setup motion & wheel references, link button to button Events. **/
  MouseDispatch::MouseDispatch ()
      : motion ((*this)[SDL_MOUSEMOTION]), wheel ((*this)[SDL_MOUSEWHEEL])
  {
    On (MouseButtonEvents, button.pipe);
  }
  /** Setup dispatching between mouse instance ids. **
   */
  WhichMouseDispatch::WhichMouseDispatch ()
      : AbstractFieldDispatch (IndirectExtractor<Uint32, SDL_EventType> (
          { { MouseButtonEvents,
              [] (const SDL_Event &e) { return e.button.which; } },
            { { SDL_MOUSEMOTION },
              [] (const SDL_Event &e) { return e.motion.which; } },
            { { SDL_MOUSEWHEEL },
              [] (const SDL_Event &e) { return e.wheel.which; } } },
          [] (const SDL_Event &e) { return (SDL_EventType)e.type; }))
  {
  }
}
/** ########## Window Dispatch Implementation
 * - WindowEventTypeDispatch
 * - WindowDispatch this dispatches all the events that report a"windowID"...
 * @todo
 *  - WhichWindowDispatch: e.<something>.windowID
 ***/
namespace hd::sdl {
  /** Set up dispatching to named references of SDL_WINDOWEVENT_* **/
  WindowEventTypeDispatch::WindowEventTypeDispatch ()
      : AbstractFieldDispatch (
          [] (const SDL_Event &e) { return e.window.event; }),
        shown ((*this)[SDL_WINDOWEVENT_SHOWN]),
        hidden ((*this)[SDL_WINDOWEVENT_HIDDEN]),
        exposed ((*this)[SDL_WINDOWEVENT_EXPOSED]),
        moved ((*this)[SDL_WINDOWEVENT_MOVED]),
        resized ((*this)[SDL_WINDOWEVENT_RESIZED]),
        sizeChanged ((*this)[SDL_WINDOWEVENT_SIZE_CHANGED]),
        minimized ((*this)[SDL_WINDOWEVENT_MINIMIZED]),
        maximized ((*this)[SDL_WINDOWEVENT_MAXIMIZED]),
        restored ((*this)[SDL_WINDOWEVENT_RESTORED]),
        enter ((*this)[SDL_WINDOWEVENT_ENTER]),
        leave ((*this)[SDL_WINDOWEVENT_LEAVE]),
        focusGained ((*this)[SDL_WINDOWEVENT_FOCUS_GAINED]),
        focusLost ((*this)[SDL_WINDOWEVENT_FOCUS_LOST]),
        close ((*this)[SDL_WINDOWEVENT_CLOSE]),
        takeFocus ((*this)[SDL_WINDOWEVENT_TAKE_FOCUS]),
        hitTest ((*this)[SDL_WINDOWEVENT_HIT_TEST]),
        none ((*this)[SDL_WINDOWEVENT_NONE])

  {
  }
  /** **/
  WindowDispatch::WindowDispatch ()
  {
    On (KeyEvents, key.pipe);
    On ({ SDL_TEXTEDITING, SDL_TEXTINPUT }, text.pipe);
    On (MouseEvents, mouse.pipe);
    On (DropEvents, drop.pipe);
    On (SDL_WINDOWEVENT, window.pipe);
  }
  /** **/
  WhichWindowDispatch::WhichWindowDispatch ()
      : AbstractFieldDispatch (IndirectExtractor<Uint32, SDL_EventType> (
          { { { SDL_WINDOWEVENT },
              [] (const SDL_Event &e) { return e.window.windowID; } },
            { KeyEvents, [] (const SDL_Event &e) { return e.key.windowID; } },
            { { SDL_TEXTEDITING },
              [] (const SDL_Event &e) { return e.edit.windowID; } },
            { { SDL_TEXTINPUT },
              [] (const SDL_Event &e) { return e.text.windowID; } },
            { { SDL_MOUSEMOTION },
              [] (const SDL_Event &e) { return e.motion.windowID; } },
            { { SDL_MOUSEWHEEL },
              [] (const SDL_Event &e) { return e.wheel.windowID; } },
            { MouseButtonEvents,
              [] (const SDL_Event &e) { return e.button.windowID; } } },
          [] (const SDL_Event &e) { return (SDL_EventType)e.type; }))

  {
    /** @todo key dispatch once & filters **/
  }
}
/** ########## Joystick Dispatch Implementation */
namespace hd::sdl {
  JoyWhichAxisDispatch::JoyWhichAxisDispatch ()
      : AbstractFieldDispatch (
          [] (const SDL_Event &e) { return e.jaxis.axis; })
  {
  }
  JoyWhichBallDispatch::JoyWhichBallDispatch ()
      : AbstractFieldDispatch (
          [] (const SDL_Event &e) { return e.jball.ball; })
  {
  }
  JoyWhichHatDispatch::JoyWhichHatDispatch ()
      : AbstractFieldDispatch ([] (const SDL_Event &e) { return e.jhat.hat; })
  {
  }
  JoyWhichButtonDispatch::JoyWhichButtonDispatch ()
      : AbstractFieldDispatch (
          [] (const SDL_Event &e) { return e.jbutton.button; })
  {
  }
  JoyDispatch::JoyDispatch ()
      : added ((*this)[SDL_JOYDEVICEADDED]),
        removed ((*this)[SDL_JOYDEVICEREMOVED])
  {
    On (SDL_JOYAXISMOTION, axis.pipe);
    On (SDL_JOYBALLMOTION, ball.pipe);
    On (SDL_JOYHATMOTION, hat.pipe);
    On (JoyButtonEvents, button.pipe);
  }
  /** **/
  WhichJoyDispatch::WhichJoyDispatch ()
      : AbstractFieldDispatch (
          IndirectExtractor<SDL_JoystickID, SDL_EventType> (
              { { { SDL_JOYAXISMOTION },
                  [] (const SDL_Event &e) { return e.jaxis.which; } },
                { { SDL_JOYBALLMOTION },
                  [] (const SDL_Event &e) { return e.jball.which; } },
                { { SDL_JOYHATMOTION },
                  [] (const SDL_Event &e) { return e.jhat.which; } },
                { JoyButtonEvents,
                  [] (const SDL_Event &e) { return e.jbutton.which; } },
                { JoyDeviceEvents,
                  [] (const SDL_Event &e) { return e.jdevice.which; } } },
              [] (const SDL_Event &e) { return (SDL_EventType)e.type; }))
  {
  }

  /** **/
  AllJoyDispatch::AllJoyDispatch () { On (JoyIdEvents, stick.pipe); }
}
/** ########## Controller Dispatch Implementation */
namespace hd::sdl {
  /** set up dispatching on `e.cbutton.button`  */
  GameControllerWhichButtonDispatch::GameControllerWhichButtonDispatch ()
      : AbstractFieldDispatch (
          [] (const SDL_Event &e) { return e.cbutton.button; })
  {
  }
  /** set up dispatching on `e.caxis.axis`  */
  GameControllerWhichAxisDispatch::GameControllerWhichAxisDispatch ()
      : AbstractFieldDispatch (
          [] (const SDL_Event &e) { return e.caxis.axis; })
  {
  }
  /** set up dispatching on `e.ctouchpad.touchpad` */
  GameControllerWhichTouchpadDispatch::GameControllerWhichTouchpadDispatch ()
      : AbstractFieldDispatch (
          [] (const SDL_Event &e) { return e.ctouchpad.touchpad; })
  {
  }
  /** set up dispatching on `e.csensor.sensor` */
  GameControllerWhichSensorDispatch::GameControllerWhichSensorDispatch ()
      : AbstractFieldDispatch (
          [] (const SDL_Event &e) { return e.csensor.sensor; })
  {
  }
  /** set up dispatching on the right `which` value for these events:
   * #. SDL_CONTROLLERAXISMOTION
   * #. GameControllerDeviceEvents (added,removed,remapped)
   * #. GameControllerButtonEvents (up, down)
   * #. GameControllerTouchpadEvents ()
   * #.
   */
  WhichGameControllerDispatch::WhichGameControllerDispatch ()
      : AbstractFieldDispatch (
          IndirectExtractor<SDL_JoystickID, SDL_EventType> (
              { { { SDL_CONTROLLERAXISMOTION },
                  [] (const SDL_Event &e) { return e.caxis.which; } },
                { GameControllerDeviceEvents,
                  [] (const SDL_Event &e) { return e.cdevice.which; } },
                { GameControllerButtonEvents,
                  [] (const SDL_Event &e) { return e.cbutton.which; } },
                { GameControllerTouchpadEvents,
                  [] (const SDL_Event &e) { return e.ctouchpad.which; } },
                { { SDL_CONTROLLERSENSORUPDATE },
                  [] (const SDL_Event &e) { return e.csensor.which; } } },
              [] (const SDL_Event &e) { return (SDL_EventType)e.type; }))
  {
  }
  GameControllerDispatch::GameControllerDispatch ()
      : added ((*this)[SDL_CONTROLLERDEVICEADDED]),
        removed ((*this)[SDL_CONTROLLERDEVICEREMOVED]),
        remapped ((*this)[SDL_CONTROLLERDEVICEREMAPPED])
  {
    On (SDL_CONTROLLERAXISMOTION, axis.pipe);
    On (SDL_CONTROLLERSENSORUPDATE, sensor.pipe);
    On (GameControllerButtonEvents, button.pipe);
    On (GameControllerTouchpadEvents, touchpad.pipe);
  }
  AllGameControllerDispatch::AllGameControllerDispatch ()
  {
    On (GameControllerEvents, controller.pipe);
  }
}
/** ########## hd::sdl::EngineDispatch
 * @todo rename to "root" or something?
 **/
namespace hd::sdl {

#define HD_APP_EVENTS                                                         \
  SDL_APP_TERMINATING, SDL_APP_LOWMEMORY, SDL_APP_WILLENTERBACKGROUND,        \
      SDL_APP_DIDENTERBACKGROUND, SDL_APP_WILLENTERFOREGROUND,                \
      SDL_APP_DIDENTERFOREGROUND

#define HD_WINDOWID_EVENTS                                                    \
  SDL_WINDOWEVENT, SDL_KEYDOWN, SDL_KEYUP, SDL_TEXTEDITING, SDL_TEXTINPUT,    \
      SDL_MOUSEMOTION, SDL_MOUSEBUTTONDOWN, SDL_MOUSEBUTTONUP,                \
      SDL_MOUSEWHEEL, SDL_DROPFILE, SDL_DROPTEXT, SDL_DROPBEGIN,              \
      SDL_DROPCOMPLETE

#define HD_FINGER_EVENTS SDL_FINGERDOWN, SDL_FINGERUP, SDL_FINGERMOTION

#define HD_DOLLAR_EVENTS SDL_DOLLARGESTURE, SDL_DOLLARRECORD, SDL_MULTIGESTURE

#define HD_RENDER_EVENTS SDL_RENDER_TARGETS_RESET, SDL_RENDER_DEVICE_RESET

  EngineDispatch::EngineDispatch ()
      : quit ((*this)[SDL_QUIT]),
        localChanged ((*this)[SDL_LOCALECHANGED]),
        clipboard ((*this)[SDL_CLIPBOARDUPDATE]),
        user((*this)[SDL_USEREVENT]),
        keymap((*this)[SDL_KEYMAPCHANGED]),
        sensor((*this)[SDL_SENSORUPDATE]),
        sysWm((*this)[SDL_SYSWMEVENT])
  {
    On ({ HD_APP_EVENTS }, app.pipe);
    On (SDL_SYSWMEVENT, // system specific event
        sysWm.pipe);
    On (SDL_KEYMAPCHANGED, // keymap changed due to a system event such as an
                           // input language or keyboard layout change (>=
                           // SDL 2.0.4)
        keymap.pipe);
    On ({ HD_WINDOWID_EVENTS }, windows.pipe);
    On (JoyEvents, joysticks.pipe);
    On (GameControllerEvents, controllers.pipe);
    On ({ HD_FINGER_EVENTS }, finger.pipe);
    On ({ HD_DOLLAR_EVENTS }, dollar.pipe);
    On (AudioDeviceEvents, audio.pipe);
    On ({ HD_RENDER_EVENTS }, render.pipe);
    On (SDL_USEREVENT, // These are for your use, and should be allocated
                       // with SDL_RegisterEvents()
        user.pipe);

    // hdError ("Unknown Event type %d\n", e.type);
  }
}
/** ######## hd::sdl::events
 *
 * @code hd::sdl::events.Close.On([](const SDL_Event&e){\/* your handler
 * *\/});
 *
 */
namespace hd::sdl {
  /**
   * @brief There are a fixed set of events that SDL can produce at any one
   * time and this is represented by this structure:
   */
  EngineDispatch events;
}