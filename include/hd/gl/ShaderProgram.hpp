#pragma once
#include "hd/gl/Shader.hpp"
namespace hd {
  /** shader programs **/
  namespace gl {
    /**
     * @brief represents an opengl shader program
     *
     */
    class ShaderProgram {
    private:
      GLuint ID;

    public:
      /** Does nothing **/
      ShaderProgram ();
      /** Destroys program **/
      ~ShaderProgram ();
      /** Attach a shader by `hd::gl::Shader` reference. **/
      void Attach (Shader&);
      /** Attach a shader by ID **/
      void Attach (GLuint);
      /** Attempt to link.
       * @returns true on success
       **/
      bool Link ();
      /** Attempt to create a shader program.
       * @returns true on success
       **/
      bool Create ();
      /** This will:
       * 1. Load aVertexSource, aFragmentSource files into shaders.
       * 2. Compile them.
       * 3. And link them to the Program.
       * @attention this does not keep the shaders active and on success or
       *failure they will be deleted.
       **/
      bool Create (std::filesystem::path, std::filesystem::path);
      /** This will delete the open gl shader program. **/
      void Free ();
      /** This will attempt to bind the shader program.
       * @return true on success
       **/
      bool Bind ();
      /**
       * @brief clear the currently binded shader program.
       * @todo should I just make this static?
       */
      void Unbind ();
      /** just print out the shader compile log to this file. **/
      void PrintLog (FILE *);
      /** just print out the active shader attributes to this file. **/
      void PrintAttribues (FILE *);
      /** return the OpenGL Identifier for this program. **/
      GLuint GetId ();
      /** Lookup the location (index) of the attribute with the specified name. **/
      GLint GetAttribLocation (const GLchar *);
      /** Lookup the location (index) of the uniform with name specified. **/
      GLint getUniformLocation (const GLchar *);
    };
  }
}