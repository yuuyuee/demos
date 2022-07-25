
#ifndef OAK_INTERNAL_COMPILER_H_
#define OAK_INTERNAL_COMPILER_H_

#if !defined(__GNUC__) || !defined(__GNUC_MINOR__)
# error "This library requires GNU C compiler."
#endif

#define OAK_GNUC_PREREQ(major, minor) \
  ((__GNUC__ << 16) + __GNUC_MINOR__ >= ((major) << 16) + (minor))

#if !OAK_GNUC_PREREQ(4, 8)
# error "This library require GNU C version at least 4.8.x."
#endif

#include <features.h>

#if !defined(__GLIBC__) || !defined(__GLIBC_MINOR__)
# error "This library requires glibc package."
#endif

#define OAK_GLIBC_PREREQ(major, minor) \
  ((__GLIBC__ << 16) + __GLIBC_MINOR__ >= ((major) << 16) + (minor))

#if !OAK_GLIBC_PREREQ(2, 17)
# error "This library require glibc version at least 2.17."
#endif

#endif /* OAK_INTERNAL_COMPILER_H_ */
