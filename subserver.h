#define RED_TEXT "\x1B[31m"
#define GREEN_TEXT "\x1b[32m"
#define GREEN_BOLD  "\x1B[32;1m"
#define COLOR_RESET "\x1b[0m"

int to_client;
char username[BUFFER_SIZE];
void subserver(int from_client);
void user_folder(char *s);
void server_publs(char *out);