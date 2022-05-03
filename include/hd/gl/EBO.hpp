#pragma once
#include "hd/Common.hpp"
namespace hd {
  namespace gl {
    class EBO {
      private:
        GLuint ID;
      public:
        EBO();
        EBO (GLuint *data, GLsizeiptr size);

        void Create (GLuint *, GLsizeiptr);

        void Bind ();
        void Unbind();
        void Delete();
    };
  }
}