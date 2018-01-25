#include "pipe_networking.h"
#include "subserver.h"
#include <dirent.h>

int to_client;
char username[BUFFER_SIZE];

void subserver(int from_client) {
  to_client = server_connect(from_client);
  
  read(from_client, username, sizeof(username));
  printf(GREEN_TEXT "[PUBServer]" COLOR_RESET);
  printf(" Connected to client \"%s\"\n", username);
  printf(GREEN_TEXT "[PUBServer]" COLOR_RESET);
  printf(" Waiting for new connection...\n");

  user_folder(username);

  char request[BUFFER_SIZE];
  char result[BUFFER_SIZE];
  while(read(from_client, request, sizeof(request))){
    memset(result, 0, strlen(result));
    if (!strcmp(request, "publs")) {
      server_publs(result);
    }
    write(to_client, result, sizeof(result));
  }

  exit(0);
}

void server_publs(char *out) {
  DIR *d;
  struct dirent *dir;
  d = opendir(username);
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if (strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..")) {
        strcat(out, dir->d_name);
        strcat(out, "\n");
      }
    }
    closedir(d);
  }
}

void user_folder(char * user) { // creates new folder if necessary
  if(access(user, F_OK) != -1) {
    char welcome[BUFFER_SIZE] = "Welcome back, ";
    strcat(welcome, user);
    strcat(welcome, "!");
    write(to_client, welcome, sizeof(welcome));
  } 
  else {
    mkdir(user, 0700);
    char new[BUFFER_SIZE] = "Created new user ";
    strcat(new, user);
    strcat(new, "!");
    write(to_client, new, sizeof(new));
  }
}