#pragma once
#include <GL/glew.h>
namespace hd {
  namespace gl {
    class VBO {
    private:
      GLuint ID;

    public:
      VBO ();
      VBO (GLfloat *, GLsizeiptr);
      void Create (GLfloat *, GLsizeiptr);
      void Bind ();
      void Unbind ();
      void Delete ();
    };
  }
}