#include "pipe_networking.h"
#include "forking_server.h"
#include "subserver.h"
#include "startup.h"


static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove("luigi");
    exit(0);
  }
}

int main() {
  signal(SIGINT, sighandler);
  startuptext(1);
  int from_client;
  int parent;

  printf(GREEN_BOLD "[PUBServer]" COLOR_RESET);
  printf(" Waiting for new connection...\n");
  while (1) {
    from_client = server_setup();
    parent = fork();
    if (!parent) {
      subserver(from_client);
    }
  }
}