#include "pipe_networking.h"
#include "subserver.h"

void subserver(int from_client) {
  char buffer[BUFFER_SIZE];
  int to_client = server_connect(from_client);
  
  read(from_client, buffer, sizeof(buffer));
  printf(GREEN_TEXT "[PUBServer]" COLOR_RESET);
  printf(" Connected to client \"%s\"\n", buffer);
  printf(GREEN_TEXT "[PUBServer]" COLOR_RESET);
  printf(" Waiting for new connection...\n");

/*
  while(read(from_client, buffer, sizeof(buffer))){
    printf("subserver %d receieved the following: %s\n", getpid(), buffer);
    process(buffer);
    printf("subserver %d sending the following: %s\n", getpid(), buffer);
    write(to_client, buffer, sizeof(buffer));
  }
  */

  


  exit(0);
}

void process(char * s) {
  //turning all c's to b's
  int counter = 0;
  while(s[counter]){
    if(s[counter] == 'a'){
      s[counter] = 'b';
    }
    counter++;
  }
}