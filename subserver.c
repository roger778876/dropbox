#include "pipe_networking.h"
#include "subserver.h"
#include <dirent.h>

int to_client;
char username[BUFFER_SIZE];
char filepath[BUFFER_SIZE] = "users/";

void subserver(int from_client) {
  to_client = server_connect(from_client);
  
  read(from_client, username, sizeof(username));
  printf(GREEN_BOLD "[PUBServer]" COLOR_RESET);
  printf(" Connected to client \"%s\".\n", username);
  printf(GREEN_BOLD "[PUBServer]" COLOR_RESET);
  printf(" Waiting for new connection...\n");

  strcat(filepath, username);
  user_folder(filepath);

  char request[FILE_SIZE + 10];
  char result[BUFFER_SIZE];
  char preview[PREVIEW_SIZE];
  while(read(from_client, request, sizeof(request))){
    printf(GREEN_BOLD "[PUBServer]" COLOR_RESET);
    printf(" Received request from client \"%s\": ", username);
    memset(preview, 0, strlen(preview));
    strncat(preview, request, (PREVIEW_SIZE * sizeof(char)));
    for (int i = PREVIEW_SIZE - 3; i < PREVIEW_SIZE; i++) {
      if (preview[i]) {
        preview[i] = '.';
      }
    }
    printf(YELLOW_TEXT "%s\n" COLOR_RESET, preview);

    memset(result, 0, strlen(result));
    if (!strcmp(request, "publs")) {
      server_publs(result);
      write(to_client, result, sizeof(result));
    }
    if (!strncmp(request, "pubup::", (7 * sizeof(char)))) {
      char *fileinfo = request + 7;
      char *filename = strsep(&fileinfo, "::");
      char *filecontent = fileinfo + 1;
      server_pubup(filename, filecontent, result);
      write(to_client, result, sizeof(result));
    }
    if (!strncmp(request, "pubdown::", (9 * sizeof(char)))) {
      char *pubfile = request + 9;

      char download[FILE_SIZE];
      server_pubdown(pubfile, download);
      write(to_client, download, sizeof(download));
    }
    if (!strncmp(request, "pubdel::", (8 * sizeof(char)))) {
      char *pubfile = request + 8;
      server_pubdel(pubfile, result);
      write(to_client, result, sizeof(result));
    }
    if (!strcmp(request, "pubuser")) {
      strcat(result, username);
      strcat(result, "\n");
      write(to_client, result, sizeof(result));
    }
    if (!strncmp(request, "pubswitch::", (11 * sizeof(char)))) {
      char *switchuser = request + 11;
      server_pubswitch(switchuser);
    }
    if (!strcmp(request, "pubremove")) {
      server_pubremove(result);
      write(to_client, result, sizeof(result));
      exit(0);
    }
  }

  exit(0);
}

void server_publs(char *out) {
  DIR *d;
  struct dirent *dir;
  d = opendir(filepath);
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

void server_pubup(char *filename, char *content, char *out) {
  char fullpath[BUFFER_SIZE];
  memset(fullpath, 0, strlen(fullpath));
  strcat(fullpath, filepath);
  strcat(fullpath, "/");
  strcat(fullpath, filename);

  if (access(fullpath, F_OK ) != -1) {
    strcat(out, "\"");
    strcat(out, filename);
    strcat(out, "\" already exists in your PUB!\n");
  }
  else {
    int fd = open(fullpath, O_RDWR | O_CREAT | O_EXCL, 0700);
    write(fd, content, strlen(content));
    close(fd);
    strcat(out, "Successfully uploaded \"");
    strcat(out, filename);
    strcat(out, "\" to your PUB!\n");
  }
}

void server_pubdown(char *file, char *out) {
  char fullpath[BUFFER_SIZE];
  memset(fullpath, 0, strlen(fullpath));
  strcat(fullpath, filepath);
  strcat(fullpath, "/");
  strcat(fullpath, file);
  // printf("%s\n", fullpath);

  if (access(fullpath, F_OK ) != -1) {
    int fd = open(fullpath, O_RDONLY);
    int bytes = read(fd, out, sizeof(out));
    close(fd);
  }
  else {
    strcat(out, "File \"");
    strcat(out, file);
    strcat(out, "\" not found!\n");
  }
}

void server_pubdel(char *file, char *out) {
  char fullpath[BUFFER_SIZE];
  memset(fullpath, 0, strlen(fullpath));
  strcat(fullpath, filepath);
  strcat(fullpath, "/");
  strcat(fullpath, file);
  // printf("%s\n", fullpath);

  if (access(fullpath, F_OK ) != -1) {
    remove(fullpath);
    strcat(out, "File \"");
    strcat(out, file);
    strcat(out, "\" deleted!\n");
  }
  else {
    strcat(out, "File \"");
    strcat(out, file);
    strcat(out, "\" not found!\n");
  }
}

void server_pubswitch(char *newuser) {
  memset(username, 0, strlen(username));
  memset(filepath, 0, strlen(filepath));
  strcat(username, newuser);
  strcat(filepath, "users/");
  strcat(filepath, newuser);
  user_folder(filepath);
}

void server_pubremove(char *out) {
  DIR *d = opendir(filepath);
  struct dirent *next_file;
  char path[BUFFER_SIZE];

  while ((next_file = readdir(d)) != NULL )
  {
      sprintf(path, "%s/%s", filepath, next_file->d_name);
      remove(path);
  }
  closedir(d);
  rmdir(filepath);

  strcat(out, "Successfully removed ");
  strcat(out, username);
  strcat(out, "'s PUB.\n");
}

void user_folder(char * userpath) { // creates new folder if necessary
  if (access(userpath, F_OK) != -1) {
    char welcome[BUFFER_SIZE] = "Welcome back, ";
    strcat(welcome, username);
    strcat(welcome, "!");
    write(to_client, welcome, sizeof(welcome));
  } 
  else {
    mkdir(userpath, 0700);
    char new[BUFFER_SIZE] = "Created new PUB for ";
    strcat(new, username);
    strcat(new, ". Welcome!");
    write(to_client, new, sizeof(new));
  }
}