
#ifndef OAK_INTERNAL_PLATFORM_H_
#define OAK_INTERNAL_PLATFORM_H_

#if !(defined(__linux__) || defined(linux) || defined(__linux))
# error "This library requires linux operator system."
#endif

#if !(defined(__x86_64__) || defined(__x86_64))
# error "This library requires x86 architecture with 64-bit extensions."
#endif

#endif /* OAK_INTERNAL_PLATFORM_H_ */
