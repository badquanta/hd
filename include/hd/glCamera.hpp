#pragma once
#include "hd/evt/WindowDispatch.hpp"
#include "hd/glProgram.hpp"

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
      Camera (glm::vec3);
      /**
       * @brief Redefine the camera.
       *
       */
      void Matrix (int, int, float, float, float, Program &, const char *);
      evt::WindowDispatch on;

      // Event Handlers
      evt::VoidDispatch::Handler StartTrackingMouse = [this] () {
        hdDebug ("Start Tracking Mouse");
        SDL_SetRelativeMouseMode (SDL_TRUE);
        TrackingListenerHandle = on.Mouse.Motion.Add(TrackingListener);
      };
      evt::VoidDispatch::Handler StopTrackingMouse = [this] () {
        hdDebug ("Stop Tracking Mouse");
        SDL_SetRelativeMouseMode (SDL_FALSE);
        on.Mouse.Motion.Delete (TrackingListenerHandle);
      };
      evt::SDL_EventDispatch::Handler TrackingListener = [this] (const SDL_Event&e) {
        hdDebug ("Tracking Listener");
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
      evt::SDL_EventDispatch::Handle TrackingListenerHandle;
      evt::VoidDispatch::Handler MoveForward = [this] () {
        hdDebug ("Move Forward");
        Position += speed * Orientation;
      };
      evt::VoidDispatch::Handler MoveBackward = [this] () {
        hdDebug ("Move Backward");
        Position += speed * -Orientation;
      };
      evt::VoidDispatch::Handler StrafeLeft = [this] () {
        hdDebug ("Strafe Left");
        Position += speed * -glm::normalize (glm::cross (Orientation, Up));
      };
      evt::VoidDispatch::Handler StrafeRight = [this] () {
        hdDebug ("Strafe Right");
        Position += speed * glm::normalize (glm::cross (Orientation, Up));
      };
      evt::VoidDispatch::Handler MoveUp = [this] () {
        hdDebug ("Move Up");
        Position += speed * Up;
      };
      evt::VoidDispatch::Handler MoveDown = [this] () {
        hdDebug ("Move Down");
        Position += speed * -Up;
      };
      evt::VoidDispatch::Handler WalkSpeed = [this] () {
        hdDebug ("Walk Speed");
        speed = 0.1f;
      };
      evt::VoidDispatch::Handler RunSpeed = [this] () {
        hdDebug ("Run Speed");
        speed = 0.4f;
      };
    };
  }
}