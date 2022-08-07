// Copyright 2022 The Oak Authors.

#ifndef OAK_COMMON_CIRCLE_H_
#define OAK_COMMON_CIRCLE_H_

#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef void* Circle[2];

#define OAK_CIRCLE_DATA(ptr, type, field)               \
  ((type *) (((char *) (ptr)) - offsetof(type, field)))   // NOLINT

#define OAK_CIRCLE_NEXT(n) (*(Circle**) &((*(n))[0]))     // NOLINT
#define OAK_CIRCLE_PREV(n) (*(Circle**) &((*(n))[1]))     // NOLINT

#define OAK_CIRCLE_HEAD(n) (OAK_CIRCLE_NEXT(n))

#define OAK_CIRCLE_INIT(n) do {                         \
  OAK_CIRCLE_NEXT(n) = (n);                             \
  OAK_CIRCLE_PREV(n) = (n);                             \
} while (0)

#define OAK_CIRCLE_EMPTY(h)                             \
  ((const Circle *)(h) == (const Circle *)OAK_CIRCLE_NEXT(h))

#define OAK_CIRCLE_PREV_NEXT(n) (OAK_CIRCLE_NEXT(OAK_CIRCLE_PREV(n)))
#define OAK_CIRCLE_NEXT_PREV(n) (OAK_CIRCLE_PREV(OAK_CIRCLE_NEXT(n)))

#define OAK_CIRCLE_FOREACH(n, h)                        \
  for ((n) = OAK_CIRCLE_NEXT(h); (n) != (h); (n) = OAK_CIRCLE_NEXT(n))

#define OAK_CIRCLE_INSERT_HEAD(h, n) do {               \
  OAK_CIRCLE_NEXT(n) = OAK_CIRCLE_NEXT(h);              \
  OAK_CIRCLE_PREV(n) = (h);                             \
  OAK_CIRCLE_NEXT_PREV(n) = (n);                        \
  OAK_CIRCLE_NEXT(h) = (n);                             \
} while (0)

#define OAK_CIRCLE_INSERT_TAIL(h, n) do {               \
  OAK_CIRCLE_NEXT(n) = (h);                             \
  OAK_CIRCLE_PREV(n) = OAK_CIRCLE_PREV(h);              \
  OAK_CIRCLE_PREV_NEXT(n) = (n);                        \
  OAK_CIRCLE_PREV(h) = (n);                             \
} while (0)

#define OAK_CIRCLE_ADD(h, h2) do {                      \
  OAK_CIRCLE_PREV_NEXT(h) = OAK_CIRCLE_NEXT(h2);        \
  OAK_CIRCLE_NEXT_PREV(h2) = OAK_CIRCLE_PREV(h2);       \
  OAK_CIRCLE_PREV(h) = OAK_CIRCLE_PREV(h2);             \
  OAK_CIRCLE_PREV_NEXT(h) = (h);                        \
} while (0)

#define OAK_CIRCLE_REMOVE(n) do {                       \
  OAK_CIRCLE_PREV_NEXT(n) = OAK_CIRCLE_NEXT(n);         \
  OAK_CIRCLE_NEXT_PREV(n) = OAK_CIRCLE_PREV(n);         \
} while (0)

#define OAK_CIRCLE_SPLIT(h, n, h2) do {                 \
  OAK_CIRCLE_PREV(h2) = OAK_CIRCLE_PREV(h);             \
  OAK_CIRCLE_PREV_NEXT(h2) = (h2);                      \
  OAK_CIRCLE_NEXT(h2) = (n);                            \
  OAK_CIRCLE_PREV(h) = OAK_CIRCLE_PREV(n);              \
  OAK_CIRCLE_PREV_NEXT(h) = (h);                        \
  OAK_CIRCLE_PREV(n) = (h2);                            \
} while (0)

#define OAK_CIRCLE_MOVE(h, h2) do {                     \
  if (OAK_CIRCLE_EMPTY(h)) {                            \
    OAK_CIRCLE_INIT(h2);                                \
  } else {                                              \
    Circle* n = OAK_CIRCLE_HEAD(h);                     \
    OAK_CIRCLE_SPLIT(h, n, h2);                         \
  }                                                     \
} while (0)

#if defined(__cplusplus)
}
#endif

#endif  // OAK_COMMON_CIRCLE_H_
