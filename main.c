#include "./mylib.h"
#include <arpa/inet.h>
#include <bits/pthreadtypes.h>
#include <bits/types/struct_iovec.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void startListeningAndPrintMessagesOnNewThread(int socketFD);
void *listenAndPrint(void *socketFD);

int main() {

  int socketFD = createTCPIpv4Socket();

  const char *ip = "127.0.0.1";
  int port = 2000;
  struct sockaddr_in *address = createIPV4Address(ip, port);

  int result = connect(socketFD, (struct sockaddr *)address, sizeof *address);

  if (result == 0) {
    printf("connection was successful\n");
  }

  char *name = NULL;
  size_t nameSize = 0;
  printf("Please enter your name: \n");
  ssize_t nameCount = getline(&name, &nameSize, stdin);
  name[nameCount - 1] = 0;

  char *line = NULL;
  size_t lineSize = 0;
  printf("Type the message you want to send. Type exit to break... \n");

  startListeningAndPrintMessagesOnNewThread(socketFD);

  char buffer[1204];

  while (true) {
    ssize_t charcount = getline(&line, &lineSize, stdin);
    line[charcount - 1] = 0;
    sprintf(buffer, "%s:%s", name, line);

    if (charcount > 0) {
      if (strcmp(line, "exit") == 0)
        break;

      ssize_t amountWasSent = send(socketFD, buffer, strlen(buffer), 0);
    }
  }

  close(socketFD);

  return 0;
}

void startListeningAndPrintMessagesOnNewThread(int socketFD) {
  pthread_t id;
  int *psocketFD = malloc(sizeof(int));
  *psocketFD = socketFD;
  pthread_create(&id, NULL, listenAndPrint, psocketFD);
}

void *listenAndPrint(void *fd) {
  int socketFD = *((int *)fd);

  char buffer[1024];
  while (true) {
    ssize_t amountRecieved = recv(socketFD, buffer, 1024, 0);
    if (amountRecieved > 0) {
      buffer[amountRecieved] = 0;
      printf("%s\n", buffer);
    }

    if (amountRecieved == 0) {
      break;
    }
  }
  close(socketFD);
}
