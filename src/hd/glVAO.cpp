#include "hd/glVAO.hpp"
namespace hd::gl {
  VAO::VAO () {hdDebugCall (NULL);}
  void
  VAO::Create ()
  {hdDebugCall (NULL);
    glGenVertexArrays (1, &ID);
  }
  void
  VAO::LinkAttrib (VBO &aVbo, GLuint aLayout,GLuint aNumComponents, GLenum aType, GLsizeiptr aStride, void* aOffset)
  {hdDebugCall (NULL);
    aVbo.Bind ();
    glVertexAttribPointer (aLayout, aNumComponents, aType, GL_FALSE, aStride, aOffset);
    glEnableVertexAttribArray (aLayout);
    aVbo.Unbind ();
  }
  bool
  VAO::LinkAttrib (VBO &aVbo, Program &aProgram, GLchar* aName,GLuint aNumComponents, GLenum aType, GLsizeiptr aStride, void* aOffset){
    GLint aLayout = aProgram.getAttribLocation (aName);
    if(aLayout==-1){
      fprintf (stderr,"Failed to get variable named %s\n", aName);
      return false;
    }
    LinkAttrib (aVbo, aLayout,aNumComponents,aType,aStride,aOffset);
    return true;
  }
  void
  VAO::Bind ()
  {
    glBindVertexArray (ID);
  }
  void
  VAO::Unbind ()
  {
    glBindVertexArray (0);
  }
  void
  VAO::Delete ()
  {
    glDeleteVertexArrays (1, &ID);
  }
}