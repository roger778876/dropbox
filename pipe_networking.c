#include "pipe_networking.h"

/*=========================
  server_setup
  args:

  creates the WKP (upstream) and opens it, waiting for a
  connection.

  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  int from_client;
  mkfifo("pubwkp", 0600);
  from_client = open("pubwkp", O_RDONLY, 0); // blocks
  remove("pubwkp");

  return from_client;
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  char buff[HANDSHAKE_BUFFER_SIZE];
  int to_client;

  //reading from wkp to get to client
  // printf("reading from well known pipe...\n");
  read(from_client, buff, sizeof(buff));
  // printf("server read: %s\n", buff);

  //writing to client through private pipe
  to_client = open(buff, O_WRONLY, 0);
  write(to_client, buff, sizeof(buff));

  //completing the handshake by reading from client one last time
  if(read(from_client, buff, sizeof(buff))){
    return to_client;
  }
  else {
    printf("PUBServer handshake not completed!\n");
    remove("pubwkp");
    exit(0);
  }
}

/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {

  int from_server;
  char buffer[HANDSHAKE_BUFFER_SIZE];

  //send pp name to server
  // printf("[client] handshake: connecting to wkp\n");
  *to_server = open( "pubwkp", O_WRONLY, 0);
  if ( *to_server == -1 ) { // if server isn't on
    printf(RED_TEXT "Couldn't connect to PUBServer\n" COLOR_RESET);
    exit(1);
  }

  //make private pipe
  sprintf(buffer, "%d", getpid() );
  mkfifo(buffer, 0600);

  write(*to_server, buffer, sizeof(buffer));

  //open and wait for connection
  from_server = open(buffer, O_RDONLY, 0);
  read(from_server, buffer, sizeof(buffer));
  /*validate buffer code goes here */
  // printf("[client] handshake: received [%s]\n", buffer);

  //remove pp
  remove(buffer);
  // printf("[client] handshake: removed pp\n");

  //send ACK to server
  write(*to_server, ACK, sizeof(buffer));

  printf("Connected to PUBServer!\n"); // confirmation message

  return from_server;
}
