#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "pipe_networking.h"
#include "startup.c"

int main() {
  startuptext(0);


  int to_server;
  int from_server;
  char buffer[BUFFER_SIZE];

  from_server = client_handshake( &to_server );
<<<<<<< HEAD

=======
  
>>>>>>> 78929e6cfa97b1202a6ce2b79ca7d25ec1e80b7d
  while (1) {
    printf("enter data: ");
    fgets(buffer, sizeof(buffer), stdin);
    *strchr(buffer, '\n') = 0;
    write(to_server, buffer, sizeof(buffer));
    read(from_server, buffer, sizeof(buffer));
    printf("received: [%s]\n", buffer);
  }
}
