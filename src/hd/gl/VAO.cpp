#include "hd/gl/VAO.hpp"
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
  VAO::LinkAttrib (VBO &aVbo, ShaderProgram &aProgram, GLchar* aName,GLuint aNumComponents, GLenum aType, GLsizeiptr aStride, void* aOffset){
    GLint aLayout = aProgram.GetAttribLocation (aName);
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
    GLenum err=glGetError();
    if(err!=GL_NO_ERROR){
      hdError ("Error binding VAO: %s", gluErrorString (err));
    }
  }
  void
  VAO::Unbind ()
  {
    hdDebugCall (NULL);
    glBindVertexArray (0);
  }
  void
  VAO::Delete ()
  {
    hdDebugCall (NULL);
    glDeleteVertexArrays (1, &ID);
  }
}