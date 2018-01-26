#include <sys/wait.h>
#include "input.h"
#include "execute.h"
#include "pipe_networking.h"

int to_server;
int from_server;

void change_dir(char **command) {
  if (chdir(command[1])) {
    printf("%s\n", strerror(errno));
  }
}

void exit_program() {
  kill(getpid(), SIGKILL);
}

void pubhelp() {
  printf(CYAN_BOLD "Pickupbox commands:\n" COLOR_RESET);
  printf(CYAN_TEXT "publs" COLOR_RESET);
  printf(" - shows list of files in your PUB\n");
  printf(CYAN_TEXT "pubup [local file] [PUB name]" COLOR_RESET);
  printf(" - uploads local file to your PUB\n");
  printf(CYAN_TEXT "pubdown [PUB file]" COLOR_RESET);
  printf(" - downloads PUB file to your current directory\n");
  printf(CYAN_TEXT "pubdel [PUB file]" COLOR_RESET);
  printf(" - deletes PUB file from your PUB\n");
  printf(CYAN_TEXT "pubuser" COLOR_RESET);
  printf(" - shows current PUB user\n");
  printf(CYAN_TEXT "pubswitch [username]" COLOR_RESET);
  printf(" - changes to existing or new user\n");
  printf(CYAN_TEXT "pubremove" COLOR_RESET);
  printf(" - deletes your PUB permanently\n");
}

void publs() {
  char input[BUFFER_SIZE] = "publs";
  char output[BUFFER_SIZE];
  write(to_server, input, sizeof(input));
  read(from_server, output, sizeof(output));
  printf(CYAN_BOLD "Your PUB files:\n" COLOR_RESET);
  printf("%s", output);
}

void pubup(char *localfile, char *pubname) {
  // printf("%s\n", path);
  int fd = open(localfile, O_RDONLY);
  struct stat s;
  fstat(fd, &s);
  int filesize = s.st_size;

  if (fd == -1) {
    printf(CYAN_BOLD "Couldn't find local file!\n" COLOR_RESET);
  }
  else if (filesize > FILE_SIZE) {
    printf(CYAN_BOLD "File is too large!\n" COLOR_RESET);
  }
  else {
    char content[FILE_SIZE];
    int bytes = read(fd, content, sizeof(content));
    // printf("%s\n", content);
    close(fd);

    char input[FILE_SIZE + 9] = "pubup::";
    char output[BUFFER_SIZE];
    strcat(input, pubname);
    strcat(input, "::");
    strcat(input, content);
    write(to_server, input, sizeof(input));
    read(from_server, output, sizeof(output));
    printf(CYAN_BOLD "%s" COLOR_RESET, output);
  }
}

void pubdown(char *file) {
  char input[BUFFER_SIZE] = "pubdown::";
  char output[BUFFER_SIZE];
  strcat(input, file);
  write(to_server, input, sizeof(input));
  read(from_server, output, sizeof(output));
  printf(CYAN_BOLD "%s" COLOR_RESET, output);
}

void pubdel(char *file) {
  char input[BUFFER_SIZE] = "pubdel::";
  char output[BUFFER_SIZE];
  strcat(input, file);
  write(to_server, input, sizeof(input));
  read(from_server, output, sizeof(output));
  printf(CYAN_BOLD "%s" COLOR_RESET, output);
}

void pubuser() {
  char input[BUFFER_SIZE] = "pubuser";
  char output[BUFFER_SIZE];
  write(to_server, input, sizeof(input));
  read(from_server, output, sizeof(output));
  printf(CYAN_BOLD "%s" COLOR_RESET, output);
}

void pubswitch(char *user) {
  char input[BUFFER_SIZE] = "pubswitch::";
  char output[BUFFER_SIZE];
  strcat(input, user);
  write(to_server, input, sizeof(input));
  read(from_server, output, sizeof(output));
  printf(CYAN_BOLD "%s\n" COLOR_RESET, output);
}

void pubremove() {
  printf(RED_BOLD "You are permanently deleting all your PUB files. Are you sure? (Yes/No) " COLOR_RESET);
  char answer[100];
  fgets(answer, sizeof(answer), stdin);
  *strchr(answer, '\n') = 0;
  if (!strcmp(answer, "Yes")) {
    printf(RED_BOLD "Confirm delete? (DELETE/NO) " COLOR_RESET);
    fgets(answer, sizeof(answer), stdin);
    *strchr(answer, '\n') = 0;

    if (!strcmp(answer, "DELETE")) {
      char output[BUFFER_SIZE];
      write(to_server, "pubremove", sizeof("pubremove"));
      read(from_server, output, sizeof(output));
      printf(CYAN_BOLD "%s" COLOR_RESET, output);
      printf("Exiting Pickupbox client...\n");
      exit(0);
    }
    else {
      printf(CYAN_BOLD "Cancelled\n" COLOR_RESET);
    }
  }
  else {
    printf(CYAN_BOLD "Cancelled\n" COLOR_RESET);
  }
}

void execute(char *command, int to_s, int from_s) {
  char **args = separate_args(command);
  to_server = to_s;
  from_server = from_s;

  if (!args[0]) {
    return;
  }
  else if (!strcmp(args[0], "cd")) {
    change_dir(args);
  }
  else if (!strcmp(args[0], "exit")) {
    exit_program();
  }

  else if (!strcmp(args[0], "pubhelp")) {
    pubhelp();
  }
  else if (!strcmp(args[0], "publs")) {
    publs();
  }
  else if (!strcmp(args[0], "pubup")) {
    if (!args[1] || !args[2]) {
      printf(CYAN_BOLD "Please specify a local file and PUB file name.\n" COLOR_RESET);
    }
    else {
      pubup(args[1], args[2]);
    }
  }
  else if (!strcmp(args[0], "pubdown")) {
    if (!args[1]) {
      printf(CYAN_BOLD "Please specify a file.\n" COLOR_RESET);
    }
    else {
      pubdown(args[1]);
    }
  }
  else if (!strcmp(args[0], "pubdel")) {
    if (!args[1]) {
      printf(CYAN_BOLD "Please specify a PUB file to delete.\n" COLOR_RESET);
    }
    else {
      pubdel(args[1]);
    }
  }
  else if (!strcmp(args[0], "pubuser")) {
    pubuser();
  }
  else if (!strcmp(args[0], "pubswitch")) {
    if (!args[1]) {
      printf(CYAN_BOLD "Please specify another username.\n" COLOR_RESET);
    }
    else {
      pubswitch(args[1]);
    }
  }
  else if (!strcmp(args[0], "pubremove")) {
    pubremove();
  }

  else {
    int piping = 0;
    for (int i = 0; args[i]; i++) {
      if (!strcmp(args[i], "|")) {
        piping++;
        break;
      }
    }

    if (piping) {
      char *cmds[2];
      int i = 0;
      while (command) {
        cmds[i] = strsep(&command, "|");
        i++;
      }
      FILE *p = popen(cmds[0], "r");
      int parent = fork();
      if (parent) {
        int pc = pclose(p);
        int status;
        wait(&status);
      }

      if (!parent) {
        char **cmd2 = separate_args(cmds[1]);
        dup2(fileno(p), 0);
        if (execvp(cmd2[0], cmd2)) {
          printf("%s\n", strerror(errno));
          exit(EXIT_FAILURE);
        }
      }
    }

    else { // all other commands
      int parent = fork();
      if (parent) {
        int status;
        wait(&status);
      }

      if (!parent) {
        for (int i = 0; args[i]; i++) {
          if (!strcmp(args[i], "<")) {
            int fd = open(args[i + 1], O_RDONLY);
            // printf("fd: %d\n", fd);
            dup2(fd, 0);
            args[i] = NULL;
            i++;
          }
          else if (!strcmp(args[i], ">")) {
            int fd = open(args[i + 1], O_WRONLY | O_CREAT, 0644);
            // printf("fd: %d\n", fd);
            dup2(fd, 1);
            args[i] = NULL;
            i++;
          }
        }
        if (execvp(args[0], args)) {
          printf("%s\n", strerror(errno));
          exit(EXIT_FAILURE);
        }
      }
    }
  }
  free(args);
}


/*
int main() {
  execute(read_input());
  return 0;
}
*/