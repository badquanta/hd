#pragma once
#include "hd/Common.hpp"
#include "hd/glVBO.hpp"
#include "hd/glProgram.hpp"
namespace hd {
  namespace gl {
    class VAO {
      private:
      GLuint ID;
      public:
      VAO();
      void Create ();
      void LinkAttrib (VBO &VBO, GLuint layout, GLuint aNumComponents, GLenum aType, GLsizeiptr aStride, void* aOffset);
      /**
       * @brief
       *
       * @param aVBO the VBO to link
       * @param aProgram the Shader program to link it to
       * @param aName name of variable to link it to
       * @return true
       * @return false
       */
      bool LinkAttrib (VBO &aVBO, Program &aProgram, GLchar* aName,GLuint aNumComponents, GLenum aType, GLsizeiptr aStride, void* aOffset);
      void Bind ();
      void Unbind();
      void Delete();
    };
  }
}