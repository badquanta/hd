/**
 * @file ControllerDeviceDispatch.hpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-15
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
#include "hd/sdl/ControllerDispatch.hpp"
namespace hd::sdl {
  /**
   * @brief Forward events to controller specific dispatchers.
   *
   */
  class ControllerDeviceDispatch
      : public std::map<SDL_JoystickID, ControllerDispatch>,
        public EventDispatch {
  public:
    EventDispatch /** detection of new device **/ Added,
        /** detection of device departure **/ Removed;
    /** Disbatch GameController events between Added, Removed general handlers,
     * and the specific handlers for each individual controller. **/
    virtual void Trigger (const SDL_Event &) override;
  };

}