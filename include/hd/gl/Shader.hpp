#pragma once
#include "hd/Common.hpp"
namespace hd {
  namespace gl {
    class Shader {
    private:
      GLuint ID;

    public:
      Shader ();
      Shader (GLenum);
      ~Shader ();
      void setSource (const GLchar *);
      bool loadSource (std::filesystem::path);
      bool compile ();
      void printLog (FILE *);
      GLuint GetId ();
      void create (GLenum type);
      void Free ();
    };
  }
}