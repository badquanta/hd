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
      /** This will:
       * 1. Load aVertexSource, aFragmentSource files into shaders.
       * 2. Compile them.
       * 3. And link them to the Program.
       **/
      bool create (std::filesystem::path, std::filesystem::path);
      void free ();
      bool Bind ();
      void unbind ();
      void printLog (FILE *);
      GLuint getId ();
      GLint getAttribLocation (const GLchar *);
      GLint getUniformLocation (const GLchar *);
    };
  }
}