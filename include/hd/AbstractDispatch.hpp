/**
 * @file AbstractDispatch.hpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-13
 *
 * @copyright GNU-GPL 3.0 Copyright (C) 2022 Jón Davíð Sawyer (badquanta@gmail.com)
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
/*
 */
#include "hd/Common.hpp"
namespace hd {
  template <class... Signature> class AbstractDispatch {
  public:
    virtual ~AbstractDispatch (){};
    typedef std::function<void (Signature...)> Handler;
    typedef unsigned int Handle;
    virtual void
    Trigger (Signature... Args)
    {
      for (std::pair<Handle, Handler> pair : m_OnceHandlers) {
        if (pair.second != NULL) {
          pair.second (Args...);
        }
      }
      m_OnceHandlers.clear ();
      for (std::pair<Handle, Handler> pair : m_Handlers) {
        if (pair.second != NULL) {
          pair.second (Args...);
        }
      }
    };

    Handle
    On (Handler aHandler)
    {
      Handle id = nextId;
      nextId++;
      m_Handlers[id] = aHandler;
      return id;
    };
    Handle
    Once (Handler aHandler)
    {
      Handle id = nextId;
      nextId++;
      m_OnceHandlers[id] = aHandler;
      return id;
    };

    bool
    Delete (Handle aId)
    {
      return m_Handlers.erase (aId) == 1;
    };
    const Handler pipe = [this] (Signature... e) { this->Trigger (e...); };

  private:
    std::map<Handle, Handler> m_Handlers;
    std::map<Handle, Handler> m_OnceHandlers;
    Handle nextId = 1;
  };

}