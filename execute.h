#define CYAN_BOLD  "\x1B[36;1m"
#define COLOR_RESET "\x1b[0m"

int to_server;
int from_server;
void change_dir(char **command);
void exit_program();
void pubhelp();
void publs();
void execute(char *command, int to_s, int from_s);