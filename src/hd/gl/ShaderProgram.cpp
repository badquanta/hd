#include "hd/gl/ShaderProgram.hpp"

namespace hd::gl {
  ShaderProgram::ShaderProgram ()
  {
    hdDebugCall (NULL);
    ID = 0;
  }
  ShaderProgram::~ShaderProgram ()
  {
    hdDebugCall (NULL);
    Free ();
  }
  bool
  ShaderProgram::Create ()
  {
    hdDebugCall (NULL);
    if (ID != 0) {
      Free ();
    }
    ID = glCreateProgram ();
    return ID != 0;
  }
  bool
  ShaderProgram::Create (std::filesystem::path aVertexSource,
                         std::filesystem::path aFragmentSource)
  {
    Create ();
    Shader vertex (GL_VERTEX_SHADER), fragment (GL_FRAGMENT_SHADER);
    if (!vertex.loadSource (aVertexSource)) {
      hdError ("Failed to load Vertex Shader Source from '%s'\n",
               aVertexSource.generic_string ().c_str ());
      return false;
    };
    if (!fragment.loadSource (aFragmentSource)) {
      hdError ("Failed to load Fragment Shader Source from '%s'\n",
               aFragmentSource.generic_string ().c_str ());
      return false;
    }
    if (!vertex.compile ()) {
      hdError ("Failed to compile Vertex Shader, log:\n");
      vertex.printLog (stderr);
      return false;
    }
    if (!fragment.compile ()) {
      hdError ("Failed to compile Fragment Shader, log:\n");
      fragment.printLog (stderr);
      return false;
    }
    Attach (vertex);
    Attach (fragment);
    if (!Link ()) {
      hdError ("Failed to link shader program %d, log:\n", ID);
      PrintLog (stderr);
      return false;
    }
    return true;
  }
  void
  ShaderProgram::Free ()
  {
    hdDebugCall (NULL);
    if (ID != 0) {
      glDeleteProgram (ID);
      ID = 0;
    }
  }
  void
  ShaderProgram::Attach (Shader& aShader)
  {
    Attach (aShader.GetId ());
  }
  void
  ShaderProgram::Attach (GLuint aShaderId)
  {
    hdDebugCall ("shader#%d", aShaderId);
    glAttachShader (ID, aShaderId);
  }
  bool
  ShaderProgram::Link ()
  {
    hdDebugCall (NULL);

    glLinkProgram (ID);
    GLint success = GL_FALSE;
    glGetProgramiv (ID, GL_LINK_STATUS, &success);
    if(success != GL_TRUE){
      hdError ("Unable to link program.");
      PrintLog (stderr);
      hdDebugReturn ("false");
    }
    return (success == GL_TRUE);
  }
  GLint
  ShaderProgram::GetAttribLocation (const GLchar *name)
  {
    hdDebugCall ("%s", name);
    GLint result = glGetAttribLocation (ID, name);
    if (result == -1) {
      hdError ("Invalid attribute name '%s'\n", name);
      PrintAttribues (stderr);
    }
    return result;
  }
  GLint
  ShaderProgram::getUniformLocation (const GLchar *name)
  {
    hdDebugCall ("%s", name);
    GLint result = glGetUniformLocation (ID, name);
    if (result == -1) {
      hdError ("Invalid uniform name '%s'.\n", name);
    }
    return result;
  }
  bool
  ShaderProgram::Bind ()
  {
    if (ID == 0) {
      return false;
    }

    glUseProgram (ID);
    GLenum error = glGetError ();
    if (error != GL_NO_ERROR) {
      hdError ("Failed binding shader program (%d) because: %s\n",
               ID,
               gluErrorString (error));
      PrintLog (stderr);
      hdDebugReturn ("FALSE");
      return false;
    }
    //hdDebugReturn ("true");
    return true;
  }
  void
  ShaderProgram::Unbind ()
  {
    glUseProgram (0);
  }
  void
  ShaderProgram::PrintLog (FILE *stream = stdout)
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
  ShaderProgram::PrintAttribues (FILE *stream = stdout)
  {
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