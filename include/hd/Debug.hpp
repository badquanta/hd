/**
 * @file Debug.hpp
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
#include "hd/Config.hpp"
#include <stdio.h>
#ifdef HD_DEBUG_BUILD

/**
 * @brief output a message if debugging is enabled.
 *
 */

#define hdDebug(MESSAGE, ...)                                                 \
  fprintf (stderr, "%s:%d\t", __FILE__, __LINE__);                            \
  fprintf (stderr, MESSAGE, ##__VA_ARGS__);                                   \
  fprintf (stderr, "\n")

/**
 * @brief output a message if debugging is enabled.
 *
 */

#define hdDebugCall(MESSAGE, ...)                                             \
  fprintf (stderr, "%s:%d ", __FILE__, __LINE__);                             \
  fprintf (stderr, "%s (", __FUNCTION__);                                     \
  if (MESSAGE != NULL) {                                                      \
    fprintf (stderr, MESSAGE, ##__VA_ARGS__);                                 \
  }                                                                           \
  fprintf (stderr, ")\n")

/**
 * @brief output a message if debugging is enabled.
 *
 */

#define hdDebugReturn(MESSAGE, ...)                                           \
  fprintf (stderr, "%s:%d ", __FILE__, __LINE__);                             \
  fprintf (stderr, "%s () returns `", __FUNCTION__);                          \
  if (MESSAGE != NULL) {                                                      \
    fprintf (stderr, MESSAGE, ##__VA_ARGS__);                                 \
  }                                                                           \
  fprintf (stderr, "`\n")

#else

/**
 * @brief output a message if debugging is enabled.
 *
 */

#define hdDebugReturn(message, ...) true

/**
 * @brief output a message if debugging is enabled.
 *
 */

#define hdDebugCall(message, ...) true

/**
 * @brief output a message if debugging is enabled.
 *
 */

#define hdDebug(message, ...) true

#endif