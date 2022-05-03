#include "hd/gl/EBO.hpp"
namespace hd::gl {
  EBO::EBO () {}
  EBO::EBO (GLuint *data, GLsizeiptr size) { Create (data, size); }
  void
  EBO::Create (GLuint *indices, GLsizeiptr size)
  {
    hdDebugCall (NULL);
    glGenBuffers (1, &ID);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
  }
  void
  EBO::Bind ()
  {
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ID);
  }
  void
  EBO::Unbind ()
  {
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  void
  EBO::Delete ()
  {
    glDeleteBuffers (1, &ID);
  }
}