#pragma once
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
      bool Create ();
      /** This will:
       * 1. Load aVertexSource, aFragmentSource files into shaders.
       * 2. Compile them.
       * 3. And link them to the Program.
       **/
      bool Create (std::filesystem::path, std::filesystem::path);
      void free ();
      bool Bind ();
      void unbind ();
      void printLog (FILE *);
      void printAttribues (FILE *);
      GLuint getId ();
      GLint getAttribLocation (const GLchar *);
      GLint getUniformLocation (const GLchar *);
    };
  }
}