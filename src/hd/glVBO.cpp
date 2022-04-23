#include "hd/glVBO.hpp"
namespace hd::gl {
  VBO::VBO () {}
  void
  VBO::Create (GLfloat *aVertices, GLsizeiptr aSize)
  {
    glGenBuffers (1, &ID);
    glBindBuffer (GL_ARRAY_BUFFER, ID);
    glBufferData (GL_ARRAY_BUFFER, aSize, aVertices, GL_STATIC_DRAW);
  }
  void
  VBO::Bind ()
  {
    glBindBuffer (GL_ARRAY_BUFFER, ID);
  }
  void
  VBO::Unbind ()
  {
    glBindBuffer (GL_ARRAY_BUFFER, 0);
  }
  void
  VBO::Delete ()
  {
    glDeleteBuffers (1, &ID);
  }
}