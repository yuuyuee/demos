#ifndef OAK_VERSION_H_
#define OAK_VERSION_H_

#define OAK_VERSION_MAJOR 1
#define OAK_VERSION_MINOR 0
#define OAK_VERSION_PATCH 0

#define OAK_VERSION_SUFFIX "devel"

#define OAK_VERSION ((OAK_VERSION_MAJOR << 16) | \
                     (OAK_VERSION_MINOR << 8)  | \
                     (OAK_VERSION_PATCH))

unsigned int oak_version();
const char* oak_version_str();

#endif /* TOOLKITS_VERSION_H_ */
