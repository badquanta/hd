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
#pragma once
#include "hd/Common.hpp"
#include <map>
namespace hd::joy {
  class Device {
  public:
    /**
  SDL_JoystickCurrentPowerLevel
  SDL_JoystickEventState
  **/
    /** SDL_JoystickFromInstanceID **/
    static Ptr FromInstanceId (SDL_JoystickID);
    /**SDL_JoystickGetAttached **/
    bool IsAttached();
    /**SDL_JoystickGetAxis **/
    Sint16 GetAxis(int);
	/** SDL_JoystickGetBall**/
    GetBall(int);
	/**SDL_JoystickGetButton**/
    GetButton()
	/** SDL_JoystickGetDeviceGUID**/
	/** SDL_JoystickGetGUID**/
	/**
                SDL_JoystickGetGUIDFromString**/
	/** SDL_JoystickGetGUIDString**/
	/**
                    SDL_JoystickGetHat**/
	/** SDL_JoystickInstanceID**/
	/** SDL_JoystickName**/
	/**
                        SDL_JoystickNameForIndex**/
	/** SDL_JoystickNumAxes**/
	/**
                            SDL_JoystickNumBalls**/
	/** SDL_JoystickNumButtons**/
	/**
                                SDL_JoystickNumHats**/
	/** SDL_JoystickOpen**/
	/**
                                    SDL_JoystickPowerLevel ** /
        /** SDL_JoystickUpdate **/
    /** @see SDL_NumJoysticks **/
    static int Count ();
    typedef std::shared_ptr<Device> Ptr;

    Ptr Open (int);
    ~Device ();

  private:
    static std::map<int, std::weak_ptr<Device> > m_Opened;
    static std::map<SDL_Joystick*, std::weak_ptr<Device> > m_OpenedByPtr;
    SDL_Joystick *m_Joystick;
    int m_Index;
    Device (SDL_Joystick *);
  };
}