#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "pipe_networking.c"
#include "startup.c"

#define RED_TEXT "\x1B[31m"
#define GREEN_TEXT "\x1b[32m"
#define COLOR_RESET "\x1b[0m"


static void sighandler(int signo) { // shuts down server when SIGINT
  if (signo == SIGINT) {
    remove("luigi");
    printf(RED_TEXT "\n[Server] Received SIGINT\n" COLOR_RESET);
    exit(0);
  }
}

int main() {
  signal(SIGINT, sighandler);
  startuptext(); // displays logo and loading text

  int from_client;
  int to_client;
  while (1) {
    from_client = server_setup();
    int parent = fork();
    if (parent) {
      remove("luigi");
      printf("[server] handshake: removed wkp\n");
      close(from_client);
    }
    else {

    }
  }
}