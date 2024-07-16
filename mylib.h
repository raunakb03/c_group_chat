#ifndef MYLIB_H
#define MYLIB_H

#include <netinet/in.h>

int createTCPIpv4Socket();
struct sockaddr_in *createIPV4Address(const char *ip, int port);

#endif 
