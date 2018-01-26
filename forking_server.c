#include "pipe_networking.h"
#include "forking_server.h"
#include "subserver.h"
#include "startup.h"


/* Removes "pubwkp" on server kill
*/
static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove("pubwkp");
    exit(0);
  }
}

/* Loads server. Waits for first connection, 
   then loops forever and waits for more connections.
*/
int main() {
  signal(SIGINT, sighandler);
  startuptext(1); // server startuptext
  root_folder(); // creates "users" folder

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
    else {
      close(from_client);
    }
  }
}

/* Creates "users" folder where user PUBs and files are stored.
*/
void root_folder() {
  if (access("users", F_OK) != -1) {
    printf(GREEN_BOLD "[PUBServer]" COLOR_RESET);
    printf(" Root users folder found!\n");
  }
  else {
    mkdir("users", 0700);
    printf(GREEN_BOLD "[PUBServer]" COLOR_RESET);
    printf(" Root users folder created!\n");
  }
}