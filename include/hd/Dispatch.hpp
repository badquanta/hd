/**
 * @file Dispatch.hpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-14
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
/* hd abstracts the dispatching of parameters to many listeners into a type.
 * **/
#include <functional>
#include <map>
#include <list>
namespace hd {
  /**
   * @brief Collection of `On` and `Once` handlers.
   *
   * @note Handlers can be removed if you save their `Handle` when attached to
   * `On` or `Once`.
   *
   * @tparam Signature a set of paramenters that define an event. @see
   * `hd::sdl::EventDispatch` for an example.
   */
  template <class... Signature> class AbstractDispatch {
  public:
    virtual ~AbstractDispatch (){};
    /** Each specialization will have its own `std::function<...>` typedef. **/
    typedef std::function<void (Signature...)> Handler;
    /** Each handler that is attached via `On` or `Once` will have its own
     * Handle. **/
    typedef unsigned int Handle;
    /**
     * @brief Cause the list of `On` and `Once` callbacks to be called.
     * @note `Once` callbacks will be called _first_ and then the list will be
     * cleared.
     * @note `On` callbacks will be called each time trigger is called.
     * @note Callbacks are gathered from the Once and On handlers and THEN executed.
     * @param Args defined by the `Signature`
     */
    virtual void
    Trigger (Signature... Args)
    {
      std::list<Handler> combined;
      for (std::pair<Handle, Handler> pair : m_OnceHandlers) {
        if (pair.second != NULL) {
          //pair.second (Args...);
          combined.push_back (pair.second);
        }
      }
      m_OnceHandlers.clear ();
      for (std::pair<Handle, Handler> pair : m_Handlers) {
        if (pair.second != NULL) {
          //pair.second (Args...);
          combined.push_back (pair.second);
        }
      }
      for(Handler handler : combined){
        handler (Args...);
      }
    };
    /**
     * @brief connect a `Handler` to this `Dispatch` instance so that on
     * _every_ subsequent call of `Trigger(...)` this callback `Handler` will
     * be called.
     * @param aHandler
     * @return Handle a value that can be later given to `Delete` in order to
     * remove this relationship.
     */
    Handle
    On (Handler aHandler)
    {
      Handle id = nextId;
      nextId++;
      m_Handlers[id] = aHandler;
      return id;
    };
    /**
     * @brief connect a `Handler` to this `Dispatch` instance so that on _the
     * next_ subsequent call of `Trigger(...)` this callback `Handler` will be
     * called __and then__ automatically deleted.
     *
     * @param aHandler
     * @return Handle a value that can later be given to `Delete` in order to
     * remove this relationship __before__ the next call to `Trigger`.
     */
    Handle
    Once (Handler aHandler)
    {
      Handle id = nextId;
      nextId++;
      m_OnceHandlers[id] = aHandler;
      return id;
    };
    /**
     * @brief remove a `Handler` callback before next call to `Trigger`
     *
     * @param aId
     * @return true
     * @return false
     */
    bool
    Delete (Handle aId)
    {
      return (m_OnceHandlers.erase (aId) == 1)
             || (m_Handlers.erase (aId) == 1);
    };
    /**
     * @brief Utility to connect one dispatcher to triggers of another
     *
     */
    const Handler pipe = [this] (Signature... e) { this->Trigger (e...); };

  private:
    /**
     * @brief Internal list of handlers to be notified on every trigger.
     *
     */
    std::map<Handle, Handler> m_Handlers;
    /**
     * @brief Internal list of handlers to be notified on the __next__ trigger
     * and then cleared.
     *
     */
    std::map<Handle, Handler> m_OnceHandlers;
    /**
     * @brief Each instance of Dispatch generates its own HandleIds
     *
     */
    Handle nextId = 1;
  };
  /** @brief A dispatch that has a zero length signature. */
  class VoidSignatureDispatch : public AbstractDispatch<> {};
  /** @brief Relays triggers to `Void::Handler`(s) */
  template <class... Signature>
  class AbstractToVoidDispatch : public AbstractDispatch<Signature...> {
  public:
    /** @brief Allows subscribers that do not need the event information */
    VoidSignatureDispatch Void;
    /** @brief Also causes `Void.Trigger()` to be called.
     * @param Args
     */
    virtual void
    Trigger (Signature... Args) override
    {
      Void.Trigger ();
      AbstractDispatch<Signature...>::Trigger (Args...);
    };
  };

  class IntDispatch : public AbstractToVoidDispatch<int> {};
}