#include "pipe_networking.h"
#include "client.h"

int to_server;
int from_server;

int main() {
  startuptext(0);
  from_server = client_handshake( &to_server );
  login();


  /*
  while (1) {
    printf("enter data: ");
    fgets(buffer, sizeof(buffer), stdin);
    *strchr(buffer, '\n') = 0;
    write(to_server, buffer, sizeof(buffer));
    read(from_server, buffer, sizeof(buffer));
    printf("received: [%s]\n", buffer);
  }
  */
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