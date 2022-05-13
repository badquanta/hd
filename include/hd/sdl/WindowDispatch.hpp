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
#include "hd/sdl/KeyboardDispatch.hpp"
#include "hd/sdl/MouseDispatch.hpp"
#include "hd/sdl/TextDispatch.hpp"
#include "hd/sdl/DropDispatch.hpp"
namespace hd::sdl {
    class WindowDispatch : public EventDispatch {
  public:
    KeyboardDispatch Key;
    MouseDispatch Mouse;
    TextDispatch Text;
    DropDispatch Drop;
    EventDispatch Shown, Hidden, Exposed, Moved, Resized, SizeChanged, Minimized,
        Maximized, Restored, Enter, Leave, FocusGained, FocusLost, Close,
        TakeFocus, HitTest;
    virtual void Trigger (const SDL_Event &) override;
  };

}