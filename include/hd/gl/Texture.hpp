#pragma once
#include "hd/gl/ShaderProgram.hpp"
namespace hd{
  namespace gl{
    /**
     * @brief Manage an OpenGL Texture lifespan.
     *
     */
    class Texture {
      private:
        GLuint ID;
        GLenum type;
      public:
        /** Create a new texture & load data from disk. **/
        bool Create (std::filesystem::path, GLenum, GLenum);
        /** Assign the texture to a shader program uniform. **/
        void Assign (ShaderProgram &, const char *, GLuint);
        /** Bind the texture to the current open gl context. **/
        void Bind ();
        /** Unbind the texture from the current open gl context. **/
        void Unbind ();
        /** Release this texture **/
        void Delete ();
    };
  }
}