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
  bool
  Program::create (std::filesystem::path aVertexSource,
                   std::filesystem::path aFragmentSource)
  {
    create ();
    Shader vertex (GL_VERTEX_SHADER), fragment (GL_FRAGMENT_SHADER);
    if (!vertex.loadSource (aVertexSource)) {
      fprintf (stderr,
               "Failed to load Vertex Shader Source from '%s'\n",
               aVertexSource.generic_string ().c_str ());
      return false;
    };
    if (!fragment.loadSource (aFragmentSource)) {
      fprintf (stderr,
               "Failed to load Fragment Shader Source from '%s'\n",
               aFragmentSource.generic_string ().c_str ());
      return false;
    }
    if(!vertex.compile()){
      fprintf (stderr, "Failed to compile Vertex Shader, log:\n");
      vertex.printLog (stderr);
      return false;
    }
    if(!fragment.compile()){
      fprintf (stderr, "Failed to compile Fragment Shader, log:\n");
      fragment.printLog (stderr);
      return false;
    }
    attach (vertex);
    attach (fragment);
    if(!link ()){
      fprintf (stderr, "Failed to link shader program %d, log:\n", ID);
      printLog (stderr);
      return false;
    }
    return true;
  }
  void
  Program::free ()
  {
    if (ID != 0) {
      glDeleteProgram (ID);
      ID = 0;
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
  GLint
  Program::getUniformLocation(const GLchar *name){
    GLint result = glGetUniformLocation (ID, name);
    if(result == -1){
      fprintf (stderr, "Invalid uniform name '%s'.\n", name);
    }
    return result;
  }
  bool
  Program::Bind ()
  {
    if (ID == 0) {
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
  void
  Program::printAttribues(FILE*stream=stdout){
    GLint count;
    glGetProgramiv (ID, GL_ACTIVE_ATTRIBUTES, &count);
    fprintf (stream, "Active Attribute count: %d\n", count);
    const GLsizei maxNameLength = 32;
    GLint size;
    GLenum type;
    GLchar name[maxNameLength];
    GLsizei length;
    for (int i = 0; i < count; i++) {
      glGetActiveAttrib (
          ID, (GLuint)i, maxNameLength, &length, &size, &type, name);
      fprintf (stream, "Attribute#%d Type: %u Name: %s\n", i, type, name);
    }
  }
}