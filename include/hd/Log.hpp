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