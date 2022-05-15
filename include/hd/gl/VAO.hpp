#pragma once
#include "hd/gl/VBO.hpp"
#include "hd/gl/ShaderProgram.hpp"
namespace hd {
  namespace gl {
    /**
     * @brief Vertex Attribute Object
     *
     */
    class VAO {
      private:
      GLuint ID;
      public:
      /** Create a NULL vertex attribute object **/
      VAO();
      /** Create a new vertex attribute object **/
      void Create ();
      /** Link the attributes from this object to a program location **/
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
      bool LinkAttrib (VBO &aVBO, ShaderProgram &aProgram, GLchar* aName,GLuint aNumComponents, GLenum aType, GLsizeiptr aStride, void* aOffset);
      /** Bind these vertex attributes to the current opengl context. **/
      void Bind ();
      /** Unbind these vertex attributes from this opengl context. **/
      void Unbind();
      /** Release this vertex attribute object. **/
      void Delete();
    };
  }
}