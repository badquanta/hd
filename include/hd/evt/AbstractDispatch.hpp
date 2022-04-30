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
#include <functional>
#include <map>
namespace hd::evt {
     template <class...Signature> class AbstractDispatch {
    public:
      virtual ~AbstractDispatch (){};
      typedef std::function<void (Signature...)> Handler;
      typedef unsigned int Handle;
      virtual void Trigger (Signature...Args){
        for (std::pair<Handle, Handler> pair : m_Handlers) {
          if(pair.second!=NULL){
          pair.second (Args...);}
        }
      };


       Handle Add (Handler aHandler)
      {
        Handle id = nextId;
        nextId++;
        m_Handlers[id] = aHandler;
        return id;
      };

      bool Delete(Handle aId){
        return m_Handlers.erase (aId) == 1;
      };
      const Handler pipe = [this] (Signature... e) { this->Trigger (e...); };

    private:
      std::map<Handle,Handler> m_Handlers;
      Handle nextId = 1;
  };

}