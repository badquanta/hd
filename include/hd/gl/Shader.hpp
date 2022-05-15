/**
 * @file Shader.hpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-14
 *
 * @copyright GNU-GPL 3.0 Copyright (C) 2022 Jón Davíð Sawyer
 * (badquanta@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */
#pragma once
#include <GL/glew.h>
#include <filesystem>
namespace hd {
  namespace gl {
    /**
     * @brief Individual shader function.
     *
     */
    class Shader {
    private:
      GLuint ID = 0;
    public:
      /** @brief NULL shader instance **/
      Shader ();
      /** @brief new reference to existing shader **/
      Shader (GLenum);
      /** @brief stop referencing this shader. **/
      ~Shader ();
      /** @brief set shader source code from string **/
      void setSource (const GLchar *);
      /** @brief read shader source code from file **/
      bool loadSource (std::filesystem::path);
      /** @brief attempt to compile shader source code.
       * @return true on success
       * @return false on error, see `printLog`.
       **/
      bool compile ();
      /** @brief print the message log from compiling this shader. **/
      void printLog (FILE *);
      /** @brief get the Id of this shader. **/
      GLuint GetId ();
      /** @brief create a new shader function **/
      void create (GLenum type);
      /** @brief destroy this shader function **/
      void Free ();
    };
  }
}