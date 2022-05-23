#include "hd/gl/Camera.hpp"
#include "glm/gtc/type_ptr.hpp"
namespace hd::gl {

  Camera::Camera (glm::vec3 p) : Position (p)
  {
    hdDebugCall (NULL);
    input.mouse.button[SDL_BUTTON_LEFT].down.Void.On (StartTrackingMouse);
    input.mouse.button[SDL_BUTTON_LEFT].up.Void.On (StopTrackingMouse);
    //on.mouse.motion.Add (TrackingListener);

    input.key.keycode[SDLK_w].down.Void.On (MoveForward);
    input.key.keycode[SDLK_s].down.Void.On (MoveBackward);
    input.key.keycode[SDLK_a].down.Void.On (StrafeLeft);
    input.key.keycode[SDLK_d].down.Void.On (StrafeRight);
    input.key.keycode[SDLK_LCTRL].down.Void.On (MoveUp);
    input.key.keycode[SDLK_SPACE].down.Void.On (MoveDown);
    input.key.keycode[SDLK_LSHIFT].down.Void.On (RunSpeed);
    input.key.keycode[SDLK_LSHIFT].up.Void.On (WalkSpeed);
  }


  /**
   * @brief
   *
   * @param aWidth Rendering width
   * @param aHeight Rendering height
   * @param aFovDeg Field of view in degrees
   * @param aNearPlane Value to clip beyond in z axis.
   * @param aFarPlane Value to clip near z axis.
   * @param aProgram OpenGL Program to use.
   * @param aUniformName Name of Program Uniform to update.
   */
  void
  Camera::Matrix (int aWidth,
                  int aHeight,
                  float aFovDeg,
                  float aNearPlane,
                  float aFarPlane,
                  ShaderProgram &aProgram,
                  const char *aUniformName)
  {
    // load identity matrixes.
    glm::mat4 view = glm::mat4 (1.0f);
    glm::mat4 projection = glm::mat4 (1.0f);
    //
    float aspect = aWidth / aHeight;
    //
    view = glm::lookAt (Position, Position + Orientation, Up);
    projection = glm::perspective (
        glm::radians (aFovDeg), aspect, aNearPlane, aFarPlane);

    glUniformMatrix4fv (aProgram.getUniformLocation (aUniformName),
                        1,
                        GL_FALSE,
                        glm::value_ptr (projection * view));
  }
}