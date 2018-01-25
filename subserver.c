#include "pipe_networking.h"
#include "subserver.h"

int to_client;

void subserver(int from_client) {
  to_client = server_connect(from_client);

  char username[BUFFER_SIZE];
  
  read(from_client, username, sizeof(username));
  printf(GREEN_TEXT "[PUBServer]" COLOR_RESET);
  printf(" Connected to client \"%s\"\n", username);
  printf(GREEN_TEXT "[PUBServer]" COLOR_RESET);
  printf(" Waiting for new connection...\n");

  user_folder(username);

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

void user_folder(char * user) { // creates new folder if necessary
  if(access(user, F_OK) != -1) {
    char welcome[BUFFER_SIZE] = "Welcome back, ";
    strcat(welcome, user);
    strcat(welcome, "!");
    write(to_client, welcome, sizeof(welcome));
  } 
  else {
    mkdir(user, 0644);
    char new[BUFFER_SIZE] = "Created new user ";
    strcat(new, user);
    strcat(new, "!");
    write(to_client, new, sizeof(new));
  }
}