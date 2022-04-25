#pragma once
#include "hd/Common.hpp"
#include "hd/glProgram.hpp"
#include "hd/Events.hpp"

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
      Camera (int, int, glm::vec3);
      void Matrix (float, float, float, Program &, const char *);
      TypeEvent on;
    };
  }
}