#include "hd/glCamera.hpp"
namespace hd::gl {

  Camera::Camera (glm::vec3 p) : Position (p)
  {
    hdDebugCall (NULL);
    on.Mouse.Button[0].Down.Void.On (StartTrackingMouse);
    on.Mouse.Button[0].Up.Void.On (StopTrackingMouse);
    //on.Mouse.Motion.Add (TrackingListener);

    on.Key.Keycode[SDLK_w].Down.Void.On (MoveForward);
    on.Key.Keycode[SDLK_s].Down.Void.On (MoveBackward);
    on.Key.Keycode[SDLK_a].Down.Void.On (StrafeLeft);
    on.Key.Keycode[SDLK_d].Down.Void.On (StrafeRight);
    on.Key.Keycode[SDLK_LCTRL].Down.Void.On (MoveUp);
    on.Key.Keycode[SDLK_SPACE].Down.Void.On (MoveDown);
    on.Key.Keycode[SDLK_LSHIFT].Down.Void.On (RunSpeed);
    on.Key.Keycode[SDLK_LSHIFT].Up.Void.On (WalkSpeed);
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
                  Program &aProgram,
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