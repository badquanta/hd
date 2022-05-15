/**
 * @file EBO.hpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-14
 *
 * @copyright GNU-GPL 3.0 Copyright (C) 2022 Jón Davíð Sawyer (badquanta@gmail.com)
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
namespace hd {
  namespace gl {
    /**
     * @brief Element (index) Buffer Object
     *
     */
    class EBO {
      private:
        GLuint ID;
      public:
        /**
         * @brief Construct a new EBO object
         *
         */
        EBO();
        /**
         * @brief Construct a new EBO object
         *
         * @param data
         * @param size
         */
        EBO (GLuint *data, GLsizeiptr size);
        /**
         * @brief Create an Element Buffer Object
         *
         */
        void Create (GLuint *, GLsizeiptr);
        /** @brief set this context as the active open gl context **/
        void Bind ();
        /** @brief clear the active open gl context **/
        void Unbind();
        /** @brief Destroy this Element Buffer Object **/
        void Delete();
    };
  }
}