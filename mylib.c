#include "mylib.h"
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

int createTCPIpv4Socket() {
  int socketFD = socket(AF_INET, SOCK_STREAM, 0);
  return socketFD;
}

struct sockaddr_in *createIPV4Address(const char *ip, int port) {
  struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in));
  address->sin_family = AF_INET;
  address->sin_port = htons(port);

  if (strlen(ip) == 0)
    address->sin_addr.s_addr = INADDR_ANY;
  else
    inet_pton(AF_INET, ip, &address->sin_addr.s_addr);

  return address;
}
