#include "pipe_networking.h"
#include "client.h"
#include "execute.c"

int to_server;
int from_server;

int main() {
  startuptext(0);
  from_server = client_handshake( &to_server );
  login();

  shell();

}

void login() {
  char username[BUFFER_SIZE];
  char result[BUFFER_SIZE];

  printf(GREEN_TEXT "Enter new or existing username: " COLOR_RESET);
  fgets(username, sizeof(username), stdin);
  *strchr(username, '\n') = 0;
  write(to_server, username, sizeof(username));

  read(from_server, result, sizeof(result));
  printf(GREEN_TEXT "%s\n" COLOR_RESET, result);
}

void shell() {
  while (1) {
    struct cmds_array commands = read_input();
    int num_cmds = commands.num_cmds;
    int i = 0;
    while (num_cmds) {
      execute(commands.cmds[i]);
      num_cmds--;
      i++;
    }
    free(commands.cmds);
  }
}