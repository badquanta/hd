#pragma once
#include "hd/Common.hpp"
namespace hd {
  namespace gl {
    class VBO {
    private:
      GLuint ID;

    public:
      VBO ();
      void Create (GLfloat *, GLsizeiptr);
      void Bind ();
      void Unbind ();
      void Delete ();
    };
  }
}