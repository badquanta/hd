#pragma once
#include "hd/Common.hpp"
#include "hd/glProgram.hpp"
namespace hd{
  namespace gl{
    class Texture {
      private:
        GLuint ID;
        GLenum type;
      public:
        bool Create (std::filesystem::path, GLenum, GLenum);
        void Assign (Program &, const char *, GLuint);
        void Bind ();
        void Unbind ();
        void Delete ();
    };
  }
}