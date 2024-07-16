#include "./mylib.h"
#include <arpa/inet.h>
#include <bits/types/struct_iovec.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {

  int socketFD = createTCPIpv4Socket();

  const char *ip = "127.0.0.1";
  int port = 2000;
  struct sockaddr_in *address = createIPV4Address(ip, port);

  int result = connect(socketFD, (struct sockaddr *)address, sizeof *address);

  if (result == 0) {
    printf("connection was successful\n");
  }

  char *line = NULL;
  size_t lineSize = 0;
  printf("Type the message you want to send. Type exit to break... \n");

  while (true) {
    ssize_t charcount = getline(&line, &lineSize, stdin);

    if (charcount > 0) {

      if (strcmp(line, "exit\n") == 0)
        break;

      ssize_t amountWasSent = send(socketFD, line, charcount, 0);
    }
  }

  close(socketFD);

  return 0;
}
