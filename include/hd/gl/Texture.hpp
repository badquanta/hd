#pragma once
#include "hd/gl/ShaderProgram.hpp"
namespace hd{
  namespace gl{
    class Texture {
      private:
        GLuint ID;
        GLenum type;
      public:
        bool Create (std::filesystem::path, GLenum, GLenum);
        void Assign (ShaderProgram &, const char *, GLuint);
        void Bind ();
        void Unbind ();
        void Delete ();
    };
  }
}