#include "hd/glCamera.hpp"
namespace hd::gl {
  Camera::Camera (int w, int h, glm::vec3 p)
      : width (w), height (h), Position (p)
  {
    on.Mouse.Button[0].Down.Add (
        [this] (const SDL_Event &e) { SDL_SetRelativeMouseMode (SDL_TRUE); });
    on.Mouse.Button[0].Up.Add (
        [this] (const SDL_Event &e) { SDL_SetRelativeMouseMode (SDL_FALSE); });
    on.Mouse.Motion.Add ([this] (const SDL_Event &e) {
      if (e.motion.state & SDL_BUTTON_LMASK) {
        glm::vec3 newOrientation
            = glm::rotate (Orientation,
                           glm::radians (sensitivity * (float)e.motion.xrel),
                           glm::normalize (glm::cross (Orientation, Up)));
        if (abs (glm::angle (newOrientation, Up) - glm::radians (90.0f))
            <= glm::radians (85.0f)) {
          Orientation = newOrientation;
        }
        Orientation
            = glm::rotate (Orientation,
                           glm::radians (sensitivity * (float)e.motion.yrel),
                           Up);
      }
    });
    on.Key.Keycode[SDLK_w].Down.Add (
        [this] (const SDL_Event &e) { Position += speed * Orientation; });
    on.Key.Keycode[SDLK_a].Down.Add ([this] (const SDL_Event &e) {
      Position += speed * -glm::normalize (glm::cross (Orientation, Up));
    });
    on.Key.Keycode[SDLK_s].Down.Add (
        [this] (const SDL_Event &e) { Position += speed * -Orientation; });
    on.Key.Keycode[SDLK_d].Down.Add ([this] (const SDL_Event &e) {
      Position += speed * glm::normalize (glm::cross (Orientation, Up));
    });
    on.Key.Keycode[SDLK_LCTRL].Down.Add (
        [this] (const SDL_Event &e) { Position += speed * Up; });
    on.Key.Keycode[SDLK_LSHIFT].Down.Add (
        [this] (const SDL_Event &e) { speed = 0.4f; });
    on.Key.Keycode[SDLK_LSHIFT].Up.Add (
        [this] (const SDL_Event &e) { speed = 0.1f; });
  }
  void
  Camera::Matrix (float aFOVdeg,
                  float aNearPlane,
                  float aFarPlane,
                  Program &aProgram,
                  const char *aUniformName)
  {
    glm::mat4 view = glm::mat4 (1.0f);
    glm::mat4 projection = glm::mat4 (1.0f);
    view = glm::lookAt (Position, Position + Orientation, Up);
    projection = glm::perspective (glm::radians (aFOVdeg),
                                   (float)width / (float)height,
                                   aNearPlane,
                                   aFarPlane);

    glUniformMatrix4fv (aProgram.getUniformLocation (aUniformName),
                        1,
                        GL_FALSE,
                        glm::value_ptr (projection * view));
  }
}