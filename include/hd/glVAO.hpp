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
      void LinkVBO (VBO &VBO, GLuint layout);
      /**
       * @brief
       *
       * @param aVBO the VBO to link
       * @param aProgram the Shader program to link it to
       * @param aName name of variable to link it to
       * @return true
       * @return false
       */
      bool LinkVBO (VBO &aVBO, Program &aProgram, std::string aName);
      void Bind ();
      void Unbind();
      void Delete();
    };
  }
}