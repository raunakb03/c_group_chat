#include "mylib.h"
#include <bits/pthreadtypes.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

struct AcceptedSocket {
  int acceptedSocketFD;
  struct sockaddr_in address;
  int error;
  bool acceptedSuccessfully;
};

struct AcceptedSocket *acceptIncomingConnection(int serverSocketFD);
void *receiveAndPrintIncomingData(void *socketFD);
void startAcceptingIncomingConnections(int serverSocketFD);
void acceptNewConnectionAndReceiveAndPrintItsData(int serverSocketFD);
void receiveAndPrintIncomingDataonSeparateThread(
    struct AcceptedSocket *pSocket);

int main() {
  int serverSocketFD = createTCPIpv4Socket();
  struct sockaddr_in *serverAddress = createIPV4Address("", 2000);

  int result = bind(serverSocketFD, (struct sockaddr *)serverAddress,
                    sizeof(*serverAddress));
  if (result == 0)
    printf("Socket was bound successfully\n");

  int listenResult = listen(serverSocketFD, 10);

  startAcceptingIncomingConnections(serverSocketFD);

  shutdown(serverSocketFD, SHUT_RDWR);

  return 0;
}

void startAcceptingIncomingConnections(int serverSocketFD) {
  while (true) {
    struct AcceptedSocket *clientSocket =
        acceptIncomingConnection(serverSocketFD);
    receiveAndPrintIncomingDataonSeparateThread(clientSocket);
  }
}

void receiveAndPrintIncomingDataonSeparateThread(
    struct AcceptedSocket *pSocket) {
  pthread_t id;
  pthread_create(&id, NULL, receiveAndPrintIncomingData,
                 &(pSocket->acceptedSocketFD));
}

void *receiveAndPrintIncomingData(void *socketFD) {
  int fd = *((int *)socketFD);
  char buffer[1024];
  while (true) {
    ssize_t amountRecieved = recv(fd, buffer, 1024, 0);
    if (amountRecieved > 0) {
      buffer[amountRecieved] = 0;
      printf("Response from client: %s\n", buffer);
    }

    if (amountRecieved == 0) {
      break;
    }
  }
  close(fd);
  return NULL;
}

struct AcceptedSocket *acceptIncomingConnection(int serverSocketFD) {
  struct sockaddr_in clientAddress;
  int clientAddressSize = sizeof(struct sockaddr_in);
  int clientSocketFD = accept(serverSocketFD, (struct sockaddr *)&clientAddress,
                              (socklen_t *)&clientAddressSize);

  struct AcceptedSocket *acceptedSocket = malloc(sizeof(struct AcceptedSocket));
  acceptedSocket->address = clientAddress;
  acceptedSocket->acceptedSocketFD = clientSocketFD;
  acceptedSocket->acceptedSuccessfully = clientSocketFD > 0;

  if (!acceptedSocket->acceptedSuccessfully) {
    acceptedSocket->error = clientSocketFD;
  }

  return acceptedSocket;
}
