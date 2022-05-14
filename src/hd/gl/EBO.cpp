#include "hd/gl/EBO.hpp"
#include "hd/Error.hpp"
#include "hd/Debug.hpp"
namespace hd::gl {
  EBO::EBO () {}
  EBO::EBO (GLuint *data, GLsizeiptr size) { Create (data, size); }
  void
  EBO::Create (GLuint *indices, GLsizeiptr size)
  {
    hdDebugCall ("%p, %p", indices, size);
    glGenBuffers (1, &ID);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
  }
  void
  EBO::Bind ()
  {
    // hdDebugCall (NULL);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ID);
  }
  void
  EBO::Unbind ()
  {
    hdDebugCall (NULL);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  void
  EBO::Delete ()
  {
    hdDebugCall (NULL);
    glDeleteBuffers (1, &ID);
  }
}