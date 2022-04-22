#include "hd/glShader.hpp"
#include "hd/Shared.hpp"
#include <fstream>
#include <iostream>
namespace hd::gl {
  Shader::Shader () { ID = 0; }
  Shader::Shader (GLenum type) { create (type); }
  Shader::~Shader () { free (); }
  void
  Shader::setSource (const GLchar *src)
  {
    glShaderSource (ID, 1, &src, NULL);
  }
  bool
  Shader::loadSource (std::filesystem::path aPath)
  {
    std::string content;
    std::filesystem::path realPath = Shared::findRealPath (aPath);
    std::ifstream file (realPath, std::ios::in);
    if (!file.is_open ()) {
      fprintf (stderr,
               "Failed to open file %s\n",
               realPath.generic_string ().c_str ());
      return false;
    }
    std::string line;
    while (!file.eof ()) {
      std::getline (file, line);
      content.append (line + "\n");
    }
    file.close ();
    if (sizeof (char) == sizeof (GLchar)) {
      setSource (content.c_str ());
    } else {
      GLchar *content2 = new GLchar[content.length ()];
      for (int i = 0; i < content.length (); i++) {
        content2[i] = content.at (i);
      }
      setSource (content2);
      delete[] content2;
    }
    return true;
  }
  bool
  Shader::compile ()
  {
    GLint success = GL_FALSE;
    glCompileShader (ID);
    glGetShaderiv (ID, GL_COMPILE_STATUS, &success);
    return (success == GL_TRUE);
  }
  GLuint
  Shader::getId ()
  {
    return ID;
  }
  void
  Shader::printLog (FILE *aFile = stdout)
  {
    if (glIsShader (ID)) {
      int infoLogLength = 0;
      int maxLength = 0;
      glGetShaderiv (ID, GL_INFO_LOG_LENGTH, &maxLength);
      char *infoLog = new char[maxLength];
      glGetShaderInfoLog (ID, maxLength, &infoLogLength, infoLog);
      if (infoLogLength > 0) {
        printf ("%s\n", infoLog);
      } else {
        printf ("Name %d is not a shader.\n", ID);
      }
    }
  }

  void
  Shader::create (GLenum type)
  {
    if (ID != 0) {
      free ();
    }
    ID = glCreateShader (type);
  }
  void
  Shader::free ()
  {
    if (ID != 0) {
      glDeleteShader (ID);
      ID = 0;
    }
  }
}