#include "pipe_networking.h"
#include "client.h"
#include "execute.h"
#include "input.c"
#include "startup.h"


int to_server;
int from_server;

int main() {
  startuptext(0); // display startup text for client
  from_server = client_handshake(&to_server); // connects to server
  login();
  shell();
}

/*  Prompts user for a username, sends to subserver.
    If new user, creates new folder in "users".
*/
void login() {
  char username[BUFFER_SIZE];
  char result[BUFFER_SIZE];

  printf(GREEN_BOLD "Enter new or existing username: " COLOR_RESET);
  fgets(username, sizeof(username), stdin);
  *strchr(username, '\n') = 0;
  write(to_server, username, sizeof(username)); // sends username off to subserver

  read(from_server, result, sizeof(result));
  printf(GREEN_TEXT "%s\n" COLOR_RESET, result);
  
  printf("Type \"pubhelp\" for helpful commands!\n\n");
}

/*  Starts the command shell.
*/
void shell() {
  while (1) {
    struct cmds_array commands = read_input();
    int num_cmds = commands.num_cmds;
    int i = 0;
    while (num_cmds) {
      execute(commands.cmds[i], to_server, from_server);
      num_cmds--;
      i++;
    }
    free(commands.cmds);
  }
}