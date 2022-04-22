#include "hd/glProgram.hpp"

namespace hd::gl {
  Program::Program () { ID = 0; }
  Program::~Program () { free (); }
  bool
  Program::create ()
  {
    if (ID == 0) {
      free ();
    }
    ID = glCreateProgram ();
    return ID != 0;
  }
  void
  Program::free ()
  {
    if (ID != 0) {
      glDeleteProgram (ID);
      ID = NULL;
    }
  }
  void
  Program::attach (Shader aShader)
  {
    attach (aShader.getId ());
  }
  void
  Program::attach (GLuint aShaderId)
  {
    glAttachShader (ID, aShaderId);
  }
  bool
  Program::link ()
  {
    glLinkProgram (ID);
    GLint success = GL_FALSE;
    glGetProgramiv (ID, GL_LINK_STATUS, &success);
    return (success == GL_TRUE);
  }
  GLint
  Program::getAttribLocation (const GLchar *name)
  {
    GLint result = glGetAttribLocation (ID, name);
    if (result == -1) {
      fprintf (stderr, "Invalid attribute name '%s'\n", name);
    }
    return result;
  }
  bool
  Program::bind ()
  {
    if (ID == NULL) {
      return false;
    }

    glUseProgram (ID);
    GLenum error = glGetError ();
    if (error != GL_NO_ERROR) {
      fprintf (stderr,
               "Failed binding shader program (%d) because: %s\n",
               ID,
               gluErrorString (error));
      printLog (stderr);
      return false;
    }
    return true;
  }
  void
  Program::unbind ()
  {
    glUseProgram (0);
  }
  void
  Program::printLog (FILE *stream = stdout)
  {
    if (glIsProgram (ID)) {
      int logLength = 0, maxLength = 0;
      glGetProgramiv (ID, GL_INFO_LOG_LENGTH, &maxLength);
      char *infoLog = new char[maxLength];
      glGetProgramInfoLog (ID, maxLength, &logLength, infoLog);
      if (logLength > 0) {
        fprintf (stream, "%s\n", infoLog);
      }
      delete[] infoLog;
    } else {
      fprintf (stream, "Name %d is not a program\n", ID);
    }
  }
}