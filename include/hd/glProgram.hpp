#pragma once
#include "hd/Common.hpp"
#include "hd/glShader.hpp"
namespace hd {
  namespace gl {
    class Program {
    private:
      GLuint ID;

    public:
      Program ();
      ~Program ();
      void attach (Shader);
      void attach (GLuint);
      bool link ();
      bool create ();
      void free ();
      bool bind ();
      void unbind ();
      void printLog (FILE *);
      GLuint getId ();
      GLint getAttribLocation (const GLchar *);
    };
  }
}