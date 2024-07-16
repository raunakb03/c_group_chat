#include "mylib.h"
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  int serverSocketFD = createTCPIpv4Socket();
  struct sockaddr_in *serverAddress = createIPV4Address("", 2000);

  int result = bind(serverSocketFD, (struct sockaddr *)serverAddress,
                    sizeof(*serverAddress));
  if (result == 0)
    printf("Socket was bound successfully\n");

  int listenResult = listen(serverSocketFD, 10);

  struct sockaddr_in clientAddress;
  int clientAddressSize = sizeof(struct sockaddr_in);
  int clientSocketFD = accept(serverSocketFD, (struct sockaddr *)&clientAddress,
                              (socklen_t *)&clientAddressSize);

  char buffer[1024];
  while (true) {
    ssize_t amountRecieved = recv(clientSocketFD, buffer, 1024, 0);
    if (amountRecieved > 0) {
      buffer[amountRecieved] = 0;
      printf("Response from client: %s\n", buffer);
    }

    if (amountRecieved == 0) {
      break;
    }
  }
  close(clientSocketFD);
  shutdown(serverSocketFD, SHUT_RDWR);

  return 0;
}
