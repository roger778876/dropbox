#include "input.h"
#include "execute.h"
#include "pipe_networking.h"


int to_server;
int from_server;

/* ORIGINAL SHELL SPECIAL COMMANDS
*/
void change_dir(char **command) {
  if (chdir(command[1])) {
    printf("%s\n", strerror(errno));
  }
}

void exit_program() {
  kill(getpid(), SIGKILL);
}


/* PICKUPBOX SPECIAL COMMANDS
   pubhelp, publs, pubup, pubdown, pubdel, pubrename, pubinfo, pubuser, pubswitch, pubremove
*/

/* PUB manual
*/
void pubhelp() {
  printf(CYAN_BOLD "Pickupbox commands:\n" COLOR_RESET);
  printf(CYAN_TEXT "publs" COLOR_RESET);
  printf(" - shows list of files in your PUB\n");
  printf(CYAN_TEXT "pubup [local file] ([PUB name])" COLOR_RESET);
  printf(" - uploads local file to your PUB\n");
  printf(CYAN_TEXT "pubdown [PUB file] ([local name])" COLOR_RESET);
  printf(" - downloads PUB file to your current directory\n");
  printf(CYAN_TEXT "pubdel [PUB file]" COLOR_RESET);
  printf(" - deletes PUB file from your PUB\n");
  printf(CYAN_TEXT "pubrename [PUB file] [new name]" COLOR_RESET);
  printf(" - renames PUB file\n");
  printf(CYAN_TEXT "pubinfo [size/amt]" COLOR_RESET);
  printf(" - shows properties of your PUB\n");
  printf(CYAN_TEXT "pubuser" COLOR_RESET);
  printf(" - shows current PUB user\n");
  printf(CYAN_TEXT "pubswitch [username]" COLOR_RESET);
  printf(" - changes to existing or new user\n");
  printf(CYAN_TEXT "pubremove" COLOR_RESET);
  printf(" - deletes your PUB permanently\n");
}

/* Lists all files in the user's PUB.
   Sends "publs" to subserver. Receives list of files string.
*/
void publs() {
  char input[BUFFER_SIZE] = "publs";
  write(to_server, input, sizeof(input));

  char output[BUFFER_SIZE];
  read(from_server, output, sizeof(output));

  printf(CYAN_BOLD "Your PUB files:\n" COLOR_RESET);
  printf("%s", output);
}

/* Uploads local file to user's PUB.
   Can optionally upload under different filename through char *pubname.
   Sends "pubup::filename::contents" to server. Receives confirmation message.
*/
void pubup(char *localfile, char *pubname) {
  int fd = open(localfile, O_RDONLY);
  struct stat s;
  fstat(fd, &s);
  int filesize = s.st_size; // to see if file exceeds FILE_SIZE

  if (fd == -1) { // file doesn't exist
    printf(CYAN_BOLD "Couldn't find local file!\n" COLOR_RESET);
  }
  else if (filesize > FILE_SIZE) { // file too large
    printf(CYAN_BOLD "File is too large!\n" COLOR_RESET);
  }
  else {
    char content[FILE_SIZE];
    int bytes = read(fd, content, sizeof(content)); // stores file into "content"
    close(fd);

    char input[FILE_SIZE + 9] = "pubup::";
    if (strlen(pubname) == 0) { // if no new name specified
      strcat(input, localfile);
    }
    else { // specified new file name
      strcat(input, pubname);
    }
    strcat(input, "::");
    strcat(input, content);
    write(to_server, input, sizeof(input));

    char output[BUFFER_SIZE];
    read(from_server, output, sizeof(output));
    printf(CYAN_BOLD "%s" COLOR_RESET, output); // confirmation message
  }
}

/* Downloads specified file from user's PUB to current directory.
   Can specify download file name through char *newfile.
   Sends "pubdown::filename" to server. Receives file contents.
*/
void pubdown(char *pubfile, char *newfile) {
  char input[BUFFER_SIZE] = "pubdown::";
  strcat(input, pubfile);
  write(to_server, input, sizeof(input));

  char content[FILE_SIZE];
  read(from_server, content, sizeof(content)); // receives file contents

  int fd;
  if (strlen(newfile) == 0) { // no file name specified
    fd = open(pubfile, O_RDWR | O_CREAT | O_EXCL, 0700);
    write(fd, content, strlen(content));
    close(fd);
    printf(CYAN_BOLD "Successfully downloaded \"%s\"!\n" COLOR_RESET, pubfile);
  }
  else { // new file name specified
    fd = open(newfile, O_RDWR | O_CREAT | O_EXCL, 0700);
    write(fd, content, strlen(content));
    close(fd);
    printf(CYAN_BOLD "Successfully downloaded \"%s\" as \"%s\"!\n" COLOR_RESET, pubfile, newfile);
  }
}

/* Deletes file from user's PUB.
   Sends "pubdel::filename" to server. Receives confirmation message.
*/
void pubdel(char *file) {
  char input[BUFFER_SIZE] = "pubdel::";
  strcat(input, file);
  write(to_server, input, sizeof(input));

  char output[BUFFER_SIZE];
  read(from_server, output, sizeof(output));
  printf(CYAN_BOLD "%s" COLOR_RESET, output);
}

/* Renames file in user's PUB.
   Sends "pubrename::oldname::newname" to server. Receives confirmation message.
*/
void pubrename(char *oldname, char *newname) {
  char input[BUFFER_SIZE] = "pubrename::";
  strcat(input, oldname);
  strcat(input, "::");
  strcat(input, newname);
  write(to_server, input, sizeof(input));

  char output[BUFFER_SIZE];
  read(from_server, output, sizeof(output));
  printf(CYAN_BOLD "%s" COLOR_RESET, output);
}

/* Shows properties of user's PUB.
   size property shows byte size of all files in PUB.
   amt property shows number of files in PUB.
   Sends "pubinfo::property" to server. Receives size or amt.
*/
void pubinfo(char *property) {
  if (!strcmp(property, "size") || !strcmp(property, "amt")) {
    char input[BUFFER_SIZE] = "pubinfo::";
    strcat(input, property);
    write(to_server, input, sizeof(input));

    char output[BUFFER_SIZE];
    read(from_server, output, sizeof(output));
    printf(CYAN_BOLD "%s" COLOR_RESET, output);
  }
  else {
    printf(CYAN_BOLD "Invalid PUB property!\n" COLOR_RESET);
  }
}

/* Shows current PUB user.
   Sends "pubuser" to server. Receives username.
*/
void pubuser() {
  char input[BUFFER_SIZE] = "pubuser";
  write(to_server, input, sizeof(input));

  char output[BUFFER_SIZE];
  read(from_server, output, sizeof(output));
  printf(CYAN_BOLD "%s" COLOR_RESET, output);
}

/* Changes PUB user.
   If user already exists, switches to that user. Otherwise, creates new user.
   Sends "pubswitch::username" to server. Receives confirmation message.
*/
void pubswitch(char *user) {
  char input[BUFFER_SIZE] = "pubswitch::";
  strcat(input, user);
  write(to_server, input, sizeof(input));

  char output[BUFFER_SIZE];
  read(from_server, output, sizeof(output));
  printf(CYAN_BOLD "%s\n" COLOR_RESET, output);
}

/* Removes a user's entire PUB. Asks for confirmation twice.
   Sends "pubremove" to server. Receives confirmation message and exits client program.
*/
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
      write(to_server, "pubremove", sizeof("pubremove"));

      char output[BUFFER_SIZE];
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


/* Routes shell commands.
*/
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

  /* PUB COMMANDS
     Makes sure there are correct number of args.
  */
  else if (!strcmp(args[0], "pubhelp")) {
    pubhelp();
  }
  else if (!strcmp(args[0], "publs")) {
    publs();
  }
  else if (!strcmp(args[0], "pubup")) {
    if (!args[1]) {
      printf(CYAN_BOLD "Please specify a local file to upload.\n" COLOR_RESET);
    }
    else {
      if (!args[2]) {
        pubup(args[1], "");
      }
      else {
        pubup(args[1], args[2]);
      }
    }
  }
  else if (!strcmp(args[0], "pubdown")) {
    if (!args[1]) {
      printf(CYAN_BOLD "Please specify a PUB file to download.\n" COLOR_RESET);
    }
    else {
      if (!args[2]) {
        pubdown(args[1], "");
      }
      else {
        pubdown(args[1], args[2]);
      }
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
  else if (!strcmp(args[0], "pubrename")) {
    if (!args[1] || !args[2]) {
      printf(CYAN_BOLD "Please specify a PUB file and a new name.\n" COLOR_RESET);
    }
    else {
      pubrename(args[1], args[2]);
    }
  }
  else if (!strcmp(args[0], "pubinfo")) {
    if (!args[1]) {
      printf(CYAN_BOLD "Please specify PUB property.\n" COLOR_RESET);
    }
    else {
      pubinfo(args[1]);
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

  /* All other commands */
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