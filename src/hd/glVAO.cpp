#include "hd/glVAO.hpp"
namespace hd::gl {
  VAO::VAO () {}
  void
  VAO::Create ()
  {
    glGenVertexArrays (1, &ID);
  }
  void
  VAO::LinkVBO (VBO &aVbo, GLuint aLayout)
  {
    aVbo.Bind ();
    glVertexAttribPointer (aLayout, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray (aLayout);
    aVbo.Unbind ();
  }
  bool
  VAO::LinkVBO (VBO &aVbo, Program &aProgram, std::string aName){
    GLint aLayout = aProgram.getAttribLocation (aName.c_str());
    if(aLayout==-1){
      fprintf (stderr,"Failed to get variable named %s\n", aName.c_str ());
      return false;
    }
    LinkVBO (aVbo, aLayout);
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