/**
 * @file Camera.hpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief Simple implementation of OpenGL Camera.
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
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "hd/Debug.hpp"
#include "hd/Error.hpp"
#include "hd/gl/ShaderProgram.hpp"
#include "hd/sdl/events.hpp"
#include <glm/vec3.hpp>
namespace hd {
  namespace gl {
    /**
     * @brief simple test implementation of sdl camera.
     *
     */
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
      /**
       * @brief Construct a new Camera object
       *
       * @param p
       */
      Camera (glm::vec3 p = { 0.0f, 0.0f, 1.0f });
      /**
       * @brief Redefine the camera.
       *
       */
      void
      Matrix (int, int, float, float, float, ShaderProgram &, const char *);
      /**
       * @brief user controls
       * @todo will be removing this soon to the "ui" family of objects.
       *
       */
      sdl::WindowDispatch input;

      /** @brief Start tracking mouse motion.**/
      VoidSignatureDispatch::Handler StartTrackingMouse = [this] () {
        hdDebug ("Start Tracking Mouse");
        SDL_SetRelativeMouseMode (SDL_TRUE);
        TrackingListenerHandle = input.mouse.motion.On (TrackingListener);
      };
      /** @brief Stop tracking mouse motion by disconnecting the Mouse.motion
       * handler. */
      VoidSignatureDispatch::Handler StopTrackingMouse = [this] () {
        hdDebug ("Stop Tracking Mouse");
        SDL_SetRelativeMouseMode (SDL_FALSE);
        input.mouse.motion.Delete (TrackingListenerHandle);
      };
      /** @brief connected to the Mouse.motion dispatcher when tracking mouse.
       * **/
      sdl::EventDispatch::Handler TrackingListener = [this] (
                                                         const SDL_Event &e) {
        // hdDebug ("Tracking Listener");
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
      /** Handle kept so that we can later remove the Mouse motion tracking
       * listener **/
      sdl::EventDispatch::Handle TrackingListenerHandle;
      /** @brief Move the camera forward **/
      VoidSignatureDispatch::Handler MoveForward = [this] () {
        hdDebug ("Move Forward");
        Position += speed * Orientation;
      };
      /** @brief Move the camera backward **/
      VoidSignatureDispatch::Handler MoveBackward = [this] () {
        hdDebug ("Move Backward");
        Position += speed * -Orientation;
      };
      /** @brief Strafe the camera left. **/
      VoidSignatureDispatch::Handler StrafeLeft = [this] () {
        hdDebug ("Strafe Left");
        Position += speed * -glm::normalize (glm::cross (Orientation, Up));
      };
      /** @brief Strafe the camera right **/
      VoidSignatureDispatch::Handler StrafeRight = [this] () {
        hdDebug ("Strafe Right");
        Position += speed * glm::normalize (glm::cross (Orientation, Up));
      };
      /** @brief Strafe the camera up **/
      VoidSignatureDispatch::Handler MoveUp = [this] () {
        hdDebug ("Move Up");
        Position += speed * Up;
      };
      /** @brief Strafe the camera right **/
      VoidSignatureDispatch::Handler MoveDown = [this] () {
        hdDebug ("Move Down");
        Position += speed * -Up;
      };
      /** @brief Set the movement speed to "walkSpeed" **/
      VoidSignatureDispatch::Handler WalkSpeed = [this] () {
        hdDebug ("Walk Speed");
        speed = 0.1f;
      };
      /** @brief Set the movement speed to "run" speed **/
      VoidSignatureDispatch::Handler RunSpeed = [this] () {
        hdDebug ("Run Speed");
        speed = 0.4f;
      };
    };
  }
}