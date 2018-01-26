#define RED_TEXT "\x1B[31m"
#define GREEN_TEXT "\x1b[32m"
#define YELLOW_TEXT  "\x1B[33m"
#define GREEN_BOLD  "\x1B[32;1m"
#define COLOR_RESET "\x1b[0m"

int to_client;
char username[BUFFER_SIZE];
char filepath[BUFFER_SIZE];
void subserver(int from_client);
void user_folder(char *s);
void server_publs(char *out);
void server_pubup(char *filename, char *content, char *out);
void server_pubdown(char *file, char *out);
void server_pubswitch(char *newuser);
void server_pubdel(char *out);