/**
 * @file Log.hpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief MACRO definitions of logging: log, error, & debug.
 *
 * @version 0.1
 * @date 2022-05-13
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

#define hdLog(MESSAGE, ...)                                                   \
  fprintf (stdout, "%s -> ", __PRETTY_FUNCTION__);                            \
  fprintf (stdout, MESSAGE, ##__VA_ARGS__);                                   \
  fprintf (stdout, "\n")

#define hdError(MESSAGE, ...)                                                 \
  fprintf (stderr, "ERROR %s:%d\t", __FILE__, __LINE__);                      \
  fprintf (stderr, MESSAGE, ##__VA_ARGS__);                                   \
  fprintf (stderr, "\n")


#ifdef HD_DEBUG_BUILD


#define hdDebug(MESSAGE, ...)                                                 \
  fprintf (stderr, "%s:%d\t", __FILE__, __LINE__);                            \
  fprintf (stderr, MESSAGE, ##__VA_ARGS__);                                   \
  fprintf (stderr, "\n")

#define hdDebugCall(MESSAGE, ...)                                             \
  fprintf (stderr, "%s:%d ", __FILE__, __LINE__);                       \
  fprintf (stderr, "%s (", __FUNCTION__);                           \
  if (MESSAGE != NULL) {                                                      \
    fprintf (stderr, MESSAGE, ##__VA_ARGS__);                                 \
  }                                                                           \
  fprintf (stderr, ")\n")

#define hdDebugReturn(MESSAGE, ...)                                             \
  fprintf (stderr, "%s:%d ", __FILE__, __LINE__);                       \
  fprintf (stderr, "%s () returns `", __FUNCTION__);                           \
  if (MESSAGE != NULL) {                                                      \
    fprintf (stderr, MESSAGE, ##__VA_ARGS__);                                 \
  }                                                                           \
  fprintf (stderr, "`\n")

#else

#define hdDebugReturn(message,...)\
  true

#define hdDebugCall(message,...)\
  true

#define hdDebug(message,...)\
  true

#endif