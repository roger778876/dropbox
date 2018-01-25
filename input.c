#include "input.h"

struct cmds_array {
  char **cmds;
  size_t num_cmds;
};

struct cmds_array separate_cmds(char *line) {
  // duplicating line
  char *line_copy = strdup(line);

  // counting # of commands
  int num_cmds = 1;
  for (int i = 0; line[i]; i++) {
    if (line[i] == ';') {
      num_cmds++;
    }
  }
  // printf("num_cmds: %d\n", num_cmds);

  // separating into command array
  char **cmds = malloc(num_cmds * sizeof(char *));
  int i = 0;
  while (line_copy) {
    cmds[i] = strsep(&line_copy, ";");
    i++;
  }

  struct cmds_array output;
  output.cmds = cmds;
  output.num_cmds = num_cmds;
  return output;
}

char **separate_args(char *cmd) {
  // duplicating cmd
  char *cmd_copy = strdup(cmd);

  // counting # of args
  int num_args = 1;
  for (int i = 0; cmd[i]; i++) {
    if (cmd[i] == ' ') {
      num_args++;
    }
  }
  // printf("num_args: %d\n", num_args);

  // separating into arg array
  char **args = malloc((num_args + 1) * sizeof(char *)); // +1 to make room for NULL for execvp
  int i = 0;
  while (cmd_copy) {
    char *arg = strsep(&cmd_copy, " ");
    if (*arg) { // removes extra spaces
      args[i] = arg;
      i++;
    }
  }
  args[i] = 0; // ending NULL

  return args;
}


struct cmds_array read_input() {
  // getting current working directory
  char cwd[256];
  getcwd(cwd, sizeof(cwd));

  // getting input
  printf(GREEN_BOLD "%s$ " COLOR_RESET, cwd);
  char input[256];
  fgets(input, 256, stdin); // 256 bytes
  // printf("received input: [%s]\n", input);

  // removing last newline character from input
  input[strlen(input) - 1] = 0;

  struct cmds_array commands =  separate_cmds(input);
  return commands;
}

/*
int main() {
  char **args = read_input();
  printf("args[0]: [%s]\n", args[0]);
  printf("args[1]: [%s]\n", args[1]);
  printf("args[2]: [%s]\n", args[2]);
  return 0;
}
*/