#include "hd/gl/VBO.hpp"
namespace hd::gl {
  VBO::VBO () {}
  VBO::VBO (GLfloat *aVertices, GLsizeiptr aSize)
  {
    hdDebugCall ("%p, %p", aVertices, aSize);
    Create (aVertices, aSize);
  }
  void
  VBO::Create (GLfloat *aVertices, GLsizeiptr aSize)
  {
    hdDebugCall ("%p, %p", aVertices, aSize);
    glGenBuffers (1, &ID);
    //glBindBuffer (GL_ARRAY_BUFFER, ID);
    Bind();
    glBufferData (GL_ARRAY_BUFFER, aSize, aVertices, GL_STATIC_DRAW);
  }
  void
  VBO::Bind ()
  {
    glBindBuffer (GL_ARRAY_BUFFER, ID);
    GLenum err=glGetError();
    if(err!=GL_NO_ERROR){
      hdError ("Error binding VAO: %s", gluErrorString (err));
    }
  }
  void
  VBO::Unbind ()
  {
    hdDebugCall (NULL);
    glBindBuffer (GL_ARRAY_BUFFER, 0);
  }
  void
  VBO::Delete ()
  {
    hdDebugCall (NULL);
    glDeleteBuffers (1, &ID);
  }
}