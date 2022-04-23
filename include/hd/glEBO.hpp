#pragma once
#include "hd/Common.hpp"
namespace hd {
  namespace gl {
    class EBO {
      private:
        GLuint ID;
      public:
        EBO();
        void Create (GLuint *, GLsizeiptr);
        void Bind ();
        void Unbind();
        void Delete();
    };
  }
}