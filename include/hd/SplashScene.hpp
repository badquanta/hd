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
#include "hd/Scene.hpp"
#include "hd/Shared.hpp"
/** **/
namespace hd
{

  /**
   *
   */
  class SplashScene : public Scene
  {
  private:
    Shared::Texture splash, up, down, left, right,textMsg;
    const Uint8 *keyState;
    Shared::Font font;

  public:
    SplashScene ();
    virtual bool load (SDL_Renderer *) override;
    virtual void unload () override;
    virtual void render (SDL_Renderer *r) override;

  };
}