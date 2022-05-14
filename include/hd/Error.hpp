/**
 * @file Error.hpp
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
#include <cassert>
#define hdError(MESSAGE, ...)                                                 \
  fprintf (stderr, "%s:%d\t+++\tERROR\t+++\n\t", __FILE__, __LINE__);         \
  fprintf (stderr, MESSAGE, ##__VA_ARGS__);                                   \
  fprintf (stderr, "\n---\tERROR\r---\n")

#define hdErrorIf(CONDITION, ERROR_MESSAGE, ...)                              \
  if (CONDITION) {                                                            \
    hdError (ERROR_MESSAGE, ##__VA_ARGS__);                                   \
    assert (!CONDITION);                                                      \
  }
