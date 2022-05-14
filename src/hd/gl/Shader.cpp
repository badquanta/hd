#include "hd/gl/Shader.hpp"
#include "hd/Engine.hpp"
#include <fstream>
#include <iostream>
#include "hd/Debug.hpp"
#include "hd/Error.hpp"
namespace hd::gl {
  Shader::Shader () { hdDebugCall(NULL); ID = 0; }
  Shader::Shader (GLenum type) { hdDebugCall(NULL); create (type); }
  Shader::~Shader () { hdDebugCall(NULL);  }
  void
  Shader::setSource (const GLchar *src)
  {
    hdDebugCall(NULL);
    glShaderSource (ID, 1, &src, NULL);
  }
  bool
  Shader::loadSource (std::filesystem::path aPath)
  {
    hdDebugCall("'%s'",aPath.generic_string().c_str());
    std::string content;
    std::filesystem::path realPath = Engine::FindPath (aPath);
    std::ifstream file (realPath, std::ios::in);
    if (!file.is_open ()) {
      fprintf (stderr,
               "Failed to open file %s\n",
               realPath.generic_string ().c_str ());
      hdDebugReturn ("FALSE");
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
    hdDebugCall (NULL);
    GLint success = GL_FALSE;
    glCompileShader (ID);
    glGetShaderiv (ID, GL_COMPILE_STATUS, &success);
    if(success != GL_TRUE){
      hdError ("Failed to compile shader.");
      printLog (stderr);
      hdDebugReturn ("FALSE");
    }
    return (success == GL_TRUE);
  }
  GLuint
  Shader::GetId ()
  {
    return ID;
  }
  void
  Shader::printLog (FILE *aFile = stdout)
  {
    hdDebugCall (NULL);
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
    hdDebugCall (NULL);
    if (ID != 0) {
      Free ();
    }
    ID = glCreateShader (type);
  }
  void
  Shader::Free ()
  {
    if (ID != 0) {
      hdDebugCall (NULL);
      glDeleteShader (ID);
      ID = 0;
    }
  }
}