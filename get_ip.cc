// Author:

#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif

#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>

#include <string.h>

#include <iostream>
#include <string>

void GetIP() {
  struct ifaddrs* ifaddr = NULL;
  getifaddrs(&ifaddr);
  char host[NI_MAXHOST];

  if (ifaddr) {
    for (struct ifaddrs* ptr = ifaddr; ptr; ptr = ptr->ifa_next) {
      if (!ptr->ifa_addr ||
          ptr->ifa_addr->sa_family != AF_INET ||
          strncmp(ptr->ifa_name, "lo", 2) == 0) {
        continue;
      }
      std::cout << "ifa_name: " << ptr->ifa_name << std::endl;
      int ret = getnameinfo(ptr->ifa_addr, sizeof(struct sockaddr_in),
          host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
      if (ret == 0) {
        std::cout << "ifa_addr: " << host << std::endl;
      }
    }
    freeifaddrs(ifaddr);
  }
}

int main() {
  GetIP();
  return 0;
}
