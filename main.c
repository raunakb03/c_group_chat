#include "./mylib.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>


int main() {

  int socketFD = createTCPIpv4Socket();

  const char *ip = "127.0.0.1";
  int port = 2000;
  struct sockaddr_in *address = createIPV4Address(ip, port);

  int result = connect(socketFD, (struct sockaddr *)address, sizeof *address);

  if (result == 0) {
    printf("connection was successful\n");
  }

  const char *message;
  message = "GET \\ HTTP/1.1\r\nHost:google.com\r\n\r\n";
  send(socketFD, message, strlen(message), 0);

  char buffer[1024];
  recv(socketFD, buffer, 1024, 0);

  printf("Data received: %s\n", buffer);

  return 0;
}
