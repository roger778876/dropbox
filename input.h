#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define GREEN_TEXT "\x1b[32m"
#define GREEN_BOLD  "\x1B[32;1m"
#define COLOR_RESET "\x1b[0m"

struct cmds_array separate_cmds(char *line);
char **separate_args(char *cmd);
struct cmds_array read_input();