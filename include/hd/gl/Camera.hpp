/**
 * @file Camera.hpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief Simple implementation of OpenGL Camera.
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
#include "hd/sdl/WindowDispatch.hpp"
#include "hd/gl/ShaderProgram.hpp"

namespace hd {
  namespace gl {
    class Camera {
    private:
      glm::vec3 Position;
      glm::vec3 Orientation = glm::vec3 (0.0f, 0.0f, -1.0f);
      glm::vec3 Up = glm::vec3 (0.0f, 1.0f, 0.0f);
      bool firstClick = true;
      int width;
      int height;
      float speed = 0.1f;
      float sensitivity = 0.10f;

    public:
      Camera (glm::vec3 p = {0.0f,0.0f,1.0f});
      /**
       * @brief Redefine the camera.
       *
       */
      void Matrix (int, int, float, float, float, ShaderProgram &, const char *);
      sdl::WindowDispatch input;

      // Event Handlers
      VoidDispatch::Handler StartTrackingMouse = [this] () {
        hdDebug ("Start Tracking Mouse");
        SDL_SetRelativeMouseMode (SDL_TRUE);
        TrackingListenerHandle = input.Mouse.Motion.On(TrackingListener);
      };
      VoidDispatch::Handler StopTrackingMouse = [this] () {
        hdDebug ("Stop Tracking Mouse");
        SDL_SetRelativeMouseMode (SDL_FALSE);
        input.Mouse.Motion.Delete (TrackingListenerHandle);
      };
      sdl::EventDispatch::Handler TrackingListener = [this] (const SDL_Event&e) {
        //hdDebug ("Tracking Listener");
        if (e.motion.state & SDL_BUTTON_LMASK) {
          glm::vec3 newOrientation
              = glm::rotate (Orientation,
                             glm::radians (sensitivity * (float)e.motion.yrel),
                             glm::normalize (glm::cross (Orientation, Up)));
          if (abs (glm::angle (newOrientation, Up) - glm::radians (90.0f))
              <= glm::radians (85.0f)) {
            Orientation = newOrientation;
          }
          Orientation
              = glm::rotate (Orientation,
                             glm::radians (sensitivity * (float)e.motion.xrel),
                             Up);
        }
      };
      sdl::EventDispatch::Handle TrackingListenerHandle;
      VoidDispatch::Handler MoveForward = [this] () {
        hdDebug ("Move Forward");
        Position += speed * Orientation;
      };
      VoidDispatch::Handler MoveBackward = [this] () {
        hdDebug ("Move Backward");
        Position += speed * -Orientation;
      };
      VoidDispatch::Handler StrafeLeft = [this] () {
        hdDebug ("Strafe Left");
        Position += speed * -glm::normalize (glm::cross (Orientation, Up));
      };
      VoidDispatch::Handler StrafeRight = [this] () {
        hdDebug ("Strafe Right");
        Position += speed * glm::normalize (glm::cross (Orientation, Up));
      };
      VoidDispatch::Handler MoveUp = [this] () {
        hdDebug ("Move Up");
        Position += speed * Up;
      };
      VoidDispatch::Handler MoveDown = [this] () {
        hdDebug ("Move Down");
        Position += speed * -Up;
      };
      VoidDispatch::Handler WalkSpeed = [this] () {
        hdDebug ("Walk Speed");
        speed = 0.1f;
      };
      VoidDispatch::Handler RunSpeed = [this] () {
        hdDebug ("Run Speed");
        speed = 0.4f;
      };
    };
  }
}