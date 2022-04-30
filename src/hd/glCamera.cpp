#include "hd/glCamera.hpp"
namespace hd::gl {

  Camera::Camera (glm::vec3 p) : Position (p)
  {
    hdDebugCall (NULL);
    on.Mouse.Button[0].Down.Void.Add (StartTrackingMouse);
    on.Mouse.Button[0].Up.Void.Add (StopTrackingMouse);
    //on.Mouse.Motion.Add (TrackingListener);

    on.Key.Keycode[SDLK_w].Down.Void.Add (MoveForward);
    on.Key.Keycode[SDLK_s].Down.Void.Add (MoveBackward);
    on.Key.Keycode[SDLK_a].Down.Void.Add (StrafeLeft);
    on.Key.Keycode[SDLK_d].Down.Void.Add (StrafeRight);
    on.Key.Keycode[SDLK_LCTRL].Down.Void.Add (MoveUp);
    on.Key.Keycode[SDLK_SPACE].Down.Void.Add (MoveDown);
    on.Key.Keycode[SDLK_LSHIFT].Down.Void.Add (RunSpeed);
    on.Key.Keycode[SDLK_LSHIFT].Up.Void.Add (WalkSpeed);
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